/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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

static const c8 *Tag = "EditorApplication";

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
    if ( name != m_projectName ) {
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
        Model *model = assimpWrapper.getModel();
        RenderBackendService *rbSrv(getRenderBackendService());
        if (nullptr != rbSrv) {
            Platform::AbstractWindow *rootWindow(getRootWindow());
            if (nullptr == rootWindow) {
                return false;
            }

            m_stage = AppBase::createStage("ModelLoading");
            AppBase::setActiveStage(m_stage);
            Scene::View *view = m_stage->addView("default_view", nullptr);
            AppBase::setActiveView(view);

            const Rect2ui &windowsRect = rootWindow->getWindowsRect();
            view->setProjectionParameters(60.f, windowsRect.m_width, windowsRect.m_height, 0.0001f, 1000.f);
            view->observeBoundingBox(model->getAABB());

            m_stage->setRoot(model->getRootNode());
            m_modelNode = m_stage->getRoot();
        }

        AppBase::onUpdate();
        //requestNextFrame();
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

    return true;
}

bool EditorApplication::saveProject( const char *filelocation, int flags ) {
    if (nullptr == filelocation) {
        return false;
    }
    IO::Uri uri( filelocation );
    AssetDataArchive archive( 0, 1 );
    return archive.save( m_world, uri );
}

bool EditorApplication::onCreate() {
    Properties::Settings *baseSettings(AppBase::getSettings());
    baseSettings->setBool( Settings::ChildWindow, true );
    baseSettings->setString( Properties::Settings::WindowsTitle, m_projectName );
    if (!AppBase::onCreate()) {
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
