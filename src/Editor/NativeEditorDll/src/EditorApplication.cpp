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
#include <osre/IO/IOService.h>
#include <osre/Assets/AssimpWrapper.h>
#include <osre/Assets/AssetDataArchive.h>
#include <osre/Assets/Model.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>

#include <src/Engine/Platform/win32/Win32Window.h>

namespace OSRE {
namespace NativeEditor {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Properties;
using namespace ::OSRE::Platform;
using namespace ::OSRE::Scene;
using namespace ::OSRE::Assets;


static const String Tag = "EditorApplication";

EditorApplication::EditorApplication( int argc, char *argv[] )
: AppBase( argc, argv )
, m_world( nullptr )
, m_stage( nullptr )
, m_modelNode()
, m_transformMatrix()
, m_platformInterface( nullptr )
, m_projectName( "untitled" ) {
    // empty
}

EditorApplication::~EditorApplication() {
    // empty
}

int EditorApplication::enqueueEvent( const Event *ev, EventData *evData ) {
    AbstractPlatformEventQueue *queue = m_platformInterface->getInstance()->getPlatformEventHandler();
    queue->enqueueEvent( *ev, evData );
    return 0;
}

void EditorApplication::newProject( const String &name ) {
    if (name != m_projectName) {
        m_projectName = name;
    }
    Platform::AbstractWindow *rootWindow( getRootWindow() );
    if (nullptr != rootWindow) {
        rootWindow->setWindowsTitle( m_projectName );
    }
}

World *EditorApplication::getWorld() const {
    return m_world;
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
        Assets::Model *model = assimpWrapper.getModel();
        Stage *stage = AppBase::createStage( model->getRootNode()->getName() );
        stage->setRoot( model->getRootNode() );
        m_world->addStage( stage );
        m_world->setActiveStage( stage );
        Collision::TAABB<f32> aabb = model->getAABB();
        const f32 diam = aabb.getDiameter();
        const Vec3f center = aabb.getCenter();

        Assets::Model::MeshArray meshArray = model->getMeshArray();
        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
        m_transformMatrix.update();
        RenderBackendService *rbSrv( getRenderBackendService() );
        if (nullptr != rbSrv) {
            Platform::AbstractWindow *rootWindow( getRootWindow() );
            if (nullptr == rootWindow) {
                return false;
            }
            const i32 w = rootWindow->getProperties()->m_width;
            const i32 h = rootWindow->getProperties()->m_height;
            const f32 aspect = static_cast<f32>(w) / static_cast<f32>(h);
            const f32 zNear = 0.0001f;
            const f32 zFar = 1000.f;

            glm::vec3 eye( diam, 0, 0 ), up( 0, 0, 1 );
            glm::vec3 c( center.getX(), center.getY(), center.getZ() );

            m_transformMatrix.m_model = glm::mat4( 1.0f );
            m_transformMatrix.m_projection = glm::perspective( glm::radians( 60.0f ), aspect, zNear, zFar );
            m_transformMatrix.m_view = glm::lookAt( eye, c, up );
            rbSrv->setMatrix( MatrixType::View, m_transformMatrix.m_view );
            rbSrv->setMatrix( MatrixType::Projection, m_transformMatrix.m_projection );

        }
        m_stage->setRoot( model->getRootNode() );

        update();
        requestNextFrame();
    }

    return 0;
}

PlatformInterface *EditorApplication::getPlatformInterface() const {
    return m_platformInterface;
}

bool EditorApplication::loadWorld( const char *filelocation, int flags ) {
    if (nullptr == filelocation) {
        return false;
    }

    return true;
}

bool EditorApplication::saveWorld( const char *filelocation, int flags ) {
    if (nullptr == filelocation) {
        return false;
    }
    IO::Uri uri( filelocation );
    AssetDataArchive archive( 0, 1 );
    return archive.save( m_world, uri );
}

bool EditorApplication::onCreate( Properties::Settings *settings ) {
    Properties::Settings *baseSettings( settings );
    if (baseSettings == nullptr) {
        baseSettings = AppBase::getSettings();
        if (nullptr == baseSettings) {
            return false;
        }
    }
    baseSettings->setBool( Settings::ChildWindow, true );
    baseSettings->setString( Properties::Settings::WindowsTitle, m_projectName );
    if (!AppBase::onCreate( baseSettings )) {
        return false;
    }

    m_world = new World( "HelloWorld", RenderMode::Render3D );
    m_stage = AppBase::createStage( "HelloWorld" );
    m_world->addStage( m_stage );

    m_platformInterface = Platform::PlatformInterface::getInstance();
    AppBase::activateStage( m_stage->getName() );

    return true;
}

}
}
