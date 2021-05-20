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
#include "Gui/UIElements.h"
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
#include <osre/RenderBackend/Mesh.h>
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

static void createTitleString( const SceneData &sd, String &titleString ) {
    titleString.clear();
    titleString += "OSRE ED!";

    titleString += " Project: ";
    titleString += sd.ProjectName;

    titleString += " Current Asset: ";
    titleString += sd.AssetName;
}

static Mesh *createGrid(ui32 numLines) {
    if (0 == numLines) {
        return nullptr;
    }

    Mesh *grid = Mesh::create(1);
    f32 currentX = -10.0f, currentY = -10.0f;
    f32 diffX = 20.0 / numLines;
    f32 diffY = 20.0 / numLines;
    CPPCore::TArray<RenderBackend::RenderVert> lineData;
    CPPCore::TArray<ui16> lineIndices;
    ui16 currentIndex = 0;
    for (ui32 y = 0; y < numLines + 1; ++y) {
        for (ui32 x = 0; x < numLines + 1; ++x) {
            RenderVert v1, v2;
            v1.position.x = currentX;
            v1.position.y = currentY;
            v1.position.z = 0.0f;
            v1.color0 = glm::vec3(0.5, 0.5, 0.5);
            currentX += diffX;
            v2.position.x = currentX;
            v2.position.y = currentY;
            v2.position.z = 0.0f;
            v2.color0 = glm::vec3(0.5, 0.5, 0.5);
            lineData.add(v1);
            lineData.add(v2);
            lineIndices.add(currentIndex);
            currentIndex++;
            lineIndices.add(currentIndex);
            currentIndex++;
        }
        currentY += diffY;
    }
    grid->attachVertices(&lineData[0], sizeof(RenderVert) * lineData.size());
    grid->attachIndices(&lineIndices[0], sizeof(ui16) * lineIndices.size());
    grid->createPrimitiveGroup(IndexType::UnsignedShort, lineData.size() / 2, PrimitiveType::LineList, 0);

    return grid;
}

SceneData::SceneData() :
        Name(),
        ProjectName("none"),
        AssetName("none"),
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
    String title;
    createTitleString(mSceneData, title);
    AppBase::setWindowsTitle(title);

    auto *w = (Win32Window *)getRootWindow();
    AbstractPlatformEventQueue *queue = PlatformInterface::getInstance()->getPlatformEventHandler();
    if (nullptr == w || nullptr == queue) {
        return false;
    }

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

    AppBase::getRenderBackendService()->enableAutoResizing(false);

    World *world = getActiveWorld();
    Entity *editorEntity = new Entity("editor.entity", *getIdContainer(), world);
    Mesh *grid = createGrid(20);
    editorEntity->addStaticMesh(grid);
    //createUI();

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
    mSceneData.mCamera->setProjectionParameters(60.f, (f32)windowsRect.width, (f32)windowsRect.height, 0.01f, 1000.f);
    Entity *entity = assimpWrapper.getEntity();

    world->addEntity(entity);
    mSceneData.mCamera->observeBoundingBox(entity->getAABB());
    mSceneData.m_modelNode = entity->getNode();

    mSceneData.AssetName = modelLoc.getResource();
    String title;
    createTitleString(mSceneData, title);
    rootWindow->setWindowsTitle(title);

    reporter.stop();
}

void OsreEdApp::newProjectCmd(ui32, void *) {
    mProject = new App::Project();
    mProject->create("New project", 0, 1);
    const String &projectName = mProject->getProjectName();
    
    String title;
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

void createRect2D(Rect2ui r, Mesh *mesh2D, Style &style) {
    if (nullptr == mesh2D) {
        return;
    }

    glm::vec2 p0(r.x1, r.y1), p1(r.getX1(), r.getY2()), p2(r.getX2(), r.getY2()), p3(r.getX2(), r.getY2());
    UIVert edges[4];
    edges[0].position = p0;
    edges[1].position = p1;
    edges[2].position = p2;
    edges[3].position = p3;
    edges[0].color0 = style.BG.toVec4();
    edges[1].color0 = style.BG.toVec4();
    edges[2].color0 = style.BG.toVec4();
    edges[3].color0 = style.BG.toVec4();
    CPPCore::TArray<ui16> indices;
    indices.resize(6);
    indices[0] = 0;
    indices[1] = 2;
    indices[2] = 1;

    indices[3] = 1;
    indices[4] = 2;
    indices[5] = 3;

    mesh2D->attachVertices(&edges[0], sizeof(glm::vec2) * 4);
    mesh2D->attachIndices(&indices[0], sizeof(ui32) * 6);
    mesh2D->createPrimitiveGroup(IndexType::UnsignedShort, 6, PrimitiveType::TriangleList, 0);
}

void drawLabel(Label &label, Mesh *mesh2D) {
}

void OsreEdApp::createUI() {
    return;
    mMesh2D = Mesh::create(1);
    Rect2ui r(100, 100, 200, 200);
    Style style;
    style.BG.m_r = 1;
    style.BG.m_g = 1;
    createRect2D(r, mMesh2D, style);

    Pipeline *pl = AppBase::findPipeline(DefaultPipelines::Pipeline_Default);
    if (nullptr != pl) {
        RenderBackend::Shader *shader = nullptr;
        PipelinePass *uiPass = PipelinePass::create(UiPassId, shader);
        pl->addPass(uiPass);
        RenderBackendService *rbSrv = AppBase::getRenderBackendService();
        rbSrv->beginPass(PipelinePass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("ui");
            rbSrv->addMesh(mMesh2D, 1);
            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();
    }
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
