/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "Platform/AbstractPlatformEventQueue.h"
#include "Common/Event.h"

#include <map>

// Forward declarations ---------------------------------------------------------------------------
struct SDL_Window;

namespace OSRE {
namespace Platform {

struct AbstractSDL2InputUpdate;

class AbstractWindow;
class SDL2Surface;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class SDL2EventHandler final : public AbstractPlatformEventQueue {
public:
    using EventArray = cppcore::TArray<const Common::Event*>;

    /// @brief The class constructor with the active window,
    /// @param window   The active window.
    SDL2EventHandler( AbstractWindow *window );

    /// @brief The class destructor.
    ~SDL2EventHandler() override;

    /// @brief The windows update function.
    /// @return false for close requested,
    bool update() override;

    /// @brief WIll register an event listener.
    /// @param events   The list of event to listen to.
    /// @param listener The listener.
    void registerEventListener( const Common::EventPtrArray &events, OSEventListener *listener ) override;
    
    /// @brief Will unregister a listener to a given array of events,
    /// @param events   The events to unregister from.
    /// @param listener The listener.
    void unregisterEventListener( const Common::EventPtrArray &events, OSEventListener *listener ) override;
    
    /// @brief All event listerner will be unregistered from an event array.
    /// @param events   The evebt array.
    void unregisterAllEventHandler(const Common::EventPtrArray &events) override;
    
    /// @brief Will register a menu functor for a menu id.
    /// @param id   The menu id.
    /// @param func The functor for handling the menu entry.
    void registerMenuCommand(ui32 id, MenuFunctor func) override;
    
    /// @brief Will unregister all menu handler entries.
    void unregisterAllMenuCommands() override;

    /// @brief Set the new polling state.
    /// @param enabled  The polling state, true for polling.
    void enablePolling(bool enabled) override;

    /// @brief Returns th e polling state.
    /// @return The polling state, true for polling.
    bool isPolling() const override;

protected:
    void onQuit() override {}

private:
    static std::map<SDL_Window*, SDL2EventHandler*> s_windowsServerMap;
    bool m_isPolling;
    bool mShutdownRequested;
    AbstractSDL2InputUpdate *m_inputUpdate;
    SDL2Surface *mWindow;
    Common::EventTriggerer *m_eventTriggerer;
};

} // Namespace Platform
} // Namespace OSRE
