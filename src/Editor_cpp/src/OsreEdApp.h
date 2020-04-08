#pragma once

#include <osre/App/AppBase.h>

namespace OSRE {

namespace UI {
class Canvas;
class Panel;
}

namespace Editor {

class OsreEdApp : public App::AppBase {
public:
    OsreEdApp(int argc, char *argv[]);
    ~OsreEdApp() override;

protected:
    bool onCreate() override;
    void onUpdate() override;
    bool onDestroy() override;

private:
    UI::Canvas *m_canvas;
    UI::Panel *m_logPanel;
    UI::Panel *m_modelPanel;
};

} // namespace Editor
} // namespace OSRE
