/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/Windows/MinWindows.h>
#include <cppcore/Container/TArray.h>

#include "Engine/Platform/win32/Win32EventQueue.h"

namespace OSRE {
namespace Platform {

/// @brief  This struct is used to define a single menu entry.
/// To  add an entry you have to add The menu-type from the WIN32-API, its id, the name as a unicode
/// string and a functor, which contains the command functor.
struct MenuEntry {
    i32 Type; /// The menu type
    i32 Id; /// The menu id, must be zero if no command shall be coupled
    wchar_t *Name; /// The Menu item name
    MenuFunctor Func; /// The functor which stores the command for the menu entry.
};

struct OSRE_EXPORT StatusBarContent {
    CPPCore::TArray<ui32> StatusBarWidths;
    CPPCore::TArray<String> StatusBarTexts;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the windows-surface API by using Win32-API.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Win32Window : public AbstractWindow {
public:
    /// The class constructor.
    Win32Window(WindowsProperties *properties);
    /// The class destructor, virtual.
    ~Win32Window() override;
    ///
    void showWindow(ShowState showState) override;
    /// Will set the windows title.
    void setWindowsTitle(const String &title) override;
    /// Will set the windows mouse cursor.
    void setWindowsMouseCursor(DefaultMouseCursorType ct) override;
    /// Returns the windows handle.
    HWND getHWnd() const;
    /// Returns the device context.
    HDC getDeviceContext() const;
    /// Returns the module handle of the window.
    HINSTANCE getModuleHandle() const;
    /// Will return the menu handle
    HMENU getMenuHandle();
    /// Will begin to add a new menu to the window.
    HMENU beginMenu();
    /// Will add a sub-menu.
    void addSubMenues(HMENU parent, AbstractPlatformEventQueue *queue, wchar_t *title, MenuEntry *menu_entries, size_t numItems);
    /// Will end the menu creation.
    void endMenu();
    ///
    HWND createStatusBar(UINT ResID, ui32 numFields);
    ///
    HWND getStatusBarHandle() const;
    ///
    void setStatusText(ui32 index, const char *Text);

protected:
    /// The create callback implementation.
    bool onCreate() override;
    /// The destroy callback implementation.
    bool onDestroy() override;
    /// The updateProperties callback implementation.
    bool onUpdateProperies() override;
    /// The resize callback implementation.
    void onResize(ui32 x, ui32 y, ui32 w, ui32 h) override;

private:
    HINSTANCE mInstance;
    HWND mWnd, mHandleStatusBar;
    HDC mDC;
    HMENU mMenu;
    StatusBarContent mStatusBarContent;
    bool mMenuCreateState;
};

} // Namespace Platform
} // Namespace OSRE
