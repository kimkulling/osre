/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#include "OsreEdApp.h"
#include "RenderView/MainRenderView.h"
#include "ProgressReporter.h"
#include "RenderView/MainRenderView.h"
#include "Gui/UIElements.h"
#include "Modules/InspectorModule/InspectorModule.h"
#include "Modules/LogModule/LogModule.h"
#include <osre/App/ModuleBase.h>
#include "Scripting/PythonInterface.h"
#include "Actions/ImportAction.h"
#include <cppcore/Common/Variant.h>
#include "Engine/App/MouseEventListener.h"

#include <osre/App/App.h>
#include <osre/IO/Directory.h>
#include <osre/IO/Uri.h>
#include <osre/IO/File.h>
#include <osre/IO/IOService.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/PlatformOperations.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Scene/Scene.h>

#ifdef OSRE_WINDOWS
#   include "Engine/Platform/win32/Win32EventQueue.h"
#   include "Engine/Platform/win32/Win32Window.h"
#   include <CommCtrl.h>
#   include <winuser.h>
#   include <windows.h>
#   include <commctrl.h>
#   include <strsafe.h>
#   include "shellapi.h"
#endif
#include <osre/Common/glm_common.h>

namespace OSRE {
namespace Editor {

using namespace ::OSRE::App;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Platform;
using namespace ::OSRE::IO;
using namespace ::OSRE::Scene;

static const ui32 HorizontalMargin = 2;
static const ui32 VerticalMargin = 2;

#ifdef OSRE_WINDOWS

constexpr int IDM_FILE_NEW = 1;
constexpr int IDM_FILE_OPEN = 2;
constexpr int IDM_FILE_SAVE = 3;
constexpr int IDM_FILE_IMPORT = 4;
constexpr int IDM_FILE_QUIT = 5;

constexpr int IDM_GETTING_HELP = 6;
constexpr int IDM_INFO_VERSION = 7;

#endif // OSRE_WINDOWS

static const c8 *Tag = "OsreApp";

static void createTitleString(const SceneData &sd, String &titleString) {
    titleString.clear();
    titleString += "OSRE ED!";

    titleString += " Project: ";
    titleString += sd.ProjectName;

    titleString += " Current Asset: ";
    titleString += sd.AssetName;
}

OsreEdApp::OsreEdApp(int argc, char *argv[]) :
        AppBase(argc, (const char **)argv, "api", "The render API"),
        m_model(),
        m_transformMatrix(),
        mSceneData(),
        mProject(nullptr),
        mModuleRegistry(),
        mResolution(),
        mMesh2D(nullptr),
        mPythonInterface(nullptr),
        mTransformController(nullptr) {
    // empty
}

OsreEdApp::~OsreEdApp() {
    // empty
}

bool OsreEdApp::onCreate() {
    if (!AppBase::onCreate()) {
        return false;
    }

    mModuleRegistry.registerModule(new InspectorModule(this));
    mModuleRegistry.registerModule(new LogModule(this));
    
    String title;
    createTitleString(mSceneData, title);
    AppBase::setWindowsTitle(title);

    Win32Window *w = (Win32Window *)getRootWindow();
    AbstractPlatformEventQueue *queue = PlatformInterface::getInstance()->getPlatformEventHandler();
    if (nullptr == w || nullptr == queue) {
        return false;
    }

    UIElements::createMenues(w, this, queue);

    /* w->beginMenu();
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

    w->endMenu();*/
    w->createStatusBar(100, 4);
    w->setStatusText(0, "Test");

    w->getWindowsRect(mResolution);

    AppBase::getRenderBackendService()->enableAutoResizing(false);

    World *world = getStage()->getActiveWorld();
    if (nullptr == world) {
        return false;
    }

    mMainRenderView = new MainRenderView();
    Entity *editorEntity = new Entity("editor.entity", *getIdContainer(), world);
    mMainRenderView->createEditorElements((RenderComponent *)editorEntity->getComponent(ComponentType::RenderComponentType));

    mPythonInterface = new PythonInterface;
    if (!mPythonInterface->create(this)) {
        osre_error(Tag, "Error while creating Python Interface.");
        return false;
    }
    const String src = "from time import time,ctime\n"
                 "print('Today is', ctime(time()))\n";
                 
    mPythonInterface->runScript(src);

    mTransformController = AppBase::getTransformController(m_transformMatrix);

#ifdef _DEBUG
    Logger::getInstance()->setVerboseMode(Logger::VerboseMode::Debug);
#endif

    return true;
}

void OsreEdApp::loadAsset(const IO::Uri &modelLoc) {
    Platform::AbstractWindow *rootWindow = getRootWindow();
    if (nullptr == rootWindow) {
        return;
    }
    
    ProgressReporter reporter(rootWindow);
    reporter.start();
    reporter.update(10);
    ImportAction action(getIdContainer(), getStage()->getActiveWorld());
    ArgumentList args;
    args.add(CPPCore::Variant::createFromString(modelLoc.getAbsPath()));
    if (!action.run(args)) {
        reporter.stop();
        return;
    }
    if (mProject == nullptr) {
        newProjectCmd(1, CPPCore::Variant::createFromString(modelLoc.getResource()));
        mProject->setStage(AppBase::getStage());
    }
    reporter.update(10);
    RenderBackendService *rbSrv = getRenderBackendService();
    if (nullptr == rbSrv) {
        reporter.stop();
        return;
    }

    Rect2ui windowsRect;
    rootWindow->getWindowsRect(windowsRect);
    World *world = getStage()->getActiveWorld();
    mSceneData.mCamera = world->addCamera("camera_1");
    mSceneData.mCamera->setProjectionParameters(60.f, (f32)windowsRect.width, (f32)windowsRect.height, 0.01f, 1000.f);
    Entity *entity = action.getEntity();

    reporter.update(10);
    world->addEntity(entity);
    mSceneData.mCamera->observeBoundingBox(entity->getAABB());
    mSceneData.m_modelNode = entity->getNode();

    mSceneData.AssetName = modelLoc.getResource();
    String title;
    createTitleString(mSceneData, title);
    rootWindow->setWindowsTitle(title);

    setStatusBarText("View", mSceneData.AssetName, 1, 1);
    reporter.update(70);
    reporter.stop();
}

void OsreEdApp::newProjectCmd(ui32, void *data) {
    mProject = new App::Project();
    std::string name = "New project";
    if (data != nullptr) {
        CPPCore::Variant *v = (CPPCore::Variant*)data;
        name = v->getString();
    }
    mProject->setProjectName(name);
    String title = mProject->getProjectName();
    createTitleString(mSceneData, title);
    AppBase::setWindowsTitle(title);
}

void OsreEdApp::loadProjectCmd(ui32, void *) {
    IO::Uri projectLoc;
    PlatformOperations::getFileOpenDialog("Select project file", "*", projectLoc);
    if (projectLoc.isValid()) {
        loadSceneData(projectLoc, mSceneData);
    }
}

void OsreEdApp::saveProjectCmd(ui32, void *) {
    IO::Uri projectLoc;
    PlatformOperations::getFileSaveDialog("Select project file", "*", projectLoc);
    if (projectLoc.isValid()) {
        saveSceneData(projectLoc, mSceneData);
    }
}

void OsreEdApp::importAssetCmd(ui32, void *) {
    IO::Uri modelLoc;
    PlatformOperations::getFileOpenDialog("Select asset for import", "*", modelLoc);
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

    ::ShellExecute(nullptr, "open", "https://github.com/kimkulling/osre/issues", NULL, NULL, SW_SHOWNORMAL);
}

void OsreEdApp::showVersionCmd(ui32 cmdId, void *data) {
    DlgResults res;
    PlatformOperations::getDialog("Version Info", "OSRE Version 0.0.1", PlatformOperations::DlgButton_ok, res);
}

ModuleRegistry &OsreEdApp::getModuleRegistry() {
    return mModuleRegistry;
}


void OsreEdApp::setStatusBarText(const String &mode, const String &model, i32 numVertices, i32 numTriangles) {
    Win32Window *win = (Win32Window *)getRootWindow();
    if (nullptr == win) {
        return;
    }

    if (!mode.empty()) {
        win->setStatusText(0, mode.c_str());
    }

    if (!mode.empty()) {
        String modelName = "Model: ";
        modelName += model;
        win->setStatusText(1, modelName.c_str());
    }

    if (-1 != numVertices) {
        c8 buffer[1024];
        memset(buffer, '\0', 1024);
        sprintf(buffer, "Number of vertices: %d", numVertices);
        win->setStatusText(2, buffer);
    }

    if (-1 != numTriangles) {
        c8 buffer[1024];
        memset(buffer, '\0', 1024);
        sprintf(buffer, "Number of triangles: %d", numTriangles);
        win->setStatusText(3, buffer);    
    }
}

void OsreEdApp::onUpdate() {
    Key key = AppBase::getKeyboardEventListener()->getLastKey();
    glm::mat4 rot(1.0);
    TArray<TransformCommandType> transformCmds;
    mTransformController->update(TransformController::getKeyBinding(key));
    for (ui32 i = 0; i < transformCmds.size(); ++i) {
        mTransformController->update(transformCmds[i]);
    }
    transformCmds.clear();
    RenderBackendService *rbSrv = getRenderBackendService();

    rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
    rbSrv->beginRenderBatch("b1");

    rbSrv->setMatrix(MatrixType::Model, m_transformMatrix.m_model);
    
    mModuleRegistry.update();
    
    rbSrv->endRenderBatch();
    rbSrv->endPass();

    AppBase::onUpdate();
}

void OsreEdApp::onRender() {
    mModuleRegistry.render();
    
    AppBase::onRender();
}

bool OsreEdApp::onDestroy() {
    mTransformController = nullptr;

    delete mPythonInterface;
    mPythonInterface = nullptr;

    delete mMainRenderView;
    mMainRenderView = nullptr;

    delete mMesh2D;
    mMesh2D = nullptr;

    return true;
}

bool OsreEdApp::loadSceneData(const IO::Uri &filename, SceneData&) {
    if (filename.isEmpty()) {
        return false;
    }

    Stream *stream = IOService::getInstance()->openStream(filename, Stream::AccessMode::ReadAccess);
    if (nullptr == stream) {
        return false;
    }

    stream->close();

    return true;
}

bool OsreEdApp::saveSceneData(const IO::Uri &filename, SceneData &sd) {
    if (filename.isEmpty()) {
        return false;
    }

    Stream *stream = IOService::getInstance()->openStream(filename, Stream::AccessMode::WriteAccess);
    if (nullptr == stream) {
        return false;
    }

    if (mProject == nullptr) {
        CPPCore::Variant *v = CPPCore::Variant::createFromString(filename.getResource());
        newProjectCmd(1, (void *)v);
    }
    mProject->save(filename.getAbsPath(), getStage());
    stream->close();

    return true;
}

} // namespace Editor
} // namespace OSRE
