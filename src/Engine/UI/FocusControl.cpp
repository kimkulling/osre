#include <osre/UI/FocusControl.h>
#include <osre/UI/Widget.h>
#include <osre/Common/Event.h>
#include <osre/Platform/PlatformInterface.h>

namespace OSRE {
namespace UI {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;

FocusControl::FocusControl() 
: m_widget( nullptr ) {
    // empty
}

FocusControl::~FocusControl() {
    m_widget = nullptr;
}

void FocusControl::setInputFocusWidget( Widget *widget ) {
    m_widget = widget;
}

Widget *FocusControl::getInputFocusWidget() const {
    return m_widget;
}

void FocusControl::onEvent( const Event &ev, const EventData *data ) {
    if (nullptr == m_widget) {
        return;
    }

    if (ev == KeyboardButtonDownEvent ) {
        if (nullptr == data) {
            return;
        }
        KeyboardButtonEventData *keyBtnData = ( KeyboardButtonEventData * )data;
        m_widget->keyPressed( keyBtnData->m_key );
    } else if (ev == KeyboardButtonUpEvent) {
        if (nullptr == data) {
            return;
        }
        KeyboardButtonEventData *keyBtnData = ( KeyboardButtonEventData * )data;
        m_widget->keyReleased( keyBtnData->m_key );
    }
}

} // Namespace UI
} // Namespace OSRE
