## The User-Interface-Demo

This example shows how to use the simple engine-intern User-Interface-Framework.

So lets start with the basic application:
```cpp
#include <osre/App/AppBase.h>
#include <osre/Properties/Settings.h>
#include <osre/Common/Logger.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/UI/Screen.h>
#include <osre/UI/ButtonBase.h>
#include <osre/UI/Panel.h>
#include <osre/Platform/PlatformOperations.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace ::OSRE;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::UI;
using namespace ::OSRE::Platform;

// To identify local log entries
static const c8 *Tag = "ModelLoadingApp";

// The example application, will create the render environment and render a simple triangle onto it
class UIDemoApp : public App::AppBase {
    Screen *m_screen;
    TransformMatrixBlock m_transformMatrix;

public:
    UIDemoApp( int argc, char *argv[] )
    : AppBase( argc, argv )
    , m_screen( nullptr )
    , m_transformMatrix() {
        // empty
    }

    virtual ~UIDemoApp() {
        // empty
    }
    void openFileCallback( ui32 id, void *data ) {
        IO::Uri loc;
        PlatformOperations::getFileOpenDialog( "All\0 *.*\0", loc );
    }

    void quitCallback( ui32 id, void *data ) {
        AppBase::requestShutdown();
    }

protected:
    bool onCreate( Properties::Settings *settings = nullptr ) override {
        Properties::Settings *baseSettings( AppBase::getSettings() );
        if ( nullptr == baseSettings ) {
            return false;
        }

        baseSettings->setString( Properties::Settings::WindowsTitle, "Demo UI!" );
        if ( !AppBase::onCreate( baseSettings ) ) {
            return false;
        }

#ifdef OSRE_WINDOWS
        Assets::AssetRegistry::registerAssetPath( "assets", "../../assets" );
#else
        Assets::AssetRegistry::registerAssetPath( "assets", "../assets" );
#endif

        m_screen = AppBase::createScreen( "HelloWorld" );

        Panel *panel = new Panel( "panel", m_screen );
        panel->setRect( 10, 10, 500, 500 );
        ButtonBase *btnClick = new ButtonBase( "click", panel );
        btnClick->registerCallback( ButtonBase::ButtonPressed, UIFunctor::Make(this, &UIDemoApp::openFileCallback ));
        btnClick->setRect( 20, 20, 100, 20 );
        ButtonBase *btnQuit  = new ButtonBase( "quit", panel );
        btnQuit->setRect( 400, 20, 100, 20 );
        btnQuit->registerCallback( ButtonBase::ButtonPressed, UIFunctor::Make( this, &UIDemoApp::quitCallback ) );
        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
        m_transformMatrix.update();
        AppBase::getRenderBackendService()->setMatrix( "MVP", m_transformMatrix.m_mvp );

        return true;
    }
};

OSRE_MAIN( UIDemoApp )
```
At first you have to create your application window with your render-window. 
To be able to render any UI-Elements you have to create a screen first. A screen represents a 2D-rect
on you screen.

The next step is to create your dialog: defining a panel, add two buttons and assign callbacks to them
In this case the button on the left will open a FileOpen-dialog and the second button will destroy your 
application.

# Todo
- Optimize UI-elements styling
- Add button texts
- Add button images
- Add onClicked animation
- Implement OpenFile-dialog for other platforms
