#include "OsreEdApp.h"

#include <osre/Platform/AbstractWindow.h>
#include <osre/UI/Canvas.h>
#include <osre/UI/Panel.h>

namespace OSRE {
namespace Editor {

OsreEdApp::OsreEdApp(int argc, char *argv[]) :
        AppBase(argc, (const char **)argv, "api", "The render API"),
        m_canvas(nullptr),
        m_logPanel( nullptr ),
        m_modelPanel( nullptr ){
    // empty
}

OsreEdApp::~OsreEdApp() {
    // empty
}

bool OsreEdApp::onCreate() {
    Rect2ui r;
    AppBase::getRootWindow()->getWindowsRect(r);
    
    m_canvas = new UI::Canvas("main", r.m_x1, r.m_y1, r.m_width, r.m_height);
    m_logPanel = new UI::Panel("log_panel", m_canvas);
    m_modelPanel = new UI::Panel("model_panel", m_canvas);
    return true;
}

void OsreEdApp::onUpdate() {
}

bool OsreEdApp::onDestroy() {
    return true;
}

} // namespace Editor
} // namespace OSRE
