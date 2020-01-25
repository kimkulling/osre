/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#include "EditorApplication.h"

#include <osre/Common/osre_common.h>
#include <osre/Common/Ids.h>
#include <osre/Common/Event.h>
#include <osre/App/AppBase.h>
#include <osre/App/Entity.h>
#include <osre/app/World.h>
#include <osre/Properties/Settings.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractPlatformEventQueue.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/App/World.h>
#include <osre/Scene/View.h>
#include <osre/Scene/TrackBall.h>
#include <osre/IO/IOService.h>
#include <osre/App/AssimpWrapper.h>
#include <osre/App/AssetDataArchive.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>

#include <src/Engine/Platform/win32/Win32Window.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace OSRE {
namespace NativeEditor {

using namespace ::OSRE::App;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Properties;
using namespace ::OSRE::Platform;
using namespace ::OSRE::Scene;

static const c8 *Tag = "EditorApplication";

static const i32 UIED_ERROR = -1;
static const i32 UIED_WORLDACCESS_ERROR = -2;

struct SceneContext {
    App::World *ActiveWorld;
    int StageIndex;
    Scene::Stage *ActiveStage;
    Scene::Node *ActiveNode;
};

struct EditorApplication::Impl {
    bool m_worldAccess;
    bool m_stageAccess;
    bool m_nodeAccess;
    Common::Ids m_ids;
    App::World *m_world;
    Scene::Stage *m_stage;
    Scene::Node *m_node;
    Scene::Node::NodePtr m_modelNode;
    RenderBackend::TransformMatrixBlock m_transformMatrix;
    Platform::PlatformInterface *m_platformInterface;
    App::Project m_project;
    Scene::TrackBall *m_trackball;
    f32 m_angle;
};

EditorApplication::EditorApplication( int argc, const char *argv[] )
: AppBase( argc, argv )
, m_impl( new Impl ) {
    // empty
}

EditorApplication::~EditorApplication() {
    delete m_impl;
    m_impl = nullptr;
}

int EditorApplication::enqueueEvent( const Event *ev, EventData *evData ) {
    if (nullptr == m_impl->m_platformInterface) {
        return 1;
    }

    AbstractPlatformEventQueue *queue = m_impl->m_platformInterface->getInstance()->getPlatformEventHandler();
    if (nullptr == queue) {
        return 1;
    }
    queue->enqueueEvent( *ev, evData );

    return 0;
}

void EditorApplication::newProject( const String &name ) {
    if ( name != m_impl->m_project.getName()) {
        m_impl->m_project.destroy();
    }

    if (!m_impl->m_project.create( name, 0, 1 ) ) {
        return;
    }

    Platform::AbstractWindow *rootWindow( getRootWindow() );
    if (nullptr != rootWindow) {
        rootWindow->setWindowsTitle( m_impl->m_project.getName() );
    }
}

int EditorApplication::openWorldAccess( const String &name ) {
    if (m_impl->m_worldAccess) {
        return UIED_WORLDACCESS_ERROR;
    }
    m_impl->m_worldAccess = true;
    if (!name.empty()) {
        World *world = AppBase::findWorld( name );
        if (nullptr == world) {
            world = AppBase::createWorld( name );
            AppBase::setActiveWorld( name );
        }

        return 0;
    }

    return -1;
}

int EditorApplication::openStageAccess( const String &name ) {
    if (!m_impl->m_worldAccess || m_impl->m_stageAccess ) {
        return -1;
    }

    if (name.empty()) {
        return -2;
    }

    Stage *stage = m_impl->m_world->setActiveStage( name );
    if (nullptr == stage) {
        return -1;
    }
    
    m_impl->m_stage = stage;
    m_impl->m_stageAccess = true;

    return -1;
}

int EditorApplication::openNodeAccess( const String &name ) {
    if (m_impl->m_nodeAccess || !m_impl->m_stageAccess) {
        return -1;
    }

    if (name.empty()) {
        return -2;
    }

    m_impl->m_node = m_impl->m_stage->findNode( name );
    if (nullptr == m_impl->m_node) {
        return -1;
    }
    m_impl->m_nodeAccess = true;

    return 0;
}

int EditorApplication::createNode( const String &name, const String &parentNode ) {
    if (!m_impl->m_stageAccess) {
        return -1;
    }

    Node *parent( nullptr );
    if (!parentNode.empty()) {
        parent = m_impl->m_stage->findNode( parentNode );
        if (nullptr == parent) {
            return -3;
        }
    }

    m_impl->m_node = m_impl->m_stage->addNode( name, parent );
    if (nullptr == m_impl->m_node) {
        return -1;
    }

    return 0;
}

int EditorApplication::closeNodeAccess() {
    if (!m_impl->m_nodeAccess) {
        return -1;
    }

    m_impl->m_node = nullptr;
    m_impl->m_nodeAccess = false;

    return 0;
}

int EditorApplication::closeStageAccess() {
    if (!m_impl->m_stageAccess) {
        return -1;
    }

    m_impl->m_stage = nullptr;
    m_impl->m_stageAccess = false;
    return -1;
}

int EditorApplication::closeWorldAccess() {
    if (!m_impl->m_worldAccess) {
        return UIED_WORLDACCESS_ERROR;
    }

    m_impl->m_world = nullptr;
    m_impl->m_worldAccess = false;
    
    return -1;
}

int EditorApplication::importAsset( const String &filename, int flags ) {
    if (filename.empty()) {
        return 1;
    }

    Ids ids;
    AssimpWrapper assimpWrapper( ids );
    String normalizedFilename;
#ifdef OSRE_WINDOWS
    IO::Uri::normalizePath( filename, '\\', normalizedFilename );
#else
    normalizedFilename = filename;
#endif
    IO::Uri modelLoc( normalizedFilename );
    if (assimpWrapper.importAsset( modelLoc, flags )) {
        Entity *entity = assimpWrapper.getEntity();
        RenderBackendService *rbSrv(getRenderBackendService());
        if (nullptr != rbSrv) {
            Platform::AbstractWindow *rootWindow(getRootWindow());
            if (nullptr == rootWindow) {
                return false;
            }
            
            if (nullptr == m_impl->m_stage) {
                m_impl->m_stage = AppBase::createStage( "ModelLoading" );
                AppBase::setActiveStage( m_impl->m_stage );
            }
            
            Scene::View *view = m_impl->m_stage->findView( "default_view" );
            if (nullptr == view) {
                view = m_impl->m_stage->addView( "default_view", nullptr );
            }
            AppBase::setActiveView(view);

            const Rect2ui &windowsRect = rootWindow->getWindowsRect();
            view->setProjectionParameters(60.f, (f32) windowsRect.m_width, (f32) windowsRect.m_height, 0.001f, 1000.f);
            view->observeBoundingBox(entity->getAABB());

            m_impl->m_stage->setRoot( entity->getNode() );
            m_impl->m_modelNode = m_impl->m_stage->getRoot();
        }

        AppBase::onUpdate();
    }

    return 0;
}

PlatformInterface *EditorApplication::getPlatformInterface() const {
    return m_impl->m_platformInterface;
}

bool EditorApplication::loadProject( const char *filelocation, int flags ) {
    if (nullptr == filelocation) {
        return false;
    }

    App::Project project;
    i32 major, minor;
    const bool result = project.load(filelocation, major, minor, flags);
    
    return result;
}

bool EditorApplication::saveProject( const char *filelocation, int flags ) {
    if (nullptr == filelocation) {
        return false;
    }

    const bool result = m_impl->m_project.save(filelocation, flags);

    return result;
}

bool EditorApplication::onCreate() {
    Properties::Settings *baseSettings(AppBase::getSettings());
    baseSettings->setBool( Settings::ChildWindow, true );
    baseSettings->setString( Properties::Settings::WindowsTitle, m_impl->m_project.getName() );
    if (!AppBase::onCreate()) {
        return false;
    }

    m_impl->m_platformInterface = Platform::PlatformInterface::getInstance();
    m_impl->m_stage = AppBase::createStage( "HelloWorld" );
    AppBase::activateStage( m_impl->m_stage->getName() );
    Stage *stage = m_impl->m_stage;

    // the scene object
    Entity *entity = new Entity( "model", m_impl->m_ids );

    // position of the entity
    Scene::Node *geoNode = stage->addNode( "geo", nullptr );

    // the meshes
    Scene::MeshBuilder meshBuilder;
    meshBuilder.allocTriangles( VertexType::ColorVertex, BufferAccessType::ReadOnly );
    RenderBackend::Mesh *mesh = meshBuilder.getMesh();
    if (nullptr != mesh) {
        m_impl->m_transformMatrix.m_model = glm::rotate( m_impl->m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
        m_impl->m_transformMatrix.update();
        getRenderBackendService()->setMatrix( "MVP", m_impl->m_transformMatrix.m_mvp );
        
        RenderComponent *comp = ( RenderComponent * )entity->getComponent( ComponentType::RenderComponentType );
        comp->addStaticMesh( mesh );
        entity->setNode( geoNode );
    }

    return true;
}

void EditorApplication::onUpdate() {
    RenderBackendService* rbSrv(getRenderBackendService());
    if (nullptr == rbSrv) {
        return;
    }

    glm::mat4 rot( 1.0 );
    m_impl->m_transformMatrix.m_model = glm::rotate( rot, m_impl->m_angle, glm::vec3( 1, 1, 0 ) );
    m_impl->m_transformMatrix.update();
    m_impl->m_angle += 0.01f;
    /*
    rbSrv->beginPass( PipelinePass::getPassNameById( RenderPassId ) );
    rbSrv->beginRenderBatch( "b1" );

    rbSrv->setMatrix( MatrixType::Model, m_transformMatrix.m_model );

    rbSrv->endRenderBatch();
    rbSrv->endPass();
    */
}

}
}
