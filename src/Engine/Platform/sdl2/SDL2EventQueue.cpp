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
#include <src/Engine/Platform/sdl2/SDL2EventQueue.h>
#include <osre/Common/EventTriggerer.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Common/Logger.h>
#include <osre/Platform/PlatformInterface.h>

#include "SDL2Surface.h"

#include <SDL.h>

namespace OSRE {
namespace Platform {

using namespace ::OSRE;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

using namespace ::CPPCore;

static const String Tag = "SDL2EventHandler";

struct AbstractSDL2InputUpdate {
    //  The virtual destructor.
    virtual ~AbstractSDL2InputUpdate( ){
        // empty
    }

    //  Will perform the update.
    virtual bool update( SDL_Event *ev ) = 0;
};

//-------------------------------------------------------------------------------------------------
//  Implements a get update, waits for the next event.
//-------------------------------------------------------------------------------------------------
struct SDL2GetInputUpdate : public AbstractSDL2InputUpdate {
    //  The default constructor.
    SDL2GetInputUpdate() {
        // empty
    }

    //  The destructor.
    ~SDL2GetInputUpdate() {
        // empty
    }

    //  Update implemented as a wait operation, will get the next upcoming event.
    bool update( SDL_Event *ev ) {
        const i32 ret = SDL_WaitEvent( ev );
        if( 0 == ret ) {
            return false;
        }

        return false;
    }
};

//-------------------------------------------------------------------------------------------------
//  Implements a peek update, polls for the next event.
//-------------------------------------------------------------------------------------------------
struct SDL2PeekInputUpdate : public AbstractSDL2InputUpdate {
    //  The default constructor.
    SDL2PeekInputUpdate() {
        // empty
    }

    //  The destructor.
    ~SDL2PeekInputUpdate() {
        // empty
    }

    //  Update implemented as a poll operation, will check for a new event.
    bool update( SDL_Event *ev ) {
        const i32 ret = ::SDL_PollEvent( ev );
        if( 0 == ret ) {
            return false;
        }
        return true;
    }
};

std::map<SDL_Window*, SDL2EventHandler*> SDL2EventHandler::s_windowsServerMap;

SDL2EventHandler::SDL2EventHandler( AbstractSurface *window )
: AbstractPlatformEventQueue()
, m_isPolling( false )
, m_shutdownRequested( false )
, m_inputUpdate( nullptr )
, m_window( nullptr )
, m_eventTriggerer( nullptr ) {
    OSRE_ASSERT( nullptr != window );
    m_window = ( SDL2Surface* ) window;
    OSRE_ASSERT( nullptr != m_window );

    m_inputUpdate = new SDL2GetInputUpdate;
    m_eventTriggerer = new EventTriggerer;
    m_eventTriggerer->addTriggerableEvent( KeyboardButtonDownEvent );
    m_eventTriggerer->addTriggerableEvent( KeyboardButtonUpEvent );
    m_eventTriggerer->addTriggerableEvent( MouseButtonDownEvent );
    m_eventTriggerer->addTriggerableEvent( MouseButtonUpEvent );
    m_eventTriggerer->addTriggerableEvent( WindowsResizeEvent );
    m_eventTriggerer->addTriggerableEvent( MouseMoveEvent );
    m_eventTriggerer->addTriggerableEvent( QuitEvent );
    m_eventTriggerer->addTriggerableEvent( AppFocusEvent );

    if( 0 == SDL_WasInit( SDL_INIT_EVERYTHING ) ) {
        SDL_Init( SDL_INIT_EVERYTHING );
    }

    if( 0 == SDL_WasInit( SDL_INIT_EVENTS ) ) {
        SDL_InitSubSystem( SDL_INIT_EVENTS );
    }

    if( 0 == SDL_WasInit( SDL_INIT_JOYSTICK ) ) {
        SDL_InitSubSystem( SDL_INIT_JOYSTICK );
    }

    SDL_JoystickEventState( SDL_ENABLE );
}

SDL2EventHandler::~SDL2EventHandler( ) {
    delete m_eventTriggerer;
    m_eventTriggerer = nullptr;

    delete m_inputUpdate;
    m_inputUpdate = nullptr;
}

bool SDL2EventHandler::update() {
    EventDataList *activeEventQueue( getActiveEventDataList() );
    OSRE_ASSERT(nullptr != activeEventQueue);

    SDL_Event ev;
    if( !m_shutdownRequested && m_inputUpdate->update( &ev ) ) {
        const Uint32 windowID = SDL_GetWindowID( m_window->getSDLSurface() );

        switch( ev.type ){
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                KeyboardButtonEventData *data = new KeyboardButtonEventData( SDL_KEYDOWN == ev.type, m_eventTriggerer );
                data->m_Key = ( Key ) ev.key.keysym.sym;
                activeEventQueue->addBack( data );
            }
            break;

            case SDL_MOUSEMOTION: {
                MouseMoveEventData *data = new MouseMoveEventData( m_eventTriggerer );
                int x, y;
                SDL_GetMouseState( &x, &y );
                data->m_AbsX = x;
                data->m_AbsY = y;
                activeEventQueue->addBack( data );
            }
            break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                MouseButtonEventData *data = new MouseButtonEventData( ev.type == SDL_MOUSEBUTTONDOWN, m_eventTriggerer );
                activeEventQueue->addBack( data );
            }
            break;

            case SDL_QUIT:  {
                m_shutdownRequested = true;
            }
            break;

            case SDL_WINDOWEVENT: {
                if ( ev.window.windowID == windowID ) {
                    switch ( ev.window.event ) {
                    case SDL_WINDOWEVENT_MOVED: {
                        ui32 x = ( ui32 )ev.window.data1;
                        ui32 y = ( ui32 )ev.window.data1;
                        getRenderBackendService()->resize( x, y, m_window->getProperties()->m_width, m_window->getProperties()->m_width );
                    }
                    break;

                    case SDL_WINDOWEVENT_SIZE_CHANGED: {
                        ui32 w = ev.window.data1;
                        ui32 h = ev.window.data2;
                        getRenderBackendService()->resize( m_window->getProperties()->m_x, m_window->getProperties()->m_y, w, h );
                    }
                    break;

                    case SDL_WINDOWEVENT_RESIZED: {
                        WindowsResizeEventData *data = new WindowsResizeEventData( m_eventTriggerer );
                        data->m_w = ( ui32 )ev.window.data1;
                        data->m_h = ( ui32 )ev.window.data1;
                        activeEventQueue->addBack( data );
                    }
                    break;
                    }
                }
            }
            break;

            default:
                break;
        }

        processEvents( m_eventTriggerer );
    }
    
    return !m_shutdownRequested;
}

void SDL2EventHandler::registerEventListener( const EventArray &events, OSEventListener *listener ) {
    if ( nullptr == listener ) {
        osre_error( Tag, "Pointer to listener is nullptr." );
        return;
    }

    m_eventTriggerer->addEventListener( events, EventFunctor::Make( listener, &OSEventListener::onOSEvent ) );
}

void SDL2EventHandler::unregisterEventListener( const EventArray &events, OSEventListener *listener ) {
    if (nullptr == listener) {
        osre_error(Tag, "Pointer to listener is nullptr.");
        return;
    }

    m_eventTriggerer->removeEventListener( events, EventFunctor::Make( listener, &OSEventListener::onOSEvent ) );
}
        
void SDL2EventHandler::enablePolling( bool enabled ) {
    if( enabled == m_isPolling ) {
        return;
    }

    delete m_inputUpdate;
    if( enabled ) {
        m_inputUpdate = new SDL2PeekInputUpdate;
    } else {
        m_inputUpdate = new SDL2GetInputUpdate;
    }
    m_isPolling = enabled;
}

bool SDL2EventHandler::isPolling() const {
    return m_isPolling;
}

} // Namespace Platform
} // Namespace OSRE
