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
#include "UIElementsWin32.h"

#include <osre/Debugging/osre_debugging.h>

#include "windows.h"
#include <commctrl.h>
#include <windowsx.h>

namespace OSRE {

bool UIElementsWin32::sInited = false;
HMODULE lib = nullptr;

bool UIElementsWin32::init() {
    if (sInited) {
        return false;
    }

    // needed for the RichEdit control in the about/help dialog
    lib = LoadLibrary("riched20.dll");
    osre_assert(lib != nullptr);

    // load windows common controls library to get XP style
    InitCommonControls();

    sInited = true;

    return true;
}

bool UIElementsWin32::release() {
    if (!sInited) {
        return false;
    }
    FreeLibrary(lib);
    lib = nullptr;

    return true;
}

void UIElementsWin32::getMonitorResolution(ui32 &width, ui32 &heigt) {
#ifdef OSRE_WINDOWS
    width = GetSystemMetrics(SM_CXSCREEN);
    heigt = GetSystemMetrics(SM_CYSCREEN);
#else
    width = heigt = 0;
#endif
}

} // namespace OSRE
