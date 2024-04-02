/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <src/Engine/Platform/win32/Win32Window.h>
#include "commctrl.h"

namespace OSRE {
namespace Platform {

static constexpr c8 Tag[] = "Win32Window";

Win32Window::Win32Window(WindowsProperties *properties) :
        AbstractWindow(properties),
        mInstance(nullptr),
        mWnd(nullptr),
        mHandleStatusBar(nullptr),
        mParent(nullptr),
        mDC(nullptr),
        mMenu(nullptr),
        mStatusBarContent(),
        mMenuCreateState(false) {
    // empty
}

void Win32Window::showWindow(ShowState showState) {
    if (mWnd == nullptr) {
        return;
    }

    if (showState != getShowState()) {
        setShowState(showState);
        int ws = SW_HIDE;
        if (showState == ShowState::Visible) {
            ws = SW_SHOW;
        }    
        ::ShowWindow(mWnd, ws);
    }
}

void Win32Window::setWindowsTitle(const String &title) {
    if (nullptr == mWnd) {
        return;
    }

    ::SetWindowText(mWnd, title.c_str());
}

void Win32Window::setWindowsMouseCursor(DefaultMouseCursorType ct){
    if (nullptr == mInstance) {
        return;
    }

    HCURSOR c;
    if (ct == DefaultMouseCursorType::WaitCursor) {
        c = ::LoadCursorA(nullptr, IDC_APPSTARTING);
    } else if ( ct == DefaultMouseCursorType::SelectCursor) {
        c = ::LoadCursorA(nullptr, IDC_CROSS);
    } else {
        c = ::LoadCursorA(nullptr, IDC_ARROW);
    }
    ::SetCursor(c);
}

HWND Win32Window::getHWnd() const {
    return mWnd;
}

HDC Win32Window::getDeviceContext() const {
    return mDC;
}

HINSTANCE Win32Window::getModuleHandle() const {
    return mInstance;
}

HMENU Win32Window::getMenuHandle() {
    if (nullptr == mMenu) {
        mMenu = CreateMenu();
    }
    return mMenu;
}

HMENU Win32Window::beginMenu() {
    if (mMenuCreateState) {
        return nullptr;
    }
    HMENU handle = getMenuHandle();
    mMenuCreateState = true;

    return handle;
}

void Win32Window::addSubMenues(HMENU parent, AbstractPlatformEventQueue *queue, wchar_t *title,
        MenuEntry *menu_entries, size_t numItems) {
    if (!mMenuCreateState || nullptr == queue) {
        osre_debug(Tag, "Not in proper state for adding sub-menues.");
        return;
    }

    if (nullptr == parent) {
        parent = getMenuHandle();
    }
    HMENU hMenu = ::CreateMenu();
    for (size_t i = 0; i < numItems; ++i) {
        ::AppendMenuW(hMenu, menu_entries[i].Type, menu_entries[i].Id, menu_entries[i].Name);
        if (0 != menu_entries[i].Id) {
            queue->registerMenuCommand(menu_entries[i].Id, menu_entries[i].Func);
        }
    }
    ::AppendMenuW(parent, MF_POPUP, (UINT_PTR)hMenu, title);
}

void Win32Window::endMenu() {
    ::SetMenu(mWnd, mMenu);
    mMenuCreateState = false;
}

HWND Win32Window::createStatusBar(UINT ResID, ui32 numFields) {
    mHandleStatusBar = CreateWindowEx(0L, // extended style
            STATUSCLASSNAME, (PCTSTR)NULL,
            SBARS_SIZEGRIP | // includes a sizing grip
            WS_CHILD | WS_VISIBLE, // window styles
            0, 0, 0, 0, // x, y, width, height
            mWnd, (HMENU)ResID, mInstance, NULL);
    cppcore::TArray<ui32> fields;
    ui32 w[4] = { 100, 300, 600, 1000 };
    for (ui32 i = 0; i < numFields; ++i) {
        mStatusBarContent.StatusBarTexts.add("");
        mStatusBarContent.StatusBarWidths.add(w[i]);
    }

    ::SendMessage(mHandleStatusBar, SB_SETPARTS, mStatusBarContent.StatusBarWidths.size(), 
        (LPARAM)&mStatusBarContent.StatusBarWidths[0]);
    for (ui32 i = 0; i < numFields; ++i) {
        ::SendMessage(mHandleStatusBar, SB_SETTEXT, i, (LPARAM) "");
    }
    ::SendMessage(mHandleStatusBar, SB_SIMPLE, 0, 0);
    return mHandleStatusBar;
}

HWND Win32Window::getStatusBarHandle() const {
    return mHandleStatusBar;
}

void Win32Window::setStatusText(ui32 index, const char *text) {
    if (index >= mStatusBarContent.StatusBarTexts.size()) {
        return;
    }

    mStatusBarContent.StatusBarTexts[index] = text;
    ::SendMessage(mHandleStatusBar, SB_SETPARTS, mStatusBarContent.StatusBarWidths.size(),
        (LPARAM)&mStatusBarContent.StatusBarWidths[0]);
    for (ui32 i = 0; i < mStatusBarContent.StatusBarTexts.size(); ++i) {
        ::SendMessage(mHandleStatusBar, SB_SETTEXT, i, (LPARAM)mStatusBarContent.StatusBarTexts[i].c_str());
    }
    ::SendMessage(mHandleStatusBar, SB_SIMPLE, 0, 0);
}

void Win32Window::setParentHandle( HWND hWnd ) { 
    ::SetParent(getHWnd(), hWnd);
    mParent = hWnd;
}

HWND Win32Window::getParentHandle() const {
    return mParent;
}

bool Win32Window::onCreate() {
    WindowsProperties *prop = getProperties();
    if (nullptr == prop) {
        return false;
    }

    WNDCLASS sWC = {}; 
    DWORD dwExStyle(0), dwStyle(0);
    RECT clientSize;
    clientSize.left = prop->mRect.x1;
    clientSize.top = prop->mRect.y1;
    clientSize.right = prop->mRect.x1 + prop->mRect.width;
    clientSize.bottom = prop->mRect.y1 + prop->mRect.height;

    DWORD style = WS_POPUP;
    if (!prop->m_fullscreen) {
        style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    } else if (prop->m_childWindow) {
        style = WS_BORDER | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;    
    }

    ::AdjustWindowRect(&clientSize, style, FALSE);
    const ui32 realWidth = clientSize.right - clientSize.left;
    const ui32 realHeight = clientSize.bottom - clientSize.top;

    ui32 cx = prop->mRect.width / 2;
    ui32 cy = prop->mRect.height / 2;
    mInstance = ::GetModuleHandle(NULL);
    sWC.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    sWC.lpfnWndProc = (WNDPROC)Win32EventQueue::winproc;
    sWC.cbClsExtra = 0;
    sWC.cbWndExtra = 0;
    sWC.hInstance = mInstance;
    sWC.hIcon = ::LoadIcon(NULL, IDI_WINLOGO);
    sWC.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    sWC.hbrBackground = (HBRUSH)::GetStockObject(BLACK_BRUSH);
    sWC.lpszMenuName = NULL;
    sWC.lpszClassName = prop->m_title.c_str();
    if (!::RegisterClass(&sWC)) {
        return false;
    }

    if (prop->m_fullscreen) {
        DEVMODE dmScreenSettings;
        ::memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
        dmScreenSettings.dmSize = sizeof(dmScreenSettings);
        dmScreenSettings.dmPelsWidth = realWidth;
        dmScreenSettings.dmPelsHeight = realHeight;
        dmScreenSettings.dmBitsPerPel = prop->m_colordepth;
        dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
        if (::ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
            ::MessageBox(NULL, "Cannot change to full-screen mode",
                    "Error", MB_OK | MB_ICONEXCLAMATION);
            prop->m_fullscreen = false;
        } else {
            dwExStyle = WS_EX_APPWINDOW;
            dwStyle = WS_POPUP;
            ::ShowCursor(false);
        }
    } else {
        // Window Ext. Style
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

        // Windows Style
        dwStyle = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
        if (prop->m_resizable) {
            dwStyle |= WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SIZEBOX;
        }
        if (prop->m_childWindow) {
            dwExStyle = 0;
            dwStyle = WS_VISIBLE;
        }
    }

    mWnd = ::CreateWindowEx(dwExStyle,
            prop->m_title.c_str(),
            prop->m_title.c_str(),
            dwStyle,
            prop->mRect.x1,
            prop->mRect.y1,
            realWidth,
            realHeight,
            NULL,
            NULL,
            mInstance,
            NULL);

    if (nullptr == mWnd) {
        MessageBox(NULL, "Cannot create the application window.", "Abort application",
                MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    if (prop->m_childWindow) {
        ::SetWindowLong(mWnd, GWL_STYLE, 0);
    }
    mDC = ::GetDC(mWnd);
    if (!mDC) {
        ::MessageBox(NULL, "Cannot create a device context.", "Abort application",
                MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    ::ShowWindow(mWnd, SW_SHOW);
    ::SetForegroundWindow(mWnd);
    ::SetFocus(mWnd);
    ::SetCursorPos(cx, cy);
    prop->m_open = true;

    return true;
}

bool Win32Window::onDestroy() {
    WindowsProperties *prop = getProperties();
    if (nullptr == prop) {
        return false;
    }

    if (!prop->m_open) {
        return false;
    }

    if (!prop->m_fullscreen) {
        ::ChangeDisplaySettings(NULL, 0);
    }

    if (mDC && !::ReleaseDC(mWnd, mDC)) {
        ::MessageBox(NULL, "Cannot release the device context.",
                "Abort application", MB_OK | MB_ICONEXCLAMATION);
        mDC = NULL;
    }

    if (nullptr != mHandleStatusBar) {
        ::DestroyWindow(mHandleStatusBar);
        mHandleStatusBar = nullptr;
    }

    if (mWnd && !::DestroyWindow(mWnd)) {
        ::MessageBox(NULL, "Cannot destroy the window.",
                "Abort application", MB_OK | MB_ICONEXCLAMATION);
        mWnd = NULL;
    }

    if (!::UnregisterClass(prop->m_title.c_str(), mInstance)) {
        ::MessageBox(NULL, "Cannot unregister the application .",
                "Abort application", MB_OK | MB_ICONEXCLAMATION);
        mInstance = NULL;
    }

    prop->m_open = false;

    return true;
}

bool Win32Window::onUpdateProperies() {
    const ui32 flags(AbstractWindow::getFlags());
    if (flags | (ui32)SurfaceFlagType::SF_WinTitleDirty) {
        const String &title = AbstractWindow::getProperties()->m_title;
        ::SetWindowText(getHWnd(), title.c_str());
    }
    AbstractWindow::setFlags(SurfaceFlagType::SF_PropertiesClean);

    return true;
}

void Win32Window::onResize(ui32 x, ui32 y, ui32 w, ui32 h) {
    if (nullptr == mWnd) {
        return;
    }

    ::MoveWindow(mWnd, x, y, w, h, NULL);
    WindowsProperties *props = AbstractWindow::getProperties();
    if (nullptr != props) {
        props->mRect.x1 = x;
        props->mRect.y1 = y;
        props->mRect.width = w;
        props->mRect.height = h;
        props->mRect.x2 = props->mRect.x1 + w;
        props->mRect.y2 = props->mRect.y1 + h;
        AbstractWindow::setProperties(props);
    }
}

} // Namespace Platform
} // Namespace OSRE
