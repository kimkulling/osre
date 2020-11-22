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
        mMouseState(),
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

    mMouseState.MouseMoved = false;
    if (osEvent == MouseButtonDownEvent) {
        MouseButtonEventData *btnData = (MouseButtonEventData *)data;
        if (btnData->m_Button == MouseButtonEventData::LeftButton) {
            mMouseState.LeftButtonPressed = true;
        }
        if (btnData->m_Button == MouseButtonEventData::MiddleButton) {
            mMouseState.MittleButtonPressed = true;
        }
        if (btnData->m_Button == MouseButtonEventData::RightButton) {
            mMouseState.RightButtonPressed = true;
        }
    }
    if (osEvent == MouseButtonUpEvent) {
        MouseButtonEventData *btnData = (MouseButtonEventData *)data;
        if (btnData->m_Button == MouseButtonEventData::LeftButton) {
            mMouseState.LeftButtonPressed = false;
        }
        if (btnData->m_Button == MouseButtonEventData::MiddleButton) {
            mMouseState.MittleButtonPressed = false;
        }
        if (btnData->m_Button == MouseButtonEventData::RightButton) {
            mMouseState.RightButtonPressed = false;
        }
    }

    if (osEvent == MouseMoveEvent || mMouseState.LeftButtonPressed) {
        MouseMoveEventData *moveData = (MouseMoveEventData *)data;
        mMouseState.mLastMouseMove.setX(moveData->m_absX);
        mMouseState.mLastMouseMove.setY(moveData->m_absY);
        mMouseState.MouseMoved = true;
    }

    if (m_uiCanvas.isValid()) {
        if (osEvent == MouseButtonDownEvent) {
            MouseButtonEventData *mouseBtnData((MouseButtonEventData *)data);
            const Point2ui pt(mouseBtnData->m_AbsX, mouseBtnData->m_AbsY);
            m_uiCanvas->mouseDown(pt, nullptr);
        }
    }
}

const MouseState &MouseEventListener::getMouseState() const {
    return mMouseState;
}

} // Namespace App
} // Namespace OSRE
