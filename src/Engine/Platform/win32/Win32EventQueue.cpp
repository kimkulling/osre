/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <src/Engine/Platform/win32/Win32EventQueue.h>
#include <src/Engine/Platform/win32/Win32Window.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Common/EventTriggerer.h>

#include <Windowsx.h>

namespace OSRE {
namespace Platform {

using namespace ::OSRE::Common;

std::map<HWND, Win32EventQueue*> Win32EventQueue::s_WindowsServerMap;

static const c8 *Tag = "Win32Eventhandler";

// The interface for the event update.
struct AbstractInputUpdate {
    ~AbstractInputUpdate() {
        // empty
    }

    virtual bool update( MSG &rProgram ) = 0;
};

struct Win32GetInputUpdate : public AbstractInputUpdate {
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

struct Win32PeekInputUpdate : public AbstractInputUpdate {
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

Win32EventQueue::Win32EventQueue( AbstractWindow *rootWindow )
: AbstractPlatformEventQueue()
, m_updateInstance( nullptr )
, m_eventTriggerer( nullptr )
, m_rootWindow( rootWindow )
, m_shutdownRequested( false )
, m_isPolling( true ) {
    OSRE_ASSERT( nullptr != rootWindow );
    enablePolling( m_isPolling );
    m_eventTriggerer = new EventTriggerer;
    m_eventTriggerer->addTriggerableEvent( KeyboardButtonDownEvent );
    m_eventTriggerer->addTriggerableEvent( KeyboardButtonUpEvent );
    m_eventTriggerer->addTriggerableEvent( MouseButtonDownEvent );
    m_eventTriggerer->addTriggerableEvent( MouseButtonUpEvent );
    m_eventTriggerer->addTriggerableEvent( MouseMoveEvent );
    m_eventTriggerer->addTriggerableEvent( WindowsResizeEvent );
    m_eventTriggerer->addTriggerableEvent( QuitEvent );
    m_eventTriggerer->addTriggerableEvent( AppFocusEvent );
}

Win32EventQueue::~Win32EventQueue( ) {
    m_rootWindow = nullptr;

    delete m_eventTriggerer;
    m_eventTriggerer = nullptr;

    delete m_updateInstance;
    m_updateInstance = nullptr;
}

bool Win32EventQueue::update() {
    EventDataList *activeEventQueue = getActiveEventDataList();
    if ( nullptr == activeEventQueue ) {
        return false;
    }
    MSG	Program;
    if( !m_shutdownRequested && m_updateInstance->update( Program ) ) {
        switch( Program.message ) {
            case WM_ACTIVATE: {
                Common::EventData *data = new Common::EventData( AppFocusEvent, m_eventTriggerer );
                activeEventQueue->addBack( data );
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
                MouseButtonEventData *data = new MouseButtonEventData( true, m_eventTriggerer );
                data->m_Button = MouseButtonEventData::LeftButton;
                getXYPosFromLParam( Program.lParam, data->m_AbsX, data->m_AbsY );
                activeEventQueue->addBack( data );
            }
            break;

            case WM_LBUTTONUP: {
                MouseButtonEventData *data = new MouseButtonEventData( false, m_eventTriggerer );
                data->m_Button = MouseButtonEventData::LeftButton;
                getXYPosFromLParam( Program.lParam, data->m_AbsX, data->m_AbsY );
                activeEventQueue->addBack( data );
            }
            break;

            case WM_MBUTTONDOWN: {
                MouseButtonEventData *data = new MouseButtonEventData( true, m_eventTriggerer );
                data->m_Button = MouseButtonEventData::MiddleButton;
                getXYPosFromLParam( Program.lParam, data->m_AbsX, data->m_AbsY );
                activeEventQueue->addBack( data );
            }
            break;

            case WM_MBUTTONUP: {
                MouseButtonEventData *data = new MouseButtonEventData( false, m_eventTriggerer );
                data->m_Button = MouseButtonEventData::MiddleButton;
                getXYPosFromLParam( Program.lParam, data->m_AbsX, data->m_AbsY );
                activeEventQueue->addBack( data );
            }
            break;

            case WM_RBUTTONDOWN: {
                MouseButtonEventData *data = new MouseButtonEventData( true, m_eventTriggerer );
                data->m_Button = MouseButtonEventData::RightButton;
                getXYPosFromLParam( Program.lParam, data->m_AbsX, data->m_AbsY );
                activeEventQueue->addBack( data );
            }
            break;

            case WM_RBUTTONUP: {
                MouseButtonEventData *data = new MouseButtonEventData( false, m_eventTriggerer );
                data->m_Button = MouseButtonEventData::RightButton;
                getXYPosFromLParam( Program.lParam, data->m_AbsX, data->m_AbsY );
                activeEventQueue->addBack( data );
            }
            break;

            case WM_MOUSEMOVE: {
                MouseMoveEventData *data = new MouseMoveEventData( m_eventTriggerer );
                getXYPosFromLParam( Program.lParam, data->m_absX, data->m_absY );
                activeEventQueue->addBack( data );
            }
            break;

            case WM_KEYDOWN: 
            case WM_KEYUP: {
                KeyboardButtonEventData *data = new KeyboardButtonEventData( Program.message == WM_KEYDOWN, m_eventTriggerer );
                data->m_key = ( Key ) Program.wParam;
                activeEventQueue->addBack( data );
            }
            break;

            case WM_PAINT:
            case WM_SIZE: {
                WindowsResizeEventData *data = new WindowsResizeEventData( m_eventTriggerer );
                RECT rcClient;
                Win32Window *s = ( Win32Window* ) m_rootWindow;
                if ( nullptr != s ) {
                    GetClientRect( s->getHWnd(), &rcClient );
                    ui32 x = rcClient.left;
                    ui32 y = rcClient.top;
                    ui32 w = rcClient.right - rcClient.left;
                    ui32 h = rcClient.bottom - rcClient.top;
                    RenderBackend::RenderBackendService *rbSrv = getRenderBackendService();
                    if (nullptr != rbSrv) {
                        rbSrv->resize( x, y, w, h );
                    }
                    activeEventQueue->addBack( data );
                }
            }
            break;
        }

        processEvents( m_eventTriggerer );

        ::TranslateMessage( &Program );
        ::DispatchMessage( &Program );
    }

    return !m_shutdownRequested;
}

LRESULT Win32EventQueue::winproc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam ) {
    Win32EventQueue *pEventHandler = Win32EventQueue::getInstance( hWnd );
    switch( Message ) {
        case WM_ACTIVATE: {
        return 0;
        }

        case WM_SYSCOMMAND: {
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

void Win32EventQueue::registerEventQueue( Win32EventQueue *server, HWND hWnd ) {
    s_WindowsServerMap[ hWnd ] = server;
}

void Win32EventQueue::unregisterEventQueue( Win32EventQueue *server, HWND hWnd ) {
    std::map<HWND, Win32EventQueue*>::iterator it = s_WindowsServerMap.find( hWnd );
    if( s_WindowsServerMap.end() != it ) {
        if (server != it->second) {
            osre_debug(Tag, "Invalid assignment frin eventqueue to window handle.");
        }
        s_WindowsServerMap.erase( it );
    }
}

Win32EventQueue *Win32EventQueue::getInstance( HWND hWnd ) {
    std::map<HWND, Win32EventQueue*>::iterator it = s_WindowsServerMap.find( hWnd );
    if( s_WindowsServerMap.end() != it ) {
        return it->second;
    }

    return nullptr;
}

bool Win32EventQueue::onQuit() {
    Common::EventData data( QuitEvent, m_eventTriggerer );
    m_eventTriggerer->triggerEvent( data.getEvent(), &data );
    m_shutdownRequested = true;

    return true;
}

void Win32EventQueue::setRootSurface( AbstractWindow *pSurface ) {
    if( !pSurface ) {
        osre_debug( Tag, "Invalid window pointer." );
        return;
    }

    Win32Window *pWin32Surface = ( Win32Window* ) pSurface;
    registerEventQueue( this, pWin32Surface->getHWnd() );

    m_rootWindow = pWin32Surface;
}

AbstractWindow *Win32EventQueue::getRootSurface( ) const {
    return m_rootWindow;
}

void Win32EventQueue::enablePolling( bool enabled ) {
    if( m_updateInstance && enabled == m_isPolling ) {
        return;
    }

    if( m_updateInstance ) {
        delete m_updateInstance;
        m_updateInstance = nullptr;
    }

    m_isPolling = enabled;
    
    if( m_isPolling ) {
        m_updateInstance = new Win32PeekInputUpdate;
    } else {
        m_updateInstance = new Win32GetInputUpdate;
    }
}

bool Win32EventQueue::isPolling( ) const {
    return m_isPolling;
}

void Win32EventQueue::registerEventListener( const EventPtrArray &events, OSEventListener *pListener ) {
    OSRE_ASSERT(nullptr != m_eventTriggerer);

    if (nullptr == m_eventTriggerer) {
        osre_error(Tag, "Pointer to event-triggerer is nullptr.");
        return;
    }

    m_eventTriggerer->addEventListener( events, Common::EventFunctor::Make( pListener,
        &OSEventListener::onOSEvent ) );
}

void Win32EventQueue::unregisterEventListener( const EventPtrArray &events, OSEventListener *pListener ) {
    OSRE_ASSERT( nullptr != m_eventTriggerer );

    if (nullptr == m_eventTriggerer) {
        osre_error(Tag, "Pointer to event-triggerer is nullptr.");
        return;
    }

    m_eventTriggerer->removeEventListener( events, Common::EventFunctor::Make( pListener,
        &OSEventListener::onOSEvent ) );
}

void Win32EventQueue::unregisterAllEventHandler(const EventPtrArray &events) {
    OSRE_ASSERT(nullptr != m_eventTriggerer);

    if (nullptr == m_eventTriggerer) {
        osre_error(Tag, "Pointer to event-triggerer is nullptr.");
        return;
    }

    m_eventTriggerer->removeAllEventListeners(events);
}

} // Namespace Platform
} // Namespace OSRE
