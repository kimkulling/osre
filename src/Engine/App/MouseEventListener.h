#pragma once

#include <osre/Common/osre_common.h>
#include <osre/Common/TObjPtr.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/app/AppBase.h>

namespace OSRE {

namespace UI {
    class Canvas;
}

namespace App {

class MouseEventListener : public Platform::OSEventListener {
public:
    MouseEventListener();
    ~MouseEventListener() override;
    UI::Canvas *getCanvas() const;
    void onOSEvent(const Common::Event &osEvent, const Common::EventData *data) override;
    void setCanvas(UI::Canvas *screen);

private:
    Common::TObjPtr<UI::Canvas> m_uiCanvas;
};

} // Namespace App
} // Namespace OSRE
