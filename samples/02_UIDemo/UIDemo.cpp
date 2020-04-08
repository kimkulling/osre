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
#include <osre/App/AppBase.h>
#include <osre/Properties/Settings.h>
#include <osre/Common/Logger.h>
#include <osre/App/AssetRegistry.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/UI/Canvas.h>
#include <osre/UI/ButtonBase.h>
#include <osre/UI/TextBase.h>
#include <osre/UI/Panel.h>
#include <osre/Platform/PlatformOperations.h>

using namespace ::OSRE;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::UI;
using namespace ::OSRE::Platform;

// To identify local log entries
static const c8 Tag[] = "ModelLoadingApp";

// The example application, will create the render environment and render a simple triangle onto it
class UIDemoApp : public App::AppBase {
    Canvas               *m_canvas;
    TransformMatrixBlock  m_transformMatrix;

public:
    UIDemoApp( int argc, char *argv[] )
    : AppBase( argc, (const char** ) argv )
    , m_canvas( nullptr )
    , m_transformMatrix() {
        // empty
    }

    virtual ~UIDemoApp() {
        // empty
    }

    void openFileCallback( ui32, void* ) {
        IO::Uri loc;
        PlatformOperations::getFileOpenDialog( "All\0 *.*\0", loc );
    }

    void quitCallback( ui32, void * ) {
        AppBase::requestShutdown();
    }

protected:
    bool onCreate() override {
        Properties::Settings *baseSettings( AppBase::getSettings() );
        if ( nullptr == baseSettings ) {
            return false;
        }

        baseSettings->setString( Properties::Settings::WindowsTitle, "Demo UI!" );
        if ( !AppBase::onCreate() ) {
            return false;
        }

#ifdef OSRE_WINDOWS
        App::AssetRegistry::registerAssetPath( "assets", "../../media" );
#else
        App::AssetRegistry::registerAssetPath( "assets", "../media" );
#endif

        m_canvas = AppBase::createScreen( "UiDemo" );

        Panel *panel = new Panel( "panel", m_canvas );
        panel->setRect( 10, 10, 500, 500 );
        ButtonBase *btnOpenFile = new ButtonBase( "Open file", panel);
        btnOpenFile->registerCallback(WidgetState::Pressed, UiFunctor::Make(this, &UIDemoApp::openFileCallback));
        btnOpenFile->setRect( 20, 20, 100, 20 );
        btnOpenFile->setLabel("Open file");

        ButtonBase *btnQuit  = new ButtonBase( "Quit", panel );
        btnQuit->setRect( 400, 20, 100, 20 );
        btnQuit->registerCallback(WidgetState::Pressed, UiFunctor::Make( this, &UIDemoApp::quitCallback ) );
        
        //TextBase *tb = new TextBase( "test", panel);
        //tb->setLabel( "Test" );
        //tb->setRect( 80, 20, 400, 60 );

        return true;
    }
};

OSRE_MAIN( UIDemoApp )
