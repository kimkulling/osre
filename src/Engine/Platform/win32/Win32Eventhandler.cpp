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
#include <src/Engine/Platform/win32/Win32Eventhandler.h>
#include <src/Engine/Platform/win32/Win32Surface.h>

#include <osre/Platform/PlatformInterface.h>
#include <osre/Common/EventTriggerer.h>

namespace OSRE {
namespace Platform {

using namespace ::OSRE::Common;

std::map<HWND, Win32Eventhandler*> Win32Eventhandler::s_WindowsServerMap;

static const String Tag = "Win32Eventhandler";

struct IInputUpdate {
    ~IInputUpdate(){
        // empty
    }

    virtual bool update( MSG &rProgram ) = 0;
};

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

static void getXYPosFromLParam( LPARAM lparam, i32 &x, i32 &y ) {
    x = GET_X_LPARAM( lparam );
    y = GET_Y_LPARAM( lparam );
}

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

Win32Eventhandler::~Win32Eventhandler( ) {
    delete m_pOSEventTriggerer;
    m_pOSEventTriggerer = nullptr;

    delete m_pUpdateInstance;
    m_pUpdateInstance = nullptr;
}

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
            case WM_CLOSE: 
            {
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

            case WM_LBUTTONDOWN: 
            {
                MouseButtonEventData *data = new MouseButtonEventData( true, m_pOSEventTriggerer );
                data->m_Button = MouseButtonEventData::LeftButton;
                getXYPosFromLParam( Program.lParam, data->m_AbsX, data->m_AbsY );
                pActiveEventQueue->addBack( data );
            }
            break;

            case WM_LBUTTONUP: 
            {
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
                osre_error( Tag, "Nullptr to event handler detected." );
            }
            ::PostQuitMessage( NULL );
            return 1;
        }
    }

    return ::DefWindowProc( hWnd, Message, wParam, lParam );
}

bool Win32Eventhandler::onAttached( const EventData *eventData ) {
    return true;
}

bool Win32Eventhandler::onDetached( const EventData *eventData ) {
    return true;
}

void Win32Eventhandler::registerEventServer( Win32Eventhandler *server, HWND hWnd ) {
    s_WindowsServerMap[ hWnd ] = server;
}

void Win32Eventhandler::unregisterEventServer( Win32Eventhandler *pServer, HWND hWnd ) {
    std::map<HWND, Win32Eventhandler*>::iterator it = s_WindowsServerMap.find( hWnd );
    if( s_WindowsServerMap.end() != it ) {
        s_WindowsServerMap.erase( it );
    }
}

Win32Eventhandler *Win32Eventhandler::getInstance( HWND hWnd ) {
    std::map<HWND, Win32Eventhandler*>::iterator it = s_WindowsServerMap.find( hWnd );
    if( s_WindowsServerMap.end() != it ) {
        return it->second;
    }

    return nullptr;
}

bool Win32Eventhandler::onQuit() {
    Common::EventData data( QuitEvent, m_pOSEventTriggerer );
    m_pOSEventTriggerer->triggerEvent( data.getEvent(), &data );
    m_shutdownRequested = true;

    return true;
}

void Win32Eventhandler::setRootSurface( AbstractSurface *pSurface ) {
    if( !pSurface ) {
        osre_debug( Tag, "Invalid window pointer." );
        return;
    }

    Win32Surface *pWin32Surface = ( Win32Surface* ) pSurface;
    registerEventServer( this, pWin32Surface->getHWnd() );

    m_pRootSurface = pWin32Surface;

}

AbstractSurface *Win32Eventhandler::getRootSurface( ) const {
    return m_pRootSurface;
}

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

bool Win32Eventhandler::isPolling( ) const {
    return m_isPolling;
}

void Win32Eventhandler::registerEventListener( const CPPCore::TArray<const Common::Event*> &events, OSEventListener *pListener ) {
    assert( nullptr != m_pOSEventTriggerer );

    m_pOSEventTriggerer->addEventListener( events, Common::EventFunctor::Make( pListener,
        &OSEventListener::onOSEvent ) );
}

void Win32Eventhandler::unregisterEventListener( const CPPCore::TArray<const Common::Event*> &events, OSEventListener *pListener ) {
    assert( nullptr != m_pOSEventTriggerer );

    m_pOSEventTriggerer->removeEventListener( events, Common::EventFunctor::Make( pListener,
        &OSEventListener::onOSEvent ) );
}

} // Namespace Platform
} // Namespace OSRE
