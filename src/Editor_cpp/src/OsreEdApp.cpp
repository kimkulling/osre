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
#include <osre/app/Project.h>
#include <osre/Platform/PlatformInterface.h>

#include "Engine/Platform/win32/Win32EventQueue.h"
#include "Engine/Platform/win32/Win32Window.h"

#include <winuser.h>
#include <windows.h>
#include <commctrl.h>
#include <strsafe.h>

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

#define IDM_INFO_VERSION 6

#define ID_STATIC 7

HWND hStatic = NULL;

void AddFileMenus(HWND hwnd) {
    HMENU hMenubar;
    HMENU hMenu;

    hMenubar = CreateMenu();
    hMenu = CreateMenu();

    AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN, L"&Open Project");
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_SAVE, L"&Save Project");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_IMPORT, L"&Import Asset");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&File");

    hMenu = CreateMenu();
    AppendMenuW(hMenu, MF_STRING, IDM_INFO_VERSION, L"&Version");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Info");

    SetMenu(hwnd, hMenubar);
}

void SetFPSText(HWND hwnd, ui32 fps) {
    std::stringstream stream;
    stream << "FPS: " << fps;
    RECT r;
    
    r.left = 10;
    r.right = 100;
    r.top = 10;
    r.bottom = 20;
    UINT format = 0;
    
    //DrawTextEx(GetDC(hwnd), stream.str().c_str(), stream.str().size(), &r, format, NULL);
}

#endif // OSRE_WINDOWS

OsreEdApp::OsreEdApp(int argc, char *argv[]) :
        AppBase(argc, (const char **)argv, "api", "The render API"),
        mModuleArray(),
        mModulePathArray(),
        mCamera(nullptr),
        m_angle(0.f),
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

bool OsreEdApp::addModulePath(const String &path) {
    if (path.empty()) {
        return false;
    }

    if (mModulePathArray.find(path) == nullptr) {
        mModulePathArray.add(path);
    }

    return true;
}

bool OsreEdApp::registerModule(ModuleBase *mod) {
    if (nullptr == mod) {
        return false;
    }

    if (nullptr == mModuleArray.find(mod)) {
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

bool OsreEdApp::onCreate() {
    if (!AppBase::onCreate()) {
        return false;
    }

    registerModule(new InspectorModule());
    //loadModules();

    AppBase::setWindowsTitle("OSRE ED!");

    Platform::Win32Window *w = (Platform::Win32Window *)getRootWindow();
    if (nullptr != w) {
        AddFileMenus(w->getHWnd());
        SetFPSText(w->getHWnd(), 60);
        Platform::AbstractPlatformEventQueue *queue = PlatformInterface::getInstance()->getPlatformEventHandler();
        if (queue != nullptr) {
            queue->registerMenuCommand(IDM_FILE_NEW, Platform::MenuFunctor::Make(this, &OsreEdApp::newProject));
            queue->registerMenuCommand(IDM_FILE_OPEN, Platform::MenuFunctor::Make(this, &OsreEdApp::loadProject));
            queue->registerMenuCommand(IDM_FILE_SAVE, Platform::MenuFunctor::Make(this, &OsreEdApp::saveProject));
            queue->registerMenuCommand(IDM_FILE_IMPORT, Platform::MenuFunctor::Make(this, &OsreEdApp::importAsset));
            queue->registerMenuCommand(IDM_FILE_QUIT, Platform::MenuFunctor::Make(this, &OsreEdApp::quitEditor));
        }
    }

    AppBase::getRenderBackendService()->setBehaviour(false);

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
    mTrackBall = new Scene::TrackBall("trackball", windowsRect.getWidth(), windowsRect.getHeight());
    mCamera->setProjectionParameters(60.f, (f32)windowsRect.m_width, (f32)windowsRect.m_height, 0.01f, 1000.f);
    Entity *entity = assimpWrapper.getEntity();

    world->addEntity(entity);
    mCamera->observeBoundingBox(entity->getAABB());
    m_modelNode = entity->getNode();

    std::string model = modelLoc.getResource();
    getRootWindow()->setWindowsTitle("Model " + model);
}

void OsreEdApp::newProject(ui32, void *) {
    mProject = new App::Project();
    mProject->create("New project", 0, 1);
    const String &projectName = mProject->getProjectName();
    AppBase::setWindowsTitle("OSRE ED!" + String(" Project: ") + projectName);
}

void OsreEdApp::loadProject(ui32, void *) {
}

void OsreEdApp::saveProject(ui32, void *) {
}

void OsreEdApp::importAsset(ui32, void *) {
    IO::Uri modelLoc;
    Platform::PlatformOperations::getFileOpenDialog("*", modelLoc);
    if (modelLoc.isValid()) {
        loadAsset(modelLoc);
    }
}

void OsreEdApp::quitEditor(ui32, void *) {
    Platform::PlatformOperations::DlgResults result;
    Platform::PlatformOperations::getDialog("Really quit?", "Do you really quite OSRE-Ed?", Platform::PlatformOperations::DlgButton_YesNo, result);
    if (result == Platform::PlatformOperations::DlgButtonRes_Yes) {
        AppBase::requestShutdown();
    }
}

void OsreEdApp::onUpdate() {
    if (AppBase::isKeyPressed(Platform::KEY_O)) {
        IO::Uri modelLoc;
        Platform::PlatformOperations::getFileOpenDialog("*", modelLoc);
        if (modelLoc.isValid()) {
            loadAsset(modelLoc);
        }
    }
    i32 x = 0, y = 0;
    if (AppBase::isMouseMoved(x, y)) {
        //    mTrackBall->
    }

    /*for (ui32 i = 0; i < mModuleArray.size(); ++i) {
        mModuleArray[i]->update();
    }*/

    // Rotate the model
    glm::mat4 rot(1.0);
    m_transformMatrix.m_model = glm::rotate(rot, m_angle, glm::vec3(0, 1, 1));

    m_angle += 0.01f;
    RenderBackendService *rbSrv = getRenderBackendService();

    rbSrv->beginPass(PipelinePass::getPassNameById(RenderPassId));
    rbSrv->beginRenderBatch("b1");

    rbSrv->setMatrix(MatrixType::Model, m_transformMatrix.m_model);

    rbSrv->endRenderBatch();
    rbSrv->endPass();

    AppBase::onUpdate();

    //SetFPSText()
}

bool OsreEdApp::onDestroy() {
    return true;
}

} // namespace Editor
} // namespace OSRE
