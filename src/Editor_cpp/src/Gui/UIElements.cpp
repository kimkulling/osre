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
#include "UIElements.h"
#include <osre/Scene/MeshBuilder.h>

#include <commoncontrols.h>
#include <winuser.h>

namespace OSRE {

using namespace ::OSRE::RenderBackend;

ProgressBar *UIElements::createProgressBar(int id, HWND hWnd, const Rect2ui &dimension) {
    ProgressBar *pb = new ProgressBar;
    pb->mPlatformData.mHWnd = CreateWindowEx(
            0,
            PROGRESS_CLASS,
            (LPSTR)NULL,
            WS_VISIBLE | WS_CHILD,
            dimension.getX1(),
            dimension.getY1(),
            dimension.getWidth(),
            dimension.getHeight(),
            hWnd,
            (HMENU)id,
            (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),
            NULL);

    if (!pb->mPlatformData.mHWnd) {
        MessageBox(NULL, "Progress Bar Failed.", "Error", MB_OK | MB_ICONERROR);
        return nullptr;
    }
    pb->mRange=100;
    pb->mCurrent = 0;
    
    SendMessage(pb->mPlatformData.mHWnd, PBM_SETRANGE, 0, MAKELPARAM(0, 99));
    SendMessage(pb->mPlatformData.mHWnd, PBM_SETSTEP, (WPARAM)1, 0);

    pb->mParent = nullptr;
    pb->mRect = dimension;

    return pb;
}

void UIElements::updateProgressBar(ProgressBar *pb, ui32 step) {
    SendMessage(pb->mPlatformData.mHWnd, PBM_SETPOS, (WPARAM)step, 0);
}
    
void UIElements::deleteProgressBar(ProgressBar *pb) {
    ::CloseWindow(pb->mPlatformData.mHWnd);
    delete pb;
}

} // namespace OSRE
