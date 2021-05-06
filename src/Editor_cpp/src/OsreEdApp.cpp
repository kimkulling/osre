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
#include "ProgressReporter.h"
#include "Modules/InspectorModule/InspectorModule.h"
#include "Modules/ModuleBase.h"

#include <osre/App/AssimpWrapper.h>
#include <osre/App/AssetRegistry.h>
#include <osre/App/Entity.h>
#include <osre/Common/TCommand.h>
#include <osre/IO/Directory.h>
#include <osre/IO/Uri.h>
#include <osre/IO/File.h>
#include <osre/IO/IOService.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/PlatformOperations.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/App/Project.h>
#include <osre/Platform/PlatformInterface.h>

#ifdef OSRE_WINDOWS
#   include "Engine/Platform/win32/Win32EventQueue.h"
#   include "Engine/Platform/win32/Win32Window.h"
#   include <CommCtrl.h>
#   include <winuser.h>
#   include <windows.h>
#   include <commctrl.h>
#   include <strsafe.h>
#endif
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace OSRE {
namespace Editor {

using namespace ::OSRE::App;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Platform;
using namespace ::OSRE::IO;

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

#define ID_TREEVIEW 100

HWND hStatic = NULL;

#endif // OSRE_WINDOWS

SceneData::SceneData() :
        Name(),
        m_modelNode(),
        mCamera( nullptr ),
        mWorld( nullptr ) {
    // empty
}

OsreEdApp::OsreEdApp(int argc, char *argv[]) :
        AppBase(argc, (const char **)argv, "api", "The render API"),
        m_model(),
        m_transformMatrix(),
        mSceneData(),
        mProject(nullptr),
        mResolution(),
        mMesh2D(nullptr) {
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
        w->getWindowsRect(mResolution);
    }

    AppBase::getRenderBackendService()->enableAutoResizing(false);
    
    createUI();

    return true;
}

void OsreEdApp::loadAsset(const IO::Uri &modelLoc) {
    Platform::AbstractWindow *rootWindow = getRootWindow();
    if (nullptr == rootWindow) {
        return;
    }
    ProgressReporter reporter(rootWindow);
    reporter.start();
    AssimpWrapper assimpWrapper(*getIdContainer(), getActiveWorld());
    if (!assimpWrapper.importAsset(modelLoc, 0)) {
        reporter.stop();
        return;
    }

    RenderBackendService *rbSrv = getRenderBackendService();
    if (nullptr == rbSrv) {
        reporter.stop();
        return;
    }

    Rect2ui windowsRect;
    rootWindow->getWindowsRect(windowsRect);
    World *world = getActiveWorld();
    mSceneData.mCamera = world->addCamera("camera_1");
    mSceneData.mCamera->setProjectionParameters(60.f, (f32)windowsRect.m_width, (f32)windowsRect.m_height, 0.01f, 1000.f);
    Entity *entity = assimpWrapper.getEntity();

    world->addEntity(entity);
    mSceneData.mCamera->observeBoundingBox(entity->getAABB());
    mSceneData.m_modelNode = entity->getNode();

    const std::string &model = modelLoc.getResource();
    rootWindow->setWindowsTitle("Model " + model);

    reporter.stop();
}

void OsreEdApp::newProjectCmd(ui32, void *) {
    mProject = new App::Project();
    mProject->create("New project", 0, 1);
    const String &projectName = mProject->getProjectName();
    
    AppBase::setWindowsTitle("OSRE ED!" + String(" Project: ") + projectName);
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

struct Widget {
    Widget *mParent;
    CPPCore::TArray<Widget *> mChildren;
    Rect2ui mRect;
};

struct Label : Widget {
    String mLabel;
};

struct Headline : Widget {
    Label mName;
};
struct Panel : Widget {
    Headline mHeadline;
};

struct Button : Widget {
    Label mLabel;
};

struct TreeViewItem {
    String Name;
    CPPCore::TArray<TreeViewItem *> Children;
};

struct TreeView : Widget {
    Label mLabel;
    TreeViewItem *mRoot;
};

void drawRect(Rect2ui r) {
    glm::vec2 p0(r.m_x1, r.m_y1), p1(r.getX1(), r.getY2()), p2(r.getX2(), r.getY2()), p3(r.getX2(), r.getY2());

}

void drawLabel(Label &label, Mesh *mesh2D) {
}

void OsreEdApp::createUI() {
    mMesh2D = Mesh::create(1);
}

void OsreEdApp::onUpdate() {
    for (ui32 i = 0; i < mModules.size(); ++i) {
        ModuleBase *module = mModules[i];
        if (nullptr == module) {
            OSRE_ASSERT(nullptr != module);
            continue;
        }

        module->update();
        module->render();
    }
    AppBase::onUpdate();
}

bool OsreEdApp::onDestroy() {
    Mesh::destroy(&mMesh2D);
    mMesh2D = nullptr;

    return true;
}

bool OsreEdApp::loadSceneData(const IO::Uri &filename, SceneData &sd) {
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


    stream->close();

    return true;
}

} // namespace Editor
} // namespace OSRE
