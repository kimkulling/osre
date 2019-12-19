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
#include <osre/Properties/Settings.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractPlatformEventQueue.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/World.h>
#include <osre/Scene/View.h>
#include <osre/Scene/TrackBall.h>
#include <osre/IO/IOService.h>
#include <osre/Assets/AssimpWrapper.h>
#include <osre/Assets/AssetDataArchive.h>
#include <osre/Assets/Model.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>

#include <src/Engine/Platform/win32/Win32Window.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace OSRE {
namespace NativeEditor {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Properties;
using namespace ::OSRE::Platform;
using namespace ::OSRE::Scene;
using namespace ::OSRE::Assets;

static const c8 *Tag = "EditorApplication";

static const i32 UIED_ERROR = -1;
static const i32 UIED_WORLDACCESS_ERROR = -2;

EditorApplication::EditorApplication( int argc, char *argv[] )
: AppBase( argc, argv )
, m_worldAccess( false )
, m_stageAccess( false )
, m_nodeAccess( false )
, m_world( nullptr )
, m_stage( nullptr )
, m_modelNode()
, m_transformMatrix()
, m_platformInterface( nullptr )
, m_project()
, m_trackball( nullptr )
, m_angle( 0.01f){
    // empty
}

EditorApplication::~EditorApplication() {
    // empty
}

int EditorApplication::enqueueEvent( const Event *ev, EventData *evData ) {
    if (nullptr == m_platformInterface) {
        return 1;
    }

    AbstractPlatformEventQueue *queue = m_platformInterface->getInstance()->getPlatformEventHandler();
    if (nullptr == queue) {
        return 1;
    }
    queue->enqueueEvent( *ev, evData );

    return 0;
}

void EditorApplication::newProject( const String &name ) {
    if ( name != m_project.getName()) {
        m_project.destroy();
    }

    if (!m_project.create( name, 0, 1 ) ) {
        return;
    }

    Platform::AbstractWindow *rootWindow( getRootWindow() );
    if (nullptr != rootWindow) {
        rootWindow->setWindowsTitle( m_project.getName() );
    }
}

int EditorApplication::openWorldAccess( const String &name ) {
    if (m_worldAccess) {
        return UIED_WORLDACCESS_ERROR;
    }
    m_worldAccess = true;
    if (!name.empty()) {
        Scene::World *world = AppBase::findWorld( name );
        if (nullptr == world) {
            world = AppBase::createWorld( name );
            AppBase::setActiveWorld();
        }

        return 0;
    }

    return -1;
}

int EditorApplication::openStageAccess( const String &name ) {
    if (!m_worldAccess || m_stageAccess ) {
        return -1;
    }

    m_stageAccess = true;

    return -1;
}

int EditorApplication::openNodeAccess( const String &name ) {
    return -1;
}

int EditorApplication::createNode( const String &name, const String &parentNode ) {

}

int EditorApplication::closeNodeAccess() {
    return -1;
}

int EditorApplication::closeStageAccess() {
    return -1;
}

int EditorApplication::closeWorldAccess() {
    if (!m_worldAccess) {
        return UIED_WORLDACCESS_ERROR;
    }
    m_worldAccess = false;
    
    return -1;
}

int EditorApplication::importAsset( const String &filename, int flags ) {
    if (filename.empty()) {
        return 1;
    }

    Ids ids;
    Assets::AssimpWrapper assimpWrapper( ids );
    String normalizedFilename;
#ifdef OSRE_WINDOWS
    IO::Uri::normalizePath( filename, '\\', normalizedFilename );
#else
    normalizedFilename = filename;
#endif
    IO::Uri modelLoc( normalizedFilename );
    if (assimpWrapper.importAsset( modelLoc, flags )) {
        Model *model = assimpWrapper.getModel();
        RenderBackendService *rbSrv(getRenderBackendService());
        if (nullptr != rbSrv) {
            Platform::AbstractWindow *rootWindow(getRootWindow());
            if (nullptr == rootWindow) {
                return false;
            }
            
            if (nullptr == m_stage) {
                m_stage = AppBase::createStage( "ModelLoading" );
                AppBase::setActiveStage( m_stage );
            }
            
            Scene::View *view = m_stage->findView( "default_view" );
            if (nullptr == view) {
                view = m_stage->addView( "default_view", nullptr );
            }
            AppBase::setActiveView(view);

            const Rect2ui &windowsRect = rootWindow->getWindowsRect();
            view->setProjectionParameters(60.f, (f32) windowsRect.m_width, (f32) windowsRect.m_height, 0.001f, 1000.f);
            view->observeBoundingBox(model->getAABB());

            m_stage->setRoot(model->getRootNode());
            m_modelNode = m_stage->getRoot();
        }

        AppBase::onUpdate();
    }

    return 0;
}

PlatformInterface *EditorApplication::getPlatformInterface() const {
    return m_platformInterface;
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

    const bool result = m_project.save(filelocation, flags);

    return result;
}

bool EditorApplication::onCreate() {
    Properties::Settings *baseSettings(AppBase::getSettings());
    baseSettings->setBool( Settings::ChildWindow, true );
    baseSettings->setString( Properties::Settings::WindowsTitle, m_project.getName() );
    if (!AppBase::onCreate()) {
        return false;
    }

    m_platformInterface = Platform::PlatformInterface::getInstance();

    m_stage = AppBase::createStage( "HelloWorld" );
    AppBase::activateStage( m_stage->getName() );

    Scene::Node *geoNode = m_stage->addNode( "geo", nullptr );
    Scene::MeshBuilder meshBuilder;
    meshBuilder.allocTriangles( VertexType::ColorVertex, BufferAccessType::ReadOnly );
    RenderBackend::Mesh *mesh = meshBuilder.getMesh();
    if (nullptr != mesh) {
        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
        m_transformMatrix.update();
        getRenderBackendService()->setMatrix( "MVP", m_transformMatrix.m_mvp );
        geoNode->addMesh( mesh );
    }

    return true;
}

void EditorApplication::onUpdate() {
    RenderBackendService* rbSrv(getRenderBackendService());
    if (nullptr == rbSrv) {
        return;
    }

    glm::mat4 rot( 1.0 );
    m_transformMatrix.m_model = glm::rotate( rot, m_angle, glm::vec3( 1, 1, 0 ) );
    m_transformMatrix.update();
    m_angle += 0.01f;
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
