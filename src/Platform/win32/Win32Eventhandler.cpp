/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-214, ZFXCE2 Development Team
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
#include <code/Infrastructure/Platform/win32/Win32Eventhandler.h>
#include <code/Infrastructure/Platform/win32/Win32Surface.h>
#include <osre2/Infrastructure/Common/EventTriggerer.h>
#include <osre2/Infrastructure/Platform/PlatformInterface.h>

namespace ZFXCE2 {
namespace Platform {

using namespace ZFXCE2::Common;

std::map<HWND, Win32Eventhandler*> Win32Eventhandler::s_WindowsServerMap;

//-------------------------------------------------------------------------------------------------
struct IInputUpdate {
    ~IInputUpdate(){
        // empty
    }

    virtual bool update( MSG &rProgram ) = 0;
};

//-------------------------------------------------------------------------------------------------
struct Win32GetInputUpdate : public IInputUpdate {
    Win32GetInputUpdate( ) {
        // empty
    }

    ~Win32GetInputUpdate( ) {
        // empty
    }

    bool update( MSG &program ) {
        if( 1 == ::GetMessage( &program, NULL, 0, 0 ) ) {
            return true;
        }

        return false;
    }
};

//-------------------------------------------------------------------------------------------------
struct Win32PeekInputUpdate : public IInputUpdate {
    Win32PeekInputUpdate( ) {
        // empty
    }
    
    ~Win32PeekInputUpdate( ) {
        // empty
    }

    bool update( MSG &rProgram ) {
        if( 1 == ::PeekMessage( &rProgram, NULL, 0, 0, PM_REMOVE ) ) {
            return true;
        }
        return false;
    }
};

//-------------------------------------------------------------------------------------------------
static void getXYPosFromLParam( LPARAM lparam, i32 &x, i32 &y ) {
    x = GET_X_LPARAM( lparam );
    y = GET_Y_LPARAM( lparam );
}

//-------------------------------------------------------------------------------------------------
Win32Eventhandler::Win32Eventhandler( )
: AbstractPlatformEventHandler()
, m_pUpdateInstance( nullptr )
, m_pOSEventTriggerer( nullptr )
, m_pRootSurface( nullptr )
, m_shutdownRequested( false )
, m_isPolling( true ) {
    enablePolling( m_isPolling );
    m_pOSEventTriggerer = new EventTriggerer;
    m_pOSEventTriggerer->addTriggerableEvent( KeyboardButtonDownEvent );
    m_pOSEventTriggerer->addTriggerableEvent( KeyboardButtonUpEvent );
    m_pOSEventTriggerer->addTriggerableEvent( MouseButtonDownEvent );
    m_pOSEventTriggerer->addTriggerableEvent( MouseButtonUpEvent );
    m_pOSEventTriggerer->addTriggerableEvent( MouseMoveEvent );
    m_pOSEventTriggerer->addTriggerableEvent( QuitEvent );
    m_pOSEventTriggerer->addTriggerableEvent( AppFocusEvent );
}

//-------------------------------------------------------------------------------------------------
Win32Eventhandler::~Win32Eventhandler( ) {
    delete m_pOSEventTriggerer;
    m_pOSEventTriggerer = nullptr;

    delete m_pUpdateInstance;
    m_pUpdateInstance = nullptr;
}

//-------------------------------------------------------------------------------------------------
bool Win32Eventhandler::onEvent( const Common::Event &ev, const Common::EventData *pEventData ) {
    EventDataList *pActiveEventQueue = getActiveEventDataList();
    MSG	Program;
    if( !m_shutdownRequested && m_pUpdateInstance->update( Program ) ) {
        switch( Program.message ) {
            case WM_ACTIVATE: {
                Common::EventData *data = new Common::EventData( AppFocusEvent, m_pOSEventTriggerer );
                pActiveEventQueue->addBack( data );
            }
            break;

            case WM_QUIT:
            case WM_CLOSE: {
                onQuit();
                m_shutdownRequested = true;
            }
            break;


            case WM_SYSCOMMAND: {
                switch( Program.wParam ) {
                    case SC_SCREENSAVE:
                    case SC_MONITORPOWER:
                        return true;

                    default:
                        return false;
                }
            }
            break;

            case WM_LBUTTONDOWN: {
                MouseButtonEventData *data = new MouseButtonEventData( true, m_pOSEventTriggerer );
                data->m_Button = MouseButtonEventData::LeftButton;
                getXYPosFromLParam( Program.lParam, data->m_AbsX, data->m_AbsY );
                pActiveEventQueue->addBack( data );
            }
            break;

            case WM_LBUTTONUP: {
                MouseButtonEventData *data = new MouseButtonEventData( false, m_pOSEventTriggerer );
                data->m_Button = MouseButtonEventData::LeftButton;
                getXYPosFromLParam( Program.lParam, data->m_AbsX, data->m_AbsY );
                pActiveEventQueue->addBack( data );
            }
            break;

            case WM_MBUTTONDOWN:
            {
                MouseButtonEventData *data = new MouseButtonEventData( true, m_pOSEventTriggerer );
                data->m_Button = MouseButtonEventData::MiddleButton;
                getXYPosFromLParam( Program.lParam, data->m_AbsX, data->m_AbsY );
                pActiveEventQueue->addBack( data );
            }
            break;

            case WM_MBUTTONUP:
            {
                MouseButtonEventData *data = new MouseButtonEventData( false, m_pOSEventTriggerer );
                data->m_Button = MouseButtonEventData::MiddleButton;
                getXYPosFromLParam( Program.lParam, data->m_AbsX, data->m_AbsY );
                pActiveEventQueue->addBack( data );
            }
            break;

            case WM_RBUTTONDOWN:
            {
                MouseButtonEventData *data = new MouseButtonEventData( true, m_pOSEventTriggerer );
                data->m_Button = MouseButtonEventData::RightButton;
                getXYPosFromLParam( Program.lParam, data->m_AbsX, data->m_AbsY );
                pActiveEventQueue->addBack( data );
            }
            break;

            case WM_RBUTTONUP:
            {
                MouseButtonEventData *data = new MouseButtonEventData( false, m_pOSEventTriggerer );
                data->m_Button = MouseButtonEventData::RightButton;
                getXYPosFromLParam( Program.lParam, data->m_AbsX, data->m_AbsY );
                pActiveEventQueue->addBack( data );
            }
            break;

            case WM_MOUSEMOVE:
            {
                MouseMoveEventData *data = new MouseMoveEventData( m_pOSEventTriggerer );
                getXYPosFromLParam( Program.lParam, data->m_AbsX, data->m_AbsY );
                pActiveEventQueue->addBack( data );
            }
            break;

            case WM_KEYDOWN:
            {
                KeyboardButtonEventData *data = new KeyboardButtonEventData( true, m_pOSEventTriggerer );
                data->m_Key = ( Key ) Program.wParam;
                pActiveEventQueue->addBack( data );
            }
            break;

            case WM_KEYUP:
            {
                KeyboardButtonEventData *data = new KeyboardButtonEventData( false, m_pOSEventTriggerer );
                data->m_Key = ( Key ) Program.wParam;
                pActiveEventQueue->addBack( data );
            }
            break;
        }

        processEvents( m_pOSEventTriggerer );

        ::TranslateMessage( &Program );
        ::DispatchMessage( &Program );
    }

    return !m_shutdownRequested;
}

//-------------------------------------------------------------------------------------------------
LRESULT Win32Eventhandler::winproc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam ) {
    Win32Eventhandler *pEventHandler = Win32Eventhandler::getInstance( hWnd );
    switch( Message ) {
        case WM_ACTIVATE:
            return 0;

        case WM_SYSCOMMAND:
        {
            switch( wParam ) {
            case SC_SCREENSAVE:
            case SC_MONITORPOWER:
                return 0;

            case WM_KEYDOWN:
                return 0;
            }
            break;
        }

        case WM_CLOSE: {
            if( pEventHandler ) {
                pEventHandler->onQuit();
            } else {
                ce_error( "Nullptr to event handler detected." );
            }
            ::PostQuitMessage( NULL );
            return 1;
        }
    }

    return ::DefWindowProc( hWnd, Message, wParam, lParam );
}

//-------------------------------------------------------------------------------------------------
bool Win32Eventhandler::onAttached( const Common::EventData *pEventData ) {
    return true;
}

//-------------------------------------------------------------------------------------------------
bool Win32Eventhandler::onDetached( const Common::EventData *pEventData ) {
    return true;
}

//-------------------------------------------------------------------------------------------------
void Win32Eventhandler::registerEventServer( Win32Eventhandler *pServer, HWND hWnd ) {
    s_WindowsServerMap[ hWnd ] = pServer;
}

//-------------------------------------------------------------------------------------------------
void Win32Eventhandler::unregisterEventServer( Win32Eventhandler *pServer, HWND hWnd ) {
    std::map<HWND, Win32Eventhandler*>::iterator it = s_WindowsServerMap.find( hWnd );
    if( s_WindowsServerMap.end() != it ) {
        s_WindowsServerMap.erase( it );
    }
}

//-------------------------------------------------------------------------------------------------
Win32Eventhandler *Win32Eventhandler::getInstance( HWND hWnd ) {
    std::map<HWND, Win32Eventhandler*>::iterator it = s_WindowsServerMap.find( hWnd );
    if( s_WindowsServerMap.end() != it ) {
        return it->second;
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------
bool Win32Eventhandler::onQuit() {
    Common::EventData data( QuitEvent, m_pOSEventTriggerer );
    m_pOSEventTriggerer->triggerEvent( data.getEvent(), &data );
    m_shutdownRequested = true;

    return true;
}

//-------------------------------------------------------------------------------------------------
void Win32Eventhandler::setRootSurface( AbstractSurface *pSurface ) {
    if( !pSurface ) {
        ce_debug( "Invalid window pointer." );
        return;
    }

    Win32Surface *pWin32Surface = ( Win32Surface* ) pSurface;
    registerEventServer( this, pWin32Surface->getHWnd() );

    m_pRootSurface = pWin32Surface;

}

//-------------------------------------------------------------------------------------------------
AbstractSurface *Win32Eventhandler::getRootSurface( ) const {
    return m_pRootSurface;
}

//-------------------------------------------------------------------------------------------------
void Win32Eventhandler::enablePolling( bool enabled ) {
    if( m_pUpdateInstance && enabled == m_isPolling ) {
        return;
    }

    if( m_pUpdateInstance ) {
        delete m_pUpdateInstance;
        m_pUpdateInstance = nullptr;
    }
    m_isPolling = enabled;
    if( m_isPolling ) {
        m_pUpdateInstance = new Win32PeekInputUpdate;
    } else {
        m_pUpdateInstance = new Win32GetInputUpdate;
    }
}

//-------------------------------------------------------------------------------------------------
bool Win32Eventhandler::isPolling( ) const {
    return m_isPolling;
}

//-------------------------------------------------------------------------------------------------
void Win32Eventhandler::registerEventListener( const CPPCommon::TArray<const Common::Event*> &events, OSEventListener *pListener ) {
    ce_assert( nullptr != m_pOSEventTriggerer );

    m_pOSEventTriggerer->addEventListener( events, Common::ceEventFunctor::Make( pListener,
        &OSEventListener::onOSEvent ) );
}

//-------------------------------------------------------------------------------------------------
void Win32Eventhandler::unregisterEventListener( const CPPCommon::TArray<const Common::Event*> &events, OSEventListener *pListener ) {
    ce_assert( nullptr != m_pOSEventTriggerer );

    m_pOSEventTriggerer->removeEventListener( events, Common::ceEventFunctor::Make( pListener,
        &OSEventListener::onOSEvent ) );
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
