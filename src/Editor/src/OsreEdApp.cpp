#pragma once

#include "OSREEdApp.h"
#include "Actions/ImportAction.h"
#include "App/ServiceProvider.h"
#include "App/TransformController.h"
#include "Common/Logger.h"
#include "Platform/PlatformOperations.h"
#include "ProgressReporter.h"
#include "RenderBackend/MeshBuilder.h"
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
    Project *project = new App::Project();
    project->setProjectName(name);

    return project;
}

OsreEdApp::OsreEdApp(int argc, char *argv[]) :
        AppBase(argc, (const char **)argv, "api", "The render API"),
        mProject(nullptr),
        mEntity(nullptr),
        mGuiEntity(nullptr),
        mKeyboardTransCtrl(nullptr) {
    // empty
}

constexpr float Near = 0.001f;
constexpr float Far  = 1000.f;

CameraComponent *OsreEdApp::setupCamera(Scene *world) {
    Entity *camEntity = world->findEntity("camera");
    if (camEntity != nullptr) {
        return (CameraComponent*) camEntity->getComponent(ComponentType::CameraComponentType);
    }
    
    camEntity = new Entity("camera", *getIdContainer(), world);
    world->addEntity(camEntity);
    CameraComponent *camera = (CameraComponent*) camEntity->createComponent(ComponentType::CameraComponentType);
    world->setActiveCamera(camera);
    ui32 w{0u}, h{0u};
    AppBase::getResolution(w, h);
    camera->setProjectionParameters(60.f, (f32)w, (f32)h, Near, Far);

    return camera;
}

void OsreEdApp::newProjectCmd(ui32, void *data) {
    String name = "New project";
    if (data != nullptr) {
        cppcore::Variant *v = (cppcore::Variant *)data;
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
    auto *camEntity = new Entity(std::string("camera"), *getIdContainer(), scene);
    auto *camera = (CameraComponent *)camEntity->createComponent(ComponentType::CameraComponentType);
    scene->setActiveCamera(camera);
    mSceneData.mCamera = camera;
    mSceneData.mCamera->setProjectionParameters(60.f, (f32)windowsRect.width, (f32)windowsRect.height, 0.01f, 1000.f);

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

bool setupTripod(Entity *guiEntity) {
    if (guiEntity == nullptr) {
        return false;
    }

    Mesh *axis = MainRenderView::createCoordAxis(150);
    RenderComponent *rc = (RenderComponent*) guiEntity->getComponent(ComponentType::RenderComponentType);
    rc->addStaticMesh(axis);

    return true;
}

bool OsreEdApp::onCreate() {
    if (!AppBase::onCreate()) {
        return false;
    }

    AppBase::setWindowsTitle("Hello-World sample! Rotate with keyboard: w, a, s, d, scroll with q, e");
    Scene *scene = getActiveScene();

    mGuiEntity = new Entity("gui", *AppBase::getIdContainer(), scene);
    scene->addEntity(mGuiEntity);

    setupTripod(mGuiEntity);
    mKeyboardTransCtrl = AppBase::getTransformController(mTransformMatrix);
    auto *camEntity = new Entity(std::string("camera"), *getIdContainer(), scene);
    auto *camera = (CameraComponent *)camEntity->createComponent(ComponentType::CameraComponentType);
    scene->setActiveCamera(camera);
    mSceneData.mCamera = camera;
    //camera->observeBoundingBox(mGuiEntity->getAABB());

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
