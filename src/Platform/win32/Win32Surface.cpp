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
#include <src/Platform/win32/Win32Surface.h>
#include <src/Platform/win32/Win32Eventhandler.h>

namespace OSRE {
namespace Platform {
    
//-------------------------------------------------------------------------------------------------
Win32Surface::Win32Surface( SurfaceProperties *pProperties )
: AbstractSurface( pProperties )
, m_hInstance( nullptr )
, m_wnd( nullptr )
, m_dc( nullptr ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
Win32Surface::~Win32Surface( ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
HWND Win32Surface::getHWnd( ) const {
    return m_wnd;
}

//-------------------------------------------------------------------------------------------------
HDC Win32Surface::getDeviceContext( ) const {
    return m_dc;
}

//-------------------------------------------------------------------------------------------------
bool Win32Surface::onCreate( ) {
    SurfaceProperties *pProp = getProperties();
    if( !pProp ) {
        return false;
    }

    WNDCLASS sWC;
    DWORD dwExStyle( 0 ), dwStyle( 0 );
    RECT clientSize;
    clientSize.left = pProp->m_x;
    clientSize.top = pProp->m_y;
    clientSize.right = pProp->m_x + pProp->m_width;
    clientSize.bottom = pProp->m_y + pProp->m_height;

    DWORD style = WS_POPUP;
    if( !pProp->m_fullscreen ) {
        style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    }

    ::AdjustWindowRect( &clientSize, style, FALSE );
    const ui32 realWidth = clientSize.right - clientSize.left;
    const ui32 realHeight = clientSize.bottom - clientSize.top;

    ui32 cx = pProp->m_width / 2;
    ui32 cy = pProp->m_height / 2;
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
    sWC.lpszClassName = pProp->m_title.c_str();
    if( !::RegisterClass( &sWC ) )
        return false;

    if( pProp->m_fullscreen ) {
        DEVMODE dmScreenSettings;
        ::memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );
        dmScreenSettings.dmSize = sizeof( dmScreenSettings );
        dmScreenSettings.dmPelsWidth = realWidth;
        dmScreenSettings.dmPelsHeight = realHeight;
        dmScreenSettings.dmBitsPerPel = pProp->m_colordepth;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        if( ::ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL ) {
            ::MessageBox( NULL, "Cannot change to full-screen mode",
                "Error", MB_OK | MB_ICONEXCLAMATION );
            pProp->m_fullscreen = false;
        } else {
            dwExStyle = WS_EX_APPWINDOW;
            dwStyle = WS_POPUP;
            ::ShowCursor( false );
        }
    } else {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;                                  // Window Ext. Style
        dwStyle = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;// Windows Style
    }

    m_wnd = ::CreateWindowEx( dwExStyle,
        pProp->m_title.c_str(),
        pProp->m_title.c_str(),
        dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        pProp->m_x, pProp->m_y,
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
    pProp->m_open = true;

    return true;
}

//-------------------------------------------------------------------------------------------------
bool Win32Surface::onDestroy( ) {
    SurfaceProperties *pProp = getProperties();
    if( !pProp ) {
        return false;
    }

    if( !pProp->m_open ) {
        return false;
    }

    if( !pProp->m_fullscreen )
        ::ChangeDisplaySettings( NULL, 0 );

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

    if( !::UnregisterClass( pProp->m_title.c_str(), m_hInstance ) ) {
        ::MessageBox( NULL, "Cannot unregister the application .",
            "Abort application", MB_OK | MB_ICONEXCLAMATION );
        m_hInstance = NULL;
    }

    pProp->m_open = false;

    return true;
}

//-------------------------------------------------------------------------------------------------
bool Win32Surface::onUpdateProperies() {
    const ui32 flags( AbstractSurface::getFlags() );
    if( flags | SF_WinTitleDirty ) {
        const ce_string &title(AbstractSurface::getProperties()->m_title );
        ::SetWindowText( getHWnd(), title.c_str() );   
    }
    AbstractSurface::setFlags( SF_PropertiesClean );

    return true;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
