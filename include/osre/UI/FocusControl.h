#pragma once

#include <osre/UI/UICommon.h>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace Common {
    struct Event;
    struct EventData;
}

namespace UI {

class Widget;

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class FocusControl {
public:
    FocusControl();
    ~FocusControl();
    void setInputFocusWidget( Widget *widget );
    Widget *getInputFocusWidget() const;
    void onEvent( const Common::Event &ev, const Common::EventData *data );

private:
    Widget *m_widget;
};

} // Namespace UI
} // Namespace OSRE

