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

void MouseEventListener::setCanvas(UI::Canvas *screen) {
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
