#include "OsreEdApp.h"
#include "Modules/ModuleBase.h"

#include <osre/Platform/AbstractWindow.h>
#include <osre/UI/Canvas.h>
#include <osre/UI/Panel.h>
#include <osre/IO/Directory.h>

namespace OSRE {
namespace Editor {

static const ui32 HorizontalMargin = 2;
static const ui32 VerticalMargin = 2;

OsreEdApp::UiScreen::UiScreen() :
        m_canvas( nullptr ),
        m_mainPanel( nullptr ),
        m_logPanel( nullptr ),
        m_modelPanel( nullptr ) {
    // empty
}

OsreEdApp::OsreEdApp(int argc, char *argv[]) :
        AppBase(argc, (const char **)argv, "api", "The render API"),
        mUiScreen(),
        mModuleArray(),
        mModulePathArray() {
    // empty
}

OsreEdApp::~OsreEdApp() {
    // empty
}

bool OsreEdApp::addModulePath( const String &path ) {
    if (path.empty()) {
        return false;
    }

    if (mModulePathArray.find(path) == nullptr) {
        mModulePathArray.add(path);
    }

    return true;
}

bool OsreEdApp::registerModule( ModuleBase *mod ) {
    if (nullptr == mod) {
        return false;
    }

    if (nullptr == mModuleArray.find(mod) ) {
        mModuleArray.add(mod);
    }

    return true;
}

bool OsreEdApp::loadModules() {
    // Load registered modules
    for (size_t i = 0; i < mModuleArray.size(); ++i) {
        ModuleBase *mod = mModuleArray[i];
        if (!mod->load()) {
            return false;
        }
    }

    return true;
}

bool OsreEdApp::onCreate() {
    if (!AppBase::onCreate()) {
        return false;
    }

    Rect2ui r;
    AppBase::getRootWindow()->getWindowsRect(r);
    mUiScreen.m_canvas = AppBase::createScreen("OsreEd");
    mUiScreen.m_canvas->setRect(r);

    mUiScreen.m_mainPanel = new UI::Panel("main_panel", mUiScreen.m_canvas);
    mUiScreen.m_mainPanel->setRect(r);
    mUiScreen.m_logPanel = new UI::Panel("log_panel", mUiScreen.m_mainPanel);
    mUiScreen.m_logPanel->setHeadline("Log");
    mUiScreen.m_logPanel->setRect(r.getX1() + HorizontalMargin, r.getY1() + r.getHeight() / 3, r.getWidth() - 2 * HorizontalMargin, r.getHeight() - (r.getY1() + r.getHeight() / 3) - 2 * VerticalMargin);
    mUiScreen.m_modelPanel = new UI::Panel("model_panel", mUiScreen.m_mainPanel);
    mUiScreen.m_modelPanel->setHeadline("Model");

    return true;
}

void OsreEdApp::onUpdate() {
    for (ui32 i = 0; i < mModuleArray.size(); ++i) {
        mModuleArray[i]->update();
    }
}

bool OsreEdApp::onDestroy() {
    return true;
}

} // namespace Editor
} // namespace OSRE
