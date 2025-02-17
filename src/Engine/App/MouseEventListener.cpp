/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "MouseEventListener.h"
#include "Platform/PlatformInterface.h"

namespace OSRE {
namespace App {

using namespace OSRE::Common;
using namespace OSRE::Platform;

MouseEventListener::MouseEventListener() : OSEventListener("App/MouseEventListener") {
    // empty
}

void MouseEventListener::onOSEvent(const Event &ev, const EventData *data) {
    mMouseInputState.LastX = mMouseInputState.AbsX;
    mMouseInputState.LastY = mMouseInputState.AbsY;
    if (!(ev == MouseButtonDownEvent) && !(ev == MouseButtonUpEvent)) {
        MouseMoveEventData *evData = (MouseMoveEventData *)data;
        mMouseInputState.AbsX = evData->m_absX;
        mMouseInputState.AbsY = evData->m_absY;
    } else {
        MouseButtonEventData *mouseEventData = (MouseButtonEventData *)data;
        mMouseInputState.AbsX = mouseEventData->m_AbsX;
        mMouseInputState.AbsY = mouseEventData->m_AbsY;
        if (mouseEventData->m_Button == MouseButtonEventData::LeftButton) {
            if (mouseEventData->m_Pressed) {
                mMouseInputState.MouseButtonState.setBit(LeftButton);
            } else {
                mMouseInputState.MouseButtonState.clearBit(LeftButton);
            }
        }

        if (mouseEventData->m_Button == MouseButtonEventData::MiddleButton) {
            if (mouseEventData->m_Pressed) {
                mMouseInputState.MouseButtonState.setBit(MiddleButton);
            } else {
                mMouseInputState.MouseButtonState.clearBit(MiddleButton);
            }
        }

        if (mouseEventData->m_Button == MouseButtonEventData::RightButton) {
            if (mouseEventData->m_Pressed) {
                mMouseInputState.MouseButtonState.setBit(RightButton);
            } else {
                mMouseInputState.MouseButtonState.clearBit(RightButton);
            }
        }
    }
    mMouseInputState.RelX = mMouseInputState.LastX - mMouseInputState.AbsX;
    mMouseInputState.RelY = mMouseInputState.LastY - mMouseInputState.AbsY;
}

} // Namespace App
} // Namespace OSRE
