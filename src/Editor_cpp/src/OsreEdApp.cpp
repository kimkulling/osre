#include "OsreEdApp.h"
#include "Modules/InspectorModule/InspectorModule.h"
#include "Modules/ModuleBase.h"
#include <osre/App/AssimpWrapper.h>
#include <osre/App/Entity.h>
#include <osre/Common/TCommand.h>
#include <osre/IO/Directory.h>
#include <osre/IO/Uri.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/PlatformOperations.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/TrackBall.h>
#include <osre/UI/Canvas.h>
#include <osre/UI/Panel.h>
#include <osre/App/Project.h>
#include <osre/Platform/PlatformInterface.h>
#ifdef OSRE_WINDOWS
#  include "Engine/Platform/win32/Win32EventQueue.h"
#  include "Engine/Platform/win32/Win32Window.h"
#endif
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#ifdef OSRE_WINDOWS
#  include <winuser.h>
#  include <windows.h>
#include <commctrl.h>
#include <strsafe.h>
#endif

namespace OSRE {
namespace Editor {

using namespace ::OSRE::App;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Platform;

static const ui32 HorizontalMargin = 2;
static const ui32 VerticalMargin = 2;

#ifdef OSRE_WINDOWS

#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_SAVE 3
#define IDM_FILE_IMPORT 4
#define IDM_FILE_QUIT 5

#define IDM_GETTING_HELP 6
#define IDM_INFO_VERSION 7

#define ID_STATIC 8

HWND hStatic = NULL;


/*void AddMenu(HWND hwnd, HMENU hMenubar, wchar_t *title, MenuEntry *me, size_t numItems) {
    HMENU hMenu = CreateMenu();
    AppendMenuW(hMenu, MF_STRING, IDM_INFO_VERSION, L"&Version");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Info");

    SetMenu(hwnd, hMenubar);
}
*/
#endif // OSRE_WINDOWS

OsreEdApp::OsreEdApp(int argc, char *argv[]) :
        AppBase(argc, (const char **)argv, "api", "The render API"),
        mCamera(nullptr),
        m_model(),
        m_transformMatrix(),
        m_modelNode(),
        mTrackBall(nullptr),
        mProject(nullptr) {
    // empty
}

OsreEdApp::~OsreEdApp() {
    // empty
}

bool OsreEdApp::onCreate() {
    if (!AppBase::onCreate()) {
        return false;
    }

    registerModule(new InspectorModule(this));

    AppBase::setWindowsTitle("OSRE ED!");

    auto *w = (Win32Window *)getRootWindow();
    AbstractPlatformEventQueue *queue = PlatformInterface::getInstance()->getPlatformEventHandler();
    if (nullptr != w && nullptr != queue) {
        w->beginMenu();
        MenuEntry FileMenu[8] = {
            { MF_STRING, IDM_FILE_NEW, L"&New", MenuFunctor::Make(this, &OsreEdApp::newProjectCmd) },
            { MF_STRING, IDM_FILE_OPEN, L"&Open Project", MenuFunctor::Make(this, &OsreEdApp::loadProjectCmd) },
            { MF_STRING, IDM_FILE_SAVE, L"&Save Project", MenuFunctor::Make(this, &OsreEdApp::saveProjectCmd) },
            { MF_SEPARATOR, 0, nullptr },
            { MF_STRING, IDM_FILE_IMPORT, L"&Import Asset", MenuFunctor::Make(this, &OsreEdApp::importAssetCmd) },
            { MF_SEPARATOR, 0, nullptr },
            { MF_STRING, IDM_FILE_QUIT, L"&Quit", MenuFunctor::Make(this, &OsreEdApp::quitEditorCmd) },
        };
        w->addSubMenues(nullptr, queue, L"File", FileMenu, 8);

        MenuEntry InfoMenu[2] = {
            { MF_STRING, IDM_GETTING_HELP, L"&Getting Help", MenuFunctor::Make(this, &OsreEdApp::gettingHelpCmd) },
            { MF_STRING, IDM_INFO_VERSION, L"&Version", MenuFunctor::Make(this, &OsreEdApp::showVersionCmd) }
        };
        w->addSubMenues(nullptr, queue, L"&Info", InfoMenu, 2);

        w->endMenu();
    }

    AppBase::getRenderBackendService()->enableAutoResizing(false);

    return true;
}

void OsreEdApp::loadAsset(const IO::Uri &modelLoc) {
    AssimpWrapper assimpWrapper(*getIdContainer(), getActiveWorld());
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

    Rect2ui windowsRect;
    rootWindow->getWindowsRect(windowsRect);
    World *world = getActiveWorld();
    mCamera = world->addCamera("camera_1");
    mTrackBall = new Scene::TrackBall("trackball", windowsRect.getWidth(), windowsRect.getHeight(), *getIdContainer());
    mCamera->setProjectionParameters(60.f, (f32)windowsRect.m_width, (f32)windowsRect.m_height, 0.01f, 1000.f);
    Entity *entity = assimpWrapper.getEntity();

    world->addEntity(entity);
    mCamera->observeBoundingBox(entity->getAABB());
    m_modelNode = entity->getNode();

    const std::string &model = modelLoc.getResource();
    getRootWindow()->setWindowsTitle("Model " + model);
}

void OsreEdApp::newProjectCmd(ui32, void *) {
    mProject = new App::Project();
    mProject->create("New project", 0, 1);
    const String &projectName = mProject->getProjectName();
    
    AppBase::setWindowsTitle("OSRE ED!" + String(" Project: ") + projectName);
}

void OsreEdApp::loadProjectCmd(ui32, void *) {
}

void OsreEdApp::saveProjectCmd(ui32, void *) {
}

void OsreEdApp::importAssetCmd(ui32, void *) {
    IO::Uri modelLoc;
    PlatformOperations::getFileOpenDialog("*", modelLoc);
    if (modelLoc.isValid()) {
        loadAsset(modelLoc);
    }
}

void OsreEdApp::quitEditorCmd(ui32, void *) {
    DlgResults result;
    PlatformOperations::getDialog("Really quit?", "Do you really quite OSRE-Ed?", Platform::PlatformOperations::DlgButton_YesNo, result);
    if (result == Platform::DlgResults::DlgButtonRes_Yes) {
        AppBase::requestShutdown();
    }
}

void OsreEdApp::gettingHelpCmd(ui32 cmdId, void *data) {

}

void OsreEdApp::showVersionCmd(ui32 cmdId, void *data) {
    DlgResults res;
    PlatformOperations::getDialog("Version Info", "OSRE Version 0.0.1", IDOK, res);
}

bool OsreEdApp::registerModule(ModuleBase *mod) {
    if (nullptr == mod) {
        return false;
    }

    if (nullptr == findModule(mod->getName())) {
        mModules.add(mod);
    }

    return true;
}

ModuleBase *OsreEdApp::findModule( const String &name ) const {
    if (name.empty()) {
        return nullptr;
    }

    for (ui32 i = 0; i < mModules.size(); ++i) {
        if (name == mModules[i]->getName()) {
            return mModules[i];
        }
    }

    return nullptr;
}

bool OsreEdApp::unregisterModule( ModuleBase *mod ) {
    return true;
}

void OsreEdApp::onUpdate() {
    if (AppBase::isKeyPressed(Platform::KEY_O)) {
        IO::Uri modelLoc;
        PlatformOperations::getFileOpenDialog("*", modelLoc);
        if (modelLoc.isValid()) {
            loadAsset(modelLoc);
        }
    }

    for (ui32 i = 0; i < mModules.size(); ++i) {
        ModuleBase *module = mModules[i];
        module->update();
        module->render();
    }
    AppBase::onUpdate();
}

bool OsreEdApp::onDestroy() {
    return true;
}

} // namespace Editor
} // namespace OSRE
