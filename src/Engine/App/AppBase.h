/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#pragma once

#include "App/AppCommon.h"
#include "App/TAbstractCtrlBase.h"
#include "App/Stage.h"
#include "Platform/AbstractWindow.h"
#include "Platform/PlatformCommon.h"
#include "Platform/PlatformInterface.h"
#include "Common/ArgumentParser.h"
#include "Platform/KeyTypes.h"
#include "Animation/AnimatorBase.h"

#include <cppcore/Container/TQueue.h>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace Common {
    class ArgumentParser;
    class Environment;
    class Ids;
    class CommandQueue;
}

namespace Properties {
    class Settings;
}

namespace RenderBackend {
    struct TransformMatrixBlock;
    struct IRenderPath;

    class CanvasRenderer;
}

namespace App {

class CameraComponent;
class Stage;
class World;
class AppBase;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
/// @brief This class implements the keyboard event listener.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT KeyboardEventListener : public Platform::OSEventListener {
public:
    /// @brief The class constructor.
    KeyboardEventListener() :
            OSEventListener("App/KeyboardEventListener"),
            mLast(Platform::KEY_UNKNOWN) {
        clearKeyMap();
    }

    /// @brief The class destructor.
    ~KeyboardEventListener() override = default;

    /// @brief The event handler.
    /// @param osEvent  The os-specific event.
    /// @param data     The event-related data.
    void onOSEvent(const Common::Event &osEvent, const Common::EventData *data) override {
        auto keyData = (Platform::KeyboardButtonEventData *)data;
        if (osEvent == Platform::KeyboardButtonDownEvent) {
            mKeymap[keyData->m_key] = 1;
            mLast = keyData->m_key;
        } else {
            mKeymap[keyData->m_key] = 0;
            mLast = Platform::KEY_UNKNOWN;
        }
    }

    /// @brief Returns true, when the key is pressed
    /// @param key      The key to look for
    /// @return true for is pressed.
    bool isKeyPressed(Platform::Key key) const {
        return mKeymap[key] == 1;
    }

    /// @brief Will return the latest pressed key.
    /// @return The latest pressed key.
    Platform::Key getLastKey() const {
        return mLast; 
    }
    
    /// @brief Clearn the map.
    void clearKeyMap() {
        ::memset(mKeymap, 0, sizeof(char) * Platform::KEY_LAST);
    }

private:
    Platform::Key mLast;
    char mKeymap[Platform::KEY_LAST];
};

enum class WindowMode {
    Invalid,
    Windowed,
    Fullscreen,
    Count
};

enum class WindowType {
    Invalid,
    Root,
    Child,
    Count
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class provides the basic API for managing a OSRE_base application.
/// The following features are currently provided:
/// - Create all subsystems on startup
/// - Prepares the ServiceProvider
/// - Destroy all subsystems on shutdown
/// - Managing worlds and their scene-graphs for rendering.
/// - Manages all render-pipelines for the frame-rendering.
/// - Setting up a root window
/// - Managing easy user-inputs.
/// - Hooks for creating, rendering, frame-updates and destroying. Each Hook is called on<Event> 
///   like onUpdate for the update hook. So if you want to write a bundle of updates you can derive 
///   your class from AppBase, override the onUpdate method with your own updates for the scene etc
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AppBase {
public:
    /// @brief  The class constructor.
    /// @param  argc            [in] Number of arguments from command prompt.
    /// @param  argv            [in] The argument array.
    /// @param  supportedArgs   [in] A list of supported arguments, separated by a :
    /// @param  desc            [in] The description for the arguments.
    AppBase(i32 argc, const c8 *argv[], const String &supportedArgs = "api:run_dll:break_on_startup", 
        const String &desc = "The render API:The module to run:Breakpoint of startup");

    /// @brief  The class destructor, virtual.
    virtual ~AppBase();

    /// @brief  Will initialize a render window, the whole render engine will be started.
    /// @param[in] x           The left position of the render windows for windowed mode.
    /// @param[in] y           The top position of the render windows for windowed mode.
    /// @param[in] width       The width of the window.
    /// @param[in] height      The height of the render window.
    /// @param[in] title       The Windows title.
    /// @param[in] fullscreen  true for fullscreen mode, false for windowed mode.
    /// @param[in] childWindow true for child window.
    /// @param[in] renderer    The requested render mode.
    /// @return true, if the window was generated.
    virtual bool initWindow( ui32 x, ui32 y, ui32 width, ui32 height, const String &title, 
        WindowMode mode, WindowType type, RenderBackendType renderer);

    /// @brief  Creates the application.
    /// @param  settings         [in] The user-defined settings.
    /// @return true, if successful, false in case of an error.
    virtual bool create( Properties::Settings *settings = nullptr );

    /// @brief  Destroys the application.
    /// @return true, if successful, false in case of an error.
    virtual bool destroy();

    /// @brief  Updates the current frame.
    virtual void update();

    /// @brief  Will perform a resize for the root window.
    /// @param  x   [in] X-coordinate of origin.
    /// @param  y   [in] Y-coordinate of origin.
    /// @param  w   [in] The width for the window.
    /// @param  h   [in] The height for the window.
    virtual void resize(i32 x, i32 y, i32 w, i32 h);

    /// @brief  Requests a new frame.
    virtual void requestNextFrame();

    /// @brief  Handles all events.
    /// @return true, if successful, false in case of an error.
    virtual bool handleEvents();

    /// @brief  Returns the current global settings.
    /// @return The global settings.
    virtual Properties::Settings *getSettings() const;

    /// @brief Will return the current stage or nullptr if none is active.
    /// @return The current stage or nullptr.
    virtual Stage *getStage() const;

    /// @brief  Will activate the given view in the active world instance.
    /// @param  view        [in] A pointer showing to the view.
    /// @return true, if the view activation was successful, false if not.
    virtual CameraComponent *setActiveCamera(CameraComponent *view);

    /// @brief  Will requested a shutdown.
    virtual void requestShutdown();

    /// @brief  Will return true, if a shutdown is requested by the user.
    /// @return true, if a shutdown is requested.
    virtual bool shutdownRequested() const;

    /// @brief  Will return the active time or nullptr, if none is active.
    /// @return Pointer showing to the active timer.
    virtual Platform::AbstractTimer *getActiveTimer() const;

    /// @brief  Will return the Root-Surface instance.
    /// @return A pointer showing to the Root-Surface.
    virtual Platform::AbstractWindow *getRootWindow() const;

    /// @brief  Will set the windows tile of the main engine window.
    /// @param  title       [in] The new windows title.
    virtual void setWindowsTitle( const String &title );

    /// @brief  Will return the id-container of the application
    /// @return The id container.
    virtual Common::Ids *getIdContainer() const;

    /// @brief  Will create the default pipeline for rendering.
    /// @param  rbService   The renderbackend service instance.
    /// @return The default pipeline.

    /// @brief  Checks if the key was pressed in this frame.
    /// @param  key     [in] The key-code to look for.
    /// @return true if the key was pressed, false if not.
    virtual bool isKeyPressed(Platform::Key key) const;

    /// @brief  Will return the resolution of the root render surface.
    /// @param  w       [out] The width.
    /// @param  height  [out] The width.
    virtual void getResolution(ui32 &w, ui32 &h);

    /// @brief  Will return the default transform controller.
    /// @param  type    [in] The requested controller type.
    /// @param  tmb     [in] The controlled transform block.
    /// @return The transform controller or nullptr if none is there.
    virtual Animation::AnimationControllerBase *getTransformController(RenderBackend::TransformMatrixBlock &tmb);

    ///	@brief  Will return the mouse event listener,
    /// @return The mouse event listener.
    virtual MouseEventListener *getMouseEventListener() const;

    ///	@brief  Will return the keyboard listener.
    /// @return The keyboard listener.
    virtual KeyboardEventListener *getKeyboardEventListener() const;

    /// @brief
    virtual RenderBackend::CanvasRenderer *getCanvasRenderer() const;
    
 protected:
    /// @brief  The onCreate callback, override this for your own creation stuff.
    /// @return true if successful,  false if not.
    virtual bool onCreate();

    /// @brief  The onDestroy callback, override this for your own destroying stuff.
    /// @return true if successful,  false if not.
    virtual bool onDestroy();

    /// @brief  The onRener callback.
    virtual void onRender();

    /// @brief  The onUpdate callback, override this for your own update stuff.
    virtual void onUpdate();

    /// @brief  Argument parser getter.
    /// @return The argument parser.
    const Common::ArgumentParser &getArgumentParser() const;

private:
    enum class State {
        Uninited,
        Created,
        Running,
        Destroyed
    };

    State mAppState;
    i64 mLastTime;
    Common::ArgumentParser mArgParser;
    Common::Environment* mEnvironment;
    Properties::Settings *mSettings;
    Platform::PlatformInterface *mPlatformInterface;
    Platform::AbstractTimer *mTimer;
    RenderBackend::RenderBackendService *mRbService;
    cppcore::TArray<World*> mWorlds;
    Stage *mStage;
    MouseEventListener *mMouseEvListener;
    KeyboardEventListener *mKeyboardEvListener;
    Common::Ids *mIds;
    StageMode mStageMode;
    bool mShutdownRequested;
    RenderBackend::IRenderPath *mCanvasRenderer;
};

inline Stage *AppBase::getStage() const {
    return mStage;
}

inline MouseEventListener *AppBase::getMouseEventListener() const {
    return mMouseEvListener;
}

inline KeyboardEventListener *AppBase::getKeyboardEventListener() const {
    return mKeyboardEvListener;
}

inline const Common::ArgumentParser &AppBase::getArgumentParser() const {
    return mArgParser;
}

inline Common::Ids *AppBase::getIdContainer() const {
    return mIds;
}

//-------------------------------------------------------------------------------------------------
///	@brief  Shortcut to get a OSRE-main function.
/// To use this you need to derive your application class from the ::OSRE::App::AppBase class and
/// put this into your main source file:
///
/// @code
///
/// class MyApp : public ::OSRE::App::AppBase { ... };
///
/// OSRE_MAIN(MyApp);
///
/// @endcode
//-------------------------------------------------------------------------------------------------
#define OSRE_MAIN( APPNAME )          \
int main( int argc, char *argv[] )  { \
    APPNAME myApp( argc, argv );      \
    if ( !myApp.create() ) {          \
        return 1;                     \
    }                                 \
    while ( myApp.handleEvents() ) {  \
        myApp.update();               \
        myApp.requestNextFrame();     \
    }                                 \
    myApp.destroy();                  \
                                      \
    return 0;                         \
}

} // Namespace App
} // Namespace OSRE
