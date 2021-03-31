#pragma once

#include "Modules/ModuleBase.h"

namespace OSRE {

namespace Editor {

class LogView;

class LogModule : public ModuleBase {
public:
    LogModule(App::AppBase *parentApp);
    ~LogModule() override;

protected:
    bool onLoad() override;
    bool onUnload() override;
    void onUpdate() override;

private:
    LogView *mLogView;
};

} // namespace Editor
} // namespace OSRE
