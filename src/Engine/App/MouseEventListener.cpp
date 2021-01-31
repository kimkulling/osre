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
#include <osre/UI/Canvas.h>

namespace OSRE {
namespace App {

static const c8 *Tag = "MouseEventListener";

using namespace OSRE::Common;
using namespace OSRE::UI;
using namespace OSRE::Platform;

MouseEventListener::MouseEventListener() :
        OSEventListener("App/MouseEventListener"),
        m_uiCanvas() {
    // empty
}

MouseEventListener::~MouseEventListener() {
    // empty
}

void MouseEventListener::setCanvas(Canvas *screen) {
    m_uiCanvas = screen;
}

Canvas *MouseEventListener::getCanvas() const {
    return m_uiCanvas.getPtr();
}

void MouseEventListener::onOSEvent(const Event &osEvent, const EventData *data) {
    osre_debug(Tag, "listener called");

    if (m_uiCanvas.isValid()) {
        if (osEvent == MouseButtonDownEvent) {
            MouseButtonEventData *mouseBtnData((MouseButtonEventData *)data);
            const Point2ui pt(mouseBtnData->m_AbsX, mouseBtnData->m_AbsY);
            m_uiCanvas->mouseDown(pt, nullptr);
        }
    }
}

} // Namespace App
} // Namespace OSRE
