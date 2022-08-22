#include "UIElementsWin32.h"

#include "windows.h"
#include <commctrl.h>
#include <windowsx.h>

namespace OSRE {

bool UIElementsWin32::sInited = false;

bool UIElementsWin32::init() {
    if (sInited) {
        return false;
    }

    // needed for the RichEdit control in the about/help dialog
    LoadLibrary("riched20.dll");
    // load windows common controls library to get XP style
    InitCommonControls();

    sInited = true;

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
