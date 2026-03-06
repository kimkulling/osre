## Instancing
![Instancing](../../assets/Images/01_box.png)
This sample demonstrates hardware instancing for rendering multiple objects efficiently.

## Code Overview
```cpp
#include "App/App.h"
#include "App/ServiceProvider.h"
#include "Common/Ids.h"
#include "IO/Uri.h"
#include "Common/BaseMath.h"
#include "Platform/AbstractWindow.h"
#include "Properties/Settings.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/MeshBuilder.h"
#include "App/CameraComponent.h"

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

DECL_OSRE_LOG_MODULE(InstancingApp);

class InstancingApp : public App::AppBase {
    App::Entity *mEntity = nullptr;
    App::CameraComponent *mCamera = nullptr;

public:
    InstancingApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv, "api:model", "The render API:The model to load"),
            mEntity(nullptr),
            mCamera(nullptr) {
    }

    ~InstancingApp() = default;

protected:
    bool setupScene() {
        Platform::AbstractWindow *rootWindow = getRootWindow();
        if (nullptr == rootWindow) {
            return false;
        }

        Scene *scene = AppBase::getActiveScene();
        mEntity = new App::Entity("instance", scene->getIds(), scene);
        Entity *camEntity = new Entity("camera", scene->getIds(), scene);
        App::CameraComponent *camera = (App::CameraComponent *)camEntity->createComponent(ComponentType::CameraComponentType);
        scene->setActiveCamera(camera);

        Rect2ui windowsRect;
        rootWindow->getWindowsRect(windowsRect);
        camera->setProjectionParameters(60.f, (f32)windowsRect.width, (f32)windowsRect.height, 0.0001f, 1000.f);
        MeshBuilder meshBuilder;
        scene->addEntity(mEntity);
        Mesh *mesh = meshBuilder.createCube(VertexType::RenderVertex, 2, BufferAccessType::ReadWrite).getMesh();
        if (nullptr != mesh) {
            RenderComponent *rc = (RenderComponent *)mEntity->getComponent(ComponentType::RenderComponentType);
            rc->addStaticMesh(mesh);
            scene->init();
            camera->observeBoundingBox(mEntity->getAABB());
        }

        return true;
    }

    bool onCreate() override {
        if (!AppBase::onCreate()) {
            osre_debug(Tag, "Error while creating.");
            return false;
        }
        AppBase::setWindowsTitle("Instancing sample!");

        return setupScene();
    }

    void onUpdate() override {
        glm::mat4 rot(1.0);
        RenderBackendService *rbSrv = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);

        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        rbSrv->beginRenderBatch("b1");
        rbSrv->endRenderBatch();
        rbSrv->endPass();

        AppBase::onUpdate();
    }
};

int main(int argc, char *argv[]) {
    InstancingApp myApp(argc, argv);
    if (!myApp.initWindow(10, 10, 1024, 768, "Instancing-Sample", WindowMode::Windowed, WindowType::Root, RenderBackendType::OpenGLRenderBackend)) {
        return 1;
    }

    while (myApp.handleEvents()) {
        myApp.update();
        myApp.requestNextFrame();
    }

    myApp.destroy();

    return 0;
}
```

## Key Concepts
Hardware instancing allows rendering multiple copies of the same mesh with a single draw call, significantly improving performance when rendering many identical objects.
