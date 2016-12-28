/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/UI/Screen.h>
#include <osre/UI/ButtonBase.h>
#include <osre/UI/Panel.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ::OSRE;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::UI;

// To identify local log entries
static const String Tag = "ModelLoadingApp";

// The example application, will create the render environment and render a simple triangle onto it
class UIDemoApp : public App::AppBase {
    Screen *m_screen;

public:
    UIDemoApp( int argc, char *argv[] )
    : AppBase( argc, argv )
    , m_screen( nullptr ) {
        // empty
    }

    virtual ~UIDemoApp() {
        // empty
    }

protected:
    virtual bool onCreate( Properties::Settings *settings = nullptr ) {
        Properties::Settings *baseSettings( AppBase::getSettings() );
        if ( nullptr == baseSettings ) {
            return false;
        }

        baseSettings->setString( Properties::Settings::WindowsTitle, "Demo UI!" );
        if ( !AppBase::onCreate( baseSettings ) ) {
            return false;
        }

#ifdef OSRE_WINDOWS
        Assets::AssetRegistry::registerAssetPath( "assets", "../../media" );
#else
        Assets::AssetRegistry::registerAssetPath( "assets", "../media" );
#endif

        m_screen = AppBase::createScreen( "HelloWorld" );

        Panel *panel = new Panel( "panel", m_screen );
        ButtonBase *btnClick = new ButtonBase( "click", panel );
        ButtonBase *btnQuit  = new ButtonBase( "quit", panel );

        return true;
    }
};

OSRE_MAIN( UIDemoApp )
