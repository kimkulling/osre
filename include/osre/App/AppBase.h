/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>
#include <osre/Common/ArgumentParser.h>

namespace OSRE {

// Forward declarations
namespace Common {
    class ArgumentParser;
}

namespace Scene {
    class View;
    class Stage;
    class World;
}

namespace Platform {
    class AbstractTimer;
    class PlatformInterface;
}

namespace RenderBackend {
    class RenderBackendService;
}

namespace Properties {
    class Settings;
}

namespace App {
        
//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the basics for a new OSRE-application. It helps you to 
/// - create all subsystems on startup
/// - destroy all subsystems on shutdown
/// - managing stages and views for a running application.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AppBase {
public:
    /// @brief  The class constructor.
    /// @param  argc            [in] Number of arguments from command prompt.
    /// @param  argv            [in] The argument array.
    /// @param  supportedArgs   [in] A list of supported arguments, separated by a :
    /// @param  desc            [in] The description for the arguments.
    AppBase( i32 argc, c8 *argv[], const String &supportedArgs = "api", const String &desc = "The render API" );

    /// @brief  The class destructor, virtual.
    virtual ~AppBase();

    /// @brief  Creates the application.
    /// @param  settings         [in] The user-defined settings.
    /// @return true, if successful, false in case of an error.
    virtual bool create( Properties::Settings *settings = nullptr );

    /// @brief  Destroys the application.
    /// @return true, if successful, false in case of an error.
    virtual bool destroy();

    /// @brief  Updates the current frame.
    virtual void update();

    /// @brief  Requests a new frame.
    virtual void requestNextFrame();

    /// @brief  Handles all events.
    /// @return true, if successful, false in case of an error.
    virtual bool handleEvents();

    /// @brief  Returns the current global settings.
    /// @return The global settings.
    virtual Properties::Settings *getSettings() const;

    /// @brief  Will create a new stage and set it as the active one.
    /// @param  name        [in] The name for the stage.
    /// @return The new stage instance or nullptr if the stage exists already.
    virtual Scene::Stage *createStage( const String &name );

    /// @brief  Activates a stage.
    /// @param  name        [in] The name of the stage.
    /// @return true if successful,  false if not.
    virtual bool activateStage( const String &name );

    virtual void requestShutdown();
    virtual bool shutdownRequested() const;
protected:
    /// @brief  The onCreate callback, override this for your own creation stuff.
    /// @return true if successful,  false if not.
    virtual bool onCreate( Properties::Settings *config );

    /// @brief  The onDestroy callback, override this for your own destroying stuff.
    /// @return true if successful,  false if not.
    virtual bool onDestroy();

    /// @brief  The onUpdate callback, override this for your own update stuff.
    virtual void onUpdate( d32 timetick );

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

    State m_state;
    d32 m_timediff;

    Common::ArgumentParser m_argParser;
    Properties::Settings *m_settings;
    Platform::PlatformInterface *m_platformInterface;
    Platform::AbstractTimer *m_timer;
    RenderBackend::RenderBackendService *m_rbService;
    Scene::World *m_world;
    bool m_shutdownRequested;
};

///	@brief  Shortcut to get a OSRE-main function.
/// 
#define OSRE_MAIN( APPNAME )          \
int main( int argc, char *argv[] )  { \
    APPNAME myApp( argc, argv );      \
    if ( !myApp.create() ) {          \
        return 1;                     \
    }                                 \
    while ( myApp.handleEvents() && !myApp.shutdownRequested() ) {  \
        myApp.update();               \
        myApp.requestNextFrame();     \
    }                                 \
    myApp.destroy();                  \
                                      \
    return 0;                         \
}


} // Namespace App
} // Namespace OSRE
