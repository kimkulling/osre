/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#pragma once

#include "OSREEdApp.h"
#include "Actions/ImportAction.h"
#include "App/ServiceProvider.h"
#include "App/TransformController.h"
#include "Common/Logger.h"
#include "Platform/PlatformOperations.h"
#include "ProgressReporter.h"
#include "RenderBackend/MeshBuilder.h"
#include "RenderBackend/Material.h"
#include "RenderBackend/RenderPass.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderView/MainRenderView.h"

namespace OSRE {
namespace Editor {

static constexpr char Tag[] = "OsreEdApp";

using namespace OSRE::App;
using namespace OSRE::RenderBackend;

static void createTitleString(const String &projectName, String &titleString) {
    titleString.clear();
    titleString += "OSRE ED!";

    titleString += " Project: ";
    titleString += projectName;
}

static Project *createProject(const String &name) {
    auto *project = new App::Project();
    project->setProjectName(name);

    return project;
}

OsreEdApp::OsreEdApp(int argc, char *argv[]) :
        AppBase(argc, const_cast<const char **>(argv), "api", "The render API"),
        mProject(nullptr),
        mEntity(nullptr),
        mGuiEntity(nullptr),
        mKeyboardTransCtrl(nullptr) {
    // empty
}

CameraComponent *OsreEdApp::setupCamera(Scene *scene) {
    Entity *camEntity = scene->findEntity("camera");
    if (camEntity == nullptr) {
        camEntity = new Entity("camera", *getIdContainer(), scene);
    }

    auto *camera = static_cast<CameraComponent*>(camEntity->createComponent(ComponentType::CameraComponentType));
    scene->addEntity(camEntity);
    scene->setActiveCamera(camera);
    ui32 w{0u}, h{0u};
    AppBase::getResolution(w, h);
    camera->setProjectionParameters(mConfig.mFov, (f32)w, (f32)h, mConfig.mNear, mConfig.mFar);

    return camera;
}

void OsreEdApp::newProjectCmd(ui32, void *data) {
    String name = "New project";
    if (data != nullptr) {
        cppcore::Variant *v = (cppcore::Variant*) data;
        name = v->getString();
    }
    mProject = createProject(name);
    String title = mProject->getProjectName();
    createTitleString(mProject->getProjectName(), title);
    AppBase::setWindowsTitle(title);
}

void OsreEdApp::loadAsset(const IO::Uri &modelLoc) {
    Platform::AbstractWindow *rootWindow = getRootWindow();
    if (nullptr == rootWindow) {
        return;
    }

    ProgressReporter reporter(rootWindow);
    reporter.start();
    reporter.update(10);
    
    Scene *scene = getActiveScene();
    if (scene == nullptr) {
        scene = new Scene(modelLoc.getResource());
        addScene(scene, true);
    }

    ImportAction action(getIdContainer(), getActiveScene());
    ArgumentList args;
    args.add(cppcore::Variant::createFromString(modelLoc.getAbsPath()));
    if (!action.run(args)) {
        reporter.stop();
        return;
    }
    if (mProject == nullptr) {
        newProjectCmd(1, cppcore::Variant::createFromString(modelLoc.getResource()));
    }
    reporter.update(10);
    auto *rbSrv = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
    if (nullptr == rbSrv) {
        reporter.stop();
        return;
    }

    Rect2ui windowsRect;
    rootWindow->getWindowsRect(windowsRect);
    scene = getActiveScene();
    if (mProject == nullptr) {
        mProject = createProject(modelLoc.getAbsPath());
    }
    Entity *entity = action.getEntity();
    mSceneData.mCamera = setupCamera(scene);

    reporter.update(10);
    scene->addEntity(entity);
    mSceneData.mCamera->observeBoundingBox(entity->getAABB());
    mSceneData.m_modelNode = entity->getNode();

    String title;
    createTitleString(mProject->getProjectName(), title);
    rootWindow->setWindowsTitle(title);

    reporter.update(70);
    reporter.stop();
}

bool setupEditorGimmics(Entity *guiEntity) {
    if (guiEntity == nullptr) {
        return false;
    }
    RenderComponent *rc = (RenderComponent*) guiEntity->getComponent(ComponentType::RenderComponentType);
    if (rc == nullptr) {
        return false;
    }

    Mesh *axis = MainRenderView::createCoordAxis(150);
    rc->addStaticMesh(axis);

    Mesh *grid = MainRenderView::createGrid(50);
    rc->addStaticMesh(grid);

    return true;
}

bool OsreEdApp::onCreate() {
    if (!AppBase::onCreate()) {
        return false;
    }

    AppBase::setWindowsTitle("OSRE Ed ");
    Scene *scene = getActiveScene();

    mGuiEntity = new Entity("gui", *AppBase::getIdContainer(), scene);
    scene->addEntity(mGuiEntity);
    setupEditorGimmics(mGuiEntity);
    scene->init();

    Platform::AbstractWindow *rootWindow = getRootWindow();
    Rect2ui windowsRect;
    rootWindow->getWindowsRect(windowsRect);

    mKeyboardTransCtrl = AppBase::getTransformController(mTransformMatrix);
    mSceneData.mCamera = setupCamera(scene);
    mSceneData.mCamera->observeBoundingBox(mGuiEntity->getAABB());

    osre_info(Tag, "Creation finished.");

    return true;
}

void OsreEdApp::onUpdate() {
    if (AppBase::isKeyPressed(Platform::KEY_O) || AppBase::isKeyPressed(Platform::KEY_o)) {
        IO::Uri modelLoc;
        Platform::PlatformOperations::getFileOpenDialog("Choose asset for import", "*", modelLoc);
        if (modelLoc.isValid()) {
            loadAsset(modelLoc);
        }
    }

    Platform::Key key = AppBase::getKeyboardEventListener()->getLastKey();
    if (key != Platform::KEY_UNKNOWN && mKeyboardTransCtrl != nullptr) {
        mKeyboardTransCtrl->update(mKeyboardTransCtrl->getKeyBinding(key));
    }

    RenderBackendService *rbSrv = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
    rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
    {
        rbSrv->beginRenderBatch("b1");

        rbSrv->setMatrix(MatrixType::Model, mTransformMatrix.mModel);

        rbSrv->endRenderBatch();
    }
    rbSrv->endPass();

    AppBase::onUpdate();
}

} // namespace Editor
} // namespace OSRE
