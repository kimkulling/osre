#include "OsreEdApp.h"
#include "Modules/ModuleBase.h"
#include "Modules/InspectorModule/InspectorModule.h"
#include <osre/Platform/AbstractWindow.h>
#include <osre/App/AssimpWrapper.h>
#include <osre/UI/Canvas.h>
#include <osre/UI/Panel.h>
#include <osre/IO/Directory.h>
#include <osre/IO/Uri.h>
#include <osre/App/Entity.h>

namespace OSRE {
namespace Editor {

using namespace ::OSRE::App;
using namespace ::OSRE::RenderBackend;

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
        if (!mod->load(this)) {
            return false;
        }
    }

    return true;
}

UI::Panel *OsreEdApp::getRootPanel() const {
    return mUiScreen.m_mainPanel;
}

bool OsreEdApp::onCreate() {
    if (!AppBase::onCreate()) {
        return false;
    }

    registerModule(new InspectorModule());
    loadModules();

    Rect2ui r;
    AppBase::getRootWindow()->getWindowsRect(r);
    /*mUiScreen.m_canvas = AppBase::createScreen("OsreEd");
    mUiScreen.m_canvas->setRect(r);

    mUiScreen.m_mainPanel = new UI::Panel("main_panel", mUiScreen.m_canvas);
    mUiScreen.m_mainPanel->setRect(r);
    mUiScreen.m_logPanel = new UI::Panel("log_panel", mUiScreen.m_mainPanel);
    mUiScreen.m_logPanel->setHeadline("Log");
    mUiScreen.m_logPanel->setRect(r.getX1() + HorizontalMargin, r.getY1() + r.getHeight() / 3, 
        r.getWidth() - 2 * HorizontalMargin, r.getHeight() - (r.getY1() + r.getHeight() / 3) - 2 * VerticalMargin);
    mUiScreen.m_modelPanel = new UI::Panel("model_panel", mUiScreen.m_mainPanel);
    mUiScreen.m_modelPanel->setHeadline("Model");*/

    return true;
}

void OsreEdApp::loadAsset(const IO::Uri &modelLoc) {
    AssimpWrapper assimpWrapper(*getIdContainer());
    if (!assimpWrapper.importAsset(modelLoc, 0)) {
        return;
    }

    RenderBackendService *rbSrv = getRenderBackendService();
    if (nullptr == rbSrv) {
        return;
    }
    Platform::AbstractWindow *rootWindow = getRootWindow();
    if (nullptr == rootWindow) {
        return;
    }
    m_stage = AppBase::createStage("ModelLoading");
    AppBase::setActiveStage(m_stage);
    m_view = m_stage->addView("default_view", nullptr);
    AppBase::setActiveView(m_view);

    Rect2ui windowsRect;
    rootWindow->getWindowsRect(windowsRect);
    m_view->setProjectionParameters(60.f, (f32)windowsRect.m_width, (f32)windowsRect.m_height, 0.01f, 1000.f);
    Entity *entity = assimpWrapper.getEntity();

    World *world = getActiveWorld();
    world->addEntity(entity);
    m_view->observeBoundingBox(entity->getAABB());
    m_modelNode = entity->getNode();
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
