/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2022 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Scripting/PythonInterface.h"
#include "RenderView/MainRenderView.h"
#include "ProgressReporter.h"
#include "RenderView/MainRenderView.h"
#include "Gui/UIElements.h"
#include "Modules/InspectorModule/InspectorModule.h"
#include "Modules/LogModule/LogModule.h"
#include "Actions/ImportAction.h"
#include "Engine/App/MouseEventListener.h"

#include <osre/App/ModuleBase.h>
#include <osre/App/App.h>
#include <osre/Animation/AnimatorBase.h>
#include <osre/IO/Directory.h>
#include <osre/IO/Uri.h>
#include <osre/IO/File.h>
#include <osre/IO/IOService.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/PlatformOperations.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Mesh.h>
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
using namespace ::OSRE::Animation;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Platform;
using namespace ::OSRE::IO;

static constexpr ui32 HorizontalMargin = 2;
static constexpr ui32 VerticalMargin = 2;

static constexpr c8 Tag[] = "OsreApp";

static void createTitleString(const String &projectName, String &titleString) {
    titleString.clear();
    titleString += "OSRE ED!";

    titleString += " Project: ";
    titleString += projectName;
}

static Project *createProject(const String &name) {
    Project *project = new App::Project();
    project->setProjectName(name);

    return project;
}

OsreEdApp::OsreEdApp(int argc, char *argv[]) :
        AppBase(argc, (const char **)argv, "api", "The render API"),
        mMainRenderView(nullptr),
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

bool OsreEdApp::onCreate() {
    if (!AppBase::onCreate()) {
        return false;
    }

    setupPythonInterface();

    mModuleRegistry.registerModule(new InspectorModule(this));
    mModuleRegistry.registerModule(new LogModule(this));
    
    setupUserInterface();
    setupRenderView();

    mTransformController = AppBase::getTransformController(m_transformMatrix);

#ifdef _DEBUG
    Logger::getInstance()->setVerboseMode(Logger::VerboseMode::Debug);
#endif

    return true;
}

void OsreEdApp::loadAsset(const Uri &modelLoc) {
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
    if (mProject == nullptr) {
        mProject = createProject(modelLoc.getAbsPath());
    }
    mSceneData.mCamera = world->addCamera("camera_1");
    mSceneData.mCamera->setProjectionParameters(60.f, (f32)windowsRect.width, (f32)windowsRect.height, 0.01f, 1000.f);
    Entity *entity = action.getEntity();

    reporter.update(10);
    world->addEntity(entity);
    mSceneData.mCamera->observeBoundingBox(entity->getAABB());
    mSceneData.m_modelNode = entity->getNode();

    String asset = modelLoc.getResource();
    mProject->addAsset(asset);
    String title;
    createTitleString(mProject->getProjectName(), title);
    rootWindow->setWindowsTitle(title);

    
    setStatusBarText("View", mProject->getProjectName(), action.getNumVertices(), action.getNumTriangles());
    reporter.update(70);
    reporter.stop();
}

void OsreEdApp::newProjectCmd(ui32, void *data) {
    std::string name = "New project";
    if (data != nullptr) {
        CPPCore::Variant *v = (CPPCore::Variant*)data;
        name = v->getString();
    }
    mProject = createProject(name);
    String title = mProject->getProjectName();
    createTitleString(mProject->getProjectName(), title);
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
    PlatformOperations::getDialog("Really quit?", 
        "Do you really quite OSRE-Ed?", 
        Platform::PlatformOperations::DlgButton_YesNo, result);
    if (result == Platform::DlgResults::DlgButtonRes_Yes) {
        AppBase::requestShutdown();
    }
}

void OsreEdApp::gettingHelpCmd(ui32, void *) {
    ::ShellExecute(nullptr, "open", "https://github.com/kimkulling/osre/issues", NULL, NULL, SW_SHOWNORMAL);
}

void OsreEdApp::showVersionCmd(ui32, void*) {
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
    const MouseInputState & mis = AppBase::getMouseEventListener()->getMouseInputState();
    
    Key key = AppBase::getKeyboardEventListener()->getLastKey();
    glm::mat4 rot(1.0);
    TArray<TransformCommandType> transformCmds;
    mTransformController->getMouseUpdate(mis);
    mTransformController->update(TransformController::getKeyBinding(key));
    for (ui32 i = 0; i < transformCmds.size(); ++i) {
        mTransformController->update(transformCmds[i]);
    }
    transformCmds.clear();
    RenderBackendService *rbSrv = getRenderBackendService();
    mModuleRegistry.update();
    
    mMainRenderView->render(rbSrv, m_transformMatrix.m_model);

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

    if (mProject != nullptr) {
        if (mProject->dataNeedsSave()) {
            // ToDo: Add dialog
        }
        mProject->clear();
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
        osre_error(Tag, "Cannot open file " + filename.getResource() + ".");
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

bool OsreEdApp::setupUserInterface() {
    Win32Window *window = (Win32Window *)getRootWindow();
    AbstractPlatformEventQueue *queue = PlatformInterface::getInstance()->getPlatformEventHandler();
    if (nullptr == window || nullptr == queue) {
        return false;
    }

    String title, projectName;
    if (mProject == nullptr) {
        projectName = "None";
    } else {
        projectName = mProject->getProjectName();
    }

    createTitleString(projectName, title);
    AppBase::setWindowsTitle(title);

    UIElements::createMenues(window, this, queue);
    window->createStatusBar(100, 4);
    window->getWindowsRect(mResolution);

    AppBase::getRenderBackendService()->enableAutoResizing(false);

    return true;
}

bool OsreEdApp::setupRenderView() {
    if (mMainRenderView != nullptr) {
        osre_error(Tag, "Renderview already initiated.");
        return false;
    }

    Stage *stage = getStage();
    if (stage == nullptr) {
        osre_error(Tag, "Stage is nullptr.");
        return false;
    }
    
    World *world = getStage()->getActiveWorld();
    if (nullptr == world) {
        osre_error(Tag, "World is nullptr.");
        return false;
    }

    mMainRenderView = new MainRenderView();
    Entity *editorEntity = new Entity("editor.entity", *getIdContainer(), world);
    mMainRenderView->createEditorElements((RenderComponent *)editorEntity->getComponent(ComponentType::RenderComponentType));

    return true;
}

bool OsreEdApp::setupPythonInterface() {
    mPythonInterface = new PythonInterface;
    if (!mPythonInterface->create(this)) {
        osre_error(Tag, "Error while creating Python Interface.");
        return false;
    }
    const String src = "from time import time,ctime\n"
                       "print('Today is', ctime(time()))\n";

    mPythonInterface->runScript(src);

    return true;
}

} // namespace Editor
} // namespace OSRE
