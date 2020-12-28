#pragma once

#include "Modules/ModuleBase.h"

namespace OSRE {

namespace UI {
    class Panel;
}

namespace Editor {

class LogModule : public ModuleBase {
public:
    LogModule(App::AppBase *parentApp);
    ~LogModule() override;

protected:
    bool onLoad() override;
    bool onUnload() override;
    void onUpdate() override;

private:
    UI::Panel *mPanel;
};

} // namespace Editor
} // namespace OSRE
