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
#include "MouseEventListener.h"
#include <osre/Platform/PlatformInterface.h>

namespace OSRE {
namespace App {

using namespace OSRE::Common;
using namespace OSRE::Platform;

MouseEventListener::MouseEventListener() :
        OSEventListener("App/MouseEventListener"),
        mRelX(0),
        mRelY(0),
        mAbsX(0),
        mAbsY(0),
        mLastX(0),
        mLastY(0),
        mMouseButtonState() {
    // empty
}

MouseEventListener::~MouseEventListener() {
    // empty
}

static void clearCoords(i32 &relX, i32 &relY, i32 &absX, i32 &absY) {
    relX = relY = 0;
    absX = absY = 0;
}

void MouseEventListener::onOSEvent(const Event &ev, const EventData *data) {
    mLastX = mAbsX;
    mLastY = mAbsY;
    if (!(ev == MouseButtonDownEvent) && !(ev == MouseButtonUpEvent)) {
        MouseMoveEventData *evData = (MouseMoveEventData *)data;
        mAbsX = evData->m_absX;
        mAbsY = evData->m_absY;
    } else {
        MouseButtonEventData *mouseEventData = (MouseButtonEventData *)data;
        mAbsX = mouseEventData->m_AbsX;
        mAbsY = mouseEventData->m_AbsY;
        if (mouseEventData->m_Button == MouseButtonEventData::LeftButton) {
            if (mouseEventData->m_Pressed) {
                mMouseButtonState.setBit(LeftButton);
            } else {
                mMouseButtonState.clearBit(LeftButton);
            }
        }

        if (mouseEventData->m_Button == MouseButtonEventData::MiddleButton) {
            if (mouseEventData->m_Pressed) {
                mMouseButtonState.setBit(MiddleButton);
            } else {
                mMouseButtonState.clearBit(MiddleButton);
            }
        }

        if (mouseEventData->m_Button == MouseButtonEventData::RightButton) {
            if (mouseEventData->m_Pressed) {
                mMouseButtonState.setBit(RightButton);
            } else {
                mMouseButtonState.clearBit(RightButton);
            }
        }
    }
    mRelX = mLastX - mAbsX;
    mRelY = mLastY - mAbsY;
}

} // Namespace App
} // Namespace OSRE
