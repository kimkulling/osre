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
#include <src/Engine/Platform/win32/Win32Surface.h>
#include <src/Engine/Platform/win32/Win32Eventhandler.h>

namespace OSRE {
namespace Platform {
    
Win32Surface::Win32Surface( SurfaceProperties *pProperties )
: AbstractSurface( pProperties )
, m_hInstance( nullptr )
, m_wnd( nullptr )
, m_dc( nullptr ) {
    // empty
}

Win32Surface::~Win32Surface( ) {
    // empty
}


void Win32Surface::setWindowsTitle( const String &title ) {
    if ( nullptr == m_wnd ) {
        return;
    }

    ::SetWindowText( m_wnd, title.c_str() );
}

HWND Win32Surface::getHWnd( ) const {
    return m_wnd;
}

HDC Win32Surface::getDeviceContext( ) const {
    return m_dc;
}

HINSTANCE Win32Surface::getModuleHandle() const {
    return m_hInstance;
}

bool Win32Surface::onCreate( ) {
    SurfaceProperties *prop = getProperties();
    if( nullptr == prop ) {
        return false;
    }

    WNDCLASS sWC;
    DWORD dwExStyle( 0 ), dwStyle( 0 );
    RECT clientSize;
    clientSize.left = prop->m_x;
    clientSize.top = prop->m_y;
    clientSize.right = prop->m_x + prop->m_width;
    clientSize.bottom = prop->m_y + prop->m_height;

    DWORD style = WS_POPUP;
    if( !prop->m_fullscreen ) {
        style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    }

    ::AdjustWindowRect( &clientSize, style, FALSE );
    const ui32 realWidth( clientSize.right - clientSize.left );
    const ui32 realHeight( clientSize.bottom - clientSize.top );

    ui32 cx = prop->m_width / 2;
    ui32 cy = prop->m_height / 2;
    m_hInstance = ::GetModuleHandle( NULL );
    sWC.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    sWC.lpfnWndProc = ( WNDPROC ) Win32Eventhandler::winproc;
    sWC.cbClsExtra = 0;
    sWC.cbWndExtra = 0;
    sWC.hInstance = m_hInstance;
    sWC.hIcon = ::LoadIcon( NULL, IDI_WINLOGO );
    sWC.hCursor = ::LoadCursor( NULL, IDC_ARROW );
    sWC.hbrBackground = ( HBRUSH )::GetStockObject( BLACK_BRUSH );
    sWC.lpszMenuName = NULL;
    sWC.lpszClassName = prop->m_title.c_str();
    if ( !::RegisterClass( &sWC ) ) {
        return false;
    }

    if( prop->m_fullscreen ) {
        DEVMODE dmScreenSettings;
        ::memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );
        dmScreenSettings.dmSize = sizeof( dmScreenSettings );
        dmScreenSettings.dmPelsWidth = realWidth;
        dmScreenSettings.dmPelsHeight = realHeight;
        dmScreenSettings.dmBitsPerPel = prop->m_colordepth;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        if( ::ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL ) {
            ::MessageBox( NULL, "Cannot change to full-screen mode",
                "Error", MB_OK | MB_ICONEXCLAMATION );
            prop->m_fullscreen = false;
        } else {
            dwExStyle = WS_EX_APPWINDOW;
            dwStyle = WS_POPUP;
            ::ShowCursor( false );
        }
    } else {
        // Window Ext. Style
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        
        // Windows Style
        dwStyle = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
        if ( prop->m_resizable ) {
            dwStyle |= WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX ;
        }
    }

    m_wnd = ::CreateWindowEx( dwExStyle,
        prop->m_title.c_str(),
        prop->m_title.c_str(),
        dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        prop->m_x, 
        prop->m_y,
        realWidth, realHeight,
        NULL, NULL,
        m_hInstance,
        NULL );

    if( !m_wnd ) {
        MessageBox( NULL, "Cannot create the application window.", "Abort application",
            MB_OK | MB_ICONEXCLAMATION );
        return false;
    }

    m_dc = ::GetDC( m_wnd );
    if( !m_dc ) {
        ::MessageBox( NULL, "Cannot create a device context.", "Abort application",
            MB_OK | MB_ICONEXCLAMATION );
        return false;
    }
    ::ShowWindow( m_wnd, SW_SHOW );
    ::SetForegroundWindow( m_wnd );
    ::SetFocus( m_wnd );
    ::SetCursorPos( cx, cy );
    prop->m_open = true;

    return true;
}

bool Win32Surface::onDestroy( ) {
    SurfaceProperties *prop = getProperties();
    if( nullptr == prop ) {
        return false;
    }

    if( !prop->m_open ) {
        return false;
    }

    if ( !prop->m_fullscreen ) {
        ::ChangeDisplaySettings( NULL, 0 );
    }

    if( m_dc && !::ReleaseDC( m_wnd, m_dc ) ) {
        MessageBox( NULL, "Cannot release the device context.",
            "Abort application", MB_OK | MB_ICONEXCLAMATION );
        m_dc = NULL;
    }

    if( m_wnd && !::DestroyWindow( m_wnd ) ) {
        ::MessageBox( NULL, "Cannot destroy the window.",
            "Abort application", MB_OK | MB_ICONEXCLAMATION );
        m_wnd = NULL;
    }

    if( !::UnregisterClass( prop->m_title.c_str(), m_hInstance ) ) {
        ::MessageBox( NULL, "Cannot unregister the application .",
            "Abort application", MB_OK | MB_ICONEXCLAMATION );
        m_hInstance = NULL;
    }

    prop->m_open = false;

    return true;
}

bool Win32Surface::onUpdateProperies() {
    const ui32 flags( AbstractSurface::getFlags() );
    if( flags | SF_WinTitleDirty ) {
        const String &title(AbstractSurface::getProperties()->m_title );
        ::SetWindowText( getHWnd(), title.c_str() );   
    }
    AbstractSurface::setFlags( SF_PropertiesClean );

    return true;
}

void Win32Surface::onResize( ui32 x, ui32 y, ui32 w, ui32 h ) {
    ::SetWindowPos( m_wnd, HWND_TOP, x, y, w, h, SWP_NOSIZE );
    SurfaceProperties *props( AbstractSurface::getProperties() );
    if ( nullptr != props ) {
        props->m_x = x;
        props->m_y = y;
        props->m_width = w;
        props->m_height = h;
        AbstractSurface::setProperties( props );
    }
}

} // Namespace Platform
} // Namespace OSRE
