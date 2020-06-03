#pragma once

#include "Modules/ModuleBase.h"

namespace OSRE {

namespace UI {
class Panel;

}

namespace Editor {

class LogModule : public ModuleBase {
public:
    LogModule();
    ~LogModule() override;

protected:
    virtual bool onLoad(OsreEdApp *parent);
    virtual bool onUnload(OsreEdApp *parent);
    virtual void onUpdate();

private:
    UI::Panel *mPanel;
};

} // namespace Editor
} // namespace OSRE
