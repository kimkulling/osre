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
#include <src/Engine/Platform/sdl2/SDL2EventHandler.h>
#include <osre/Common/EventTriggerer.h>
#include <osre/Platform/PlatformInterface.h>

#include <SDL.h>

namespace OSRE {
namespace Platform {

using namespace OSRE::Common;

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
void SDL2EventHandler::registerEventListener( const CPPCore::TArray<const Common::Event*> &events,
                                              OSEventListener *pListener ) {
    assert( nullptr != pListener );

    m_pEventTriggerer->addEventListener( events, 
        Common::EventFunctor::Make( pListener, &OSEventListener::onOSEvent ) );
}

//-------------------------------------------------------------------------------------------------
void SDL2EventHandler::unregisterEventListener( const CPPCore::TArray<const Common::Event*> &events,
                                                OSEventListener *pListener ) {
    assert( nullptr != pListener );

    m_pEventTriggerer->removeEventListener( events, 
        Common::EventFunctor::Make( pListener, &OSEventListener::onOSEvent ) );
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
} // Namespace OSRE
