#pragma once

#include <osre/App/AppBase.h>
#include <cppcore/Container/TArray.h>

namespace OSRE {

namespace UI {
class Canvas;
class Panel;
}

namespace Editor {

class ModuleBase;

class OsreEdApp : public App::AppBase {
    using ModuleArray = ::CPPCore::TArray<ModuleBase *>;
    using ModulePathArray = ::CPPCore::TArray<String>;
public:
    OsreEdApp(int argc, char *argv[]);
    ~OsreEdApp() override;
    bool addModulePath(const String &path);
    bool registerModule(ModuleBase *mod);
    bool loadModules();

protected:
    bool onCreate() override;
    void onUpdate() override;
    bool onDestroy() override;

private:
    struct UiScreen {
        UI::Canvas *m_canvas;
        UI::Panel  *m_mainPanel;
        UI::Panel  *m_logPanel;
        UI::Panel  *m_modelPanel;

        UiScreen();
    };
    UiScreen mUiScreen;
    ModuleArray mModuleArray;
    ModulePathArray mModulePathArray;
};

} // namespace Editor
} // namespace OSRE
