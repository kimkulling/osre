#include "OsreEdApp.h"

#include <osre/Platform/AbstractWindow.h>
#include <osre/UI/Canvas.h>
#include <osre/UI/Panel.h>

namespace OSRE {
namespace Editor {

OsreEdApp::OsreEdApp(int argc, char *argv[]) :
        AppBase(argc, (const char **)argv, "api", "The render API"),
        m_canvas(nullptr),
        m_mainPanel(nullptr),
        m_logPanel(nullptr),
        m_modelPanel(nullptr) {
    // empty
}

OsreEdApp::~OsreEdApp() {
    // empty
}

static const ui32 HorizontalMargin = 2;
static const ui32 VerticalMargin = 2;

bool OsreEdApp::onCreate() {
    if (!AppBase::onCreate()) {
        return false;
    }

    Rect2ui r;
    AppBase::getRootWindow()->getWindowsRect(r);
    m_canvas = AppBase::createScreen("OsreEd");
    m_canvas->setRect(r);

    m_mainPanel = new UI::Panel("main_panel", m_canvas);
    m_mainPanel->setRect(r);
    m_logPanel = new UI::Panel("log_panel", m_mainPanel);
    m_logPanel->setHeadline("Log");
    m_logPanel->setRect(r.getX1() + HorizontalMargin, r.getY1() + r.getHeight() / 3, r.getWidth() - 2 * HorizontalMargin, r.getHeight() - (r.getY1() + r.getHeight() / 3)-2*VerticalMargin);
    m_modelPanel = new UI::Panel("model_panel", m_mainPanel);
    m_modelPanel->setHeadline("Model");

    return true;
}

void OsreEdApp::onUpdate() {
}

bool OsreEdApp::onDestroy() {
    return true;
}

} // namespace Editor
} // namespace OSRE
