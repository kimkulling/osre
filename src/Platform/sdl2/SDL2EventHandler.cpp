/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-2015, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions
and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
and the following disclaimer in the documentation and/or other materials provided with the
distribution.

* Neither the name of the ZFXCE2 team, nor the names of its contributors may be used to endorse or
promote products derived from this software without specific prior written permission of the
ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
#include <Code/Infrastructure/Platform/sdl2/SDL2EventHandler.h>
#include <osre2/Infrastructure/Common/EventTriggerer.h>
#include <osre2/Infrastructure/Platform/PlatformInterface.h>

#include <SDL.h>

namespace ZFXCE2 {
namespace Platform {

using namespace ZFXCE2::Common;

//-------------------------------------------------------------------------------------------------
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

    //  Update implemented as a poll operation, will check for a new exent.
    bool update( SDL_Event *ev ) {
        const i32 ret = ::SDL_PollEvent( ev );
        if( 0 == ret ) {
            return false;
        }
        return true;
    }
};

std::map<SDL_Window*, SDL2EventHandler*> SDL2EventHandler::s_windowsServerMap;

//-------------------------------------------------------------------------------------------------
SDL2EventHandler::SDL2EventHandler()
: AbstractPlatformEventHandler()
, m_isPolling( false )
, m_shutdownRequested( false )
, m_pInputUpdate( nullptr )
, m_pEventTriggerer( nullptr ) {
    m_pInputUpdate = new SDL2GetInputUpdate;
    m_pEventTriggerer = new EventTriggerer;
    m_pEventTriggerer->addTriggerableEvent( KeyboardButtonDownEvent );
    m_pEventTriggerer->addTriggerableEvent( KeyboardButtonUpEvent );
    m_pEventTriggerer->addTriggerableEvent( MouseButtonDownEvent );
    m_pEventTriggerer->addTriggerableEvent( MouseButtonUpEvent );
    m_pEventTriggerer->addTriggerableEvent( MouseMoveEvent );
    m_pEventTriggerer->addTriggerableEvent( QuitEvent );
    m_pEventTriggerer->addTriggerableEvent( AppFocusEvent );

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

//-------------------------------------------------------------------------------------------------
SDL2EventHandler::~SDL2EventHandler( ) {
    delete m_pEventTriggerer;
    m_pEventTriggerer = nullptr;

    delete m_pInputUpdate;
    m_pInputUpdate = nullptr;
}

//-------------------------------------------------------------------------------------------------
bool SDL2EventHandler::onEvent( const Event &event, const EventData *pEventData ){
    EventDataList *pActiveEventQueue( getActiveEventDataList() );
    SDL_Event ev;
    if( !m_shutdownRequested && m_pInputUpdate->update( &ev ) ) {
        switch( ev.type ){
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                KeyboardButtonEventData *data = new KeyboardButtonEventData( SDL_KEYDOWN == ev.type, m_pEventTriggerer );
                data->m_Key = ( Key ) ev.key.keysym.sym;
                pActiveEventQueue->addBack( data );
            }
            break;

            case SDL_MOUSEMOTION:
            {
            }
            break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
            }
            break;


            case SDL_QUIT: {
                m_shutdownRequested = true;
            }
            break;

            //case SDL_VIDEORESIZE:
            //case SDL_VIDEOEXPOSE:
            //case SDL_SYSWMEVENT:
            //case SDL_USEREVENT:
        default:
            break;
        }

        processEvents( m_pEventTriggerer );
    }
    
    return !m_shutdownRequested;
}

//-------------------------------------------------------------------------------------------------
void SDL2EventHandler::registerEventListener( const CPPCommon::TArray<const Common::Event*> &events,
                                              OSEventListener *pListener ) {
    ce_assert( nullptr != pListener );

    m_pEventTriggerer->addEventListener( events, 
        Common::ceEventFunctor::Make( pListener, &OSEventListener::onOSEvent ) );
}

//-------------------------------------------------------------------------------------------------
void SDL2EventHandler::unregisterEventListener( const CPPCommon::TArray<const Common::Event*> &events, 
                                                OSEventListener *pListener ) {
    ce_assert( nullptr != pListener );

    m_pEventTriggerer->removeEventListener( events, 
        Common::ceEventFunctor::Make( pListener, &OSEventListener::onOSEvent ) );
}
        
//-------------------------------------------------------------------------------------------------
void SDL2EventHandler::enablePolling( bool enabled ) {
    if( enabled == m_isPolling ) {
        return;
    }

    delete m_pInputUpdate;
    if( enabled ) {
        m_pInputUpdate = new SDL2PeekInputUpdate;
    } else {
        m_pInputUpdate = new SDL2GetInputUpdate;
    }
    m_isPolling = enabled;
}

//-------------------------------------------------------------------------------------------------
bool SDL2EventHandler::onAttached( const Common::EventData *pEventData ) {
    return true;
}

//-------------------------------------------------------------------------------------------------
bool SDL2EventHandler::onDetached( const Common::EventData *pEventData ) {
    return true;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
