/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "App/App.h"
#include "App/ServiceProvider.h"
#include "Common/Ids.h"
#include "IO/Uri.h"
#include "Common/BaseMath.h"
#include "Platform/AbstractWindow.h"
#include "Properties/Settings.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/RenderCommon.h"
#include "App/CameraComponent.h"
#include "App/Scene.h"

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

// To identify local log entries
static constexpr c8 Tag[] = "InstancingApp";

//-------------------------------------------------------------------------------------------------
///	@ingroup    Editor
///
/// @brief
//-------------------------------------------------------------------------------------------------
class InstancingApp : public App::AppBase {
    App::Entity *mEntity;
    App::CameraComponent *mCamera;

public:
    InstancingApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv, "api:model", "The render API:The model to load"),
            mEntity(nullptr),
            mCamera(nullptr) {
        // empty
    }

    ~InstancingApp() = default;

protected:
    bool setupScene() {
        Platform::AbstractWindow *rootWindow = getRootWindow();
        if (nullptr == rootWindow) {
            return false;
        }
        World *world = getStage()->getActiveWorld(0);
        mEntity = new App::Entity("instance", world->getIds(), world);
        Entity *camEntity = new App::Entity("camera", world->getIds(), world);
        App::CameraComponent *camera = (App::CameraComponent *)camEntity->createComponent(ComponentType::CameraComponentType);
        world->setActiveCamera(camera);

        Rect2ui windowsRect;
        rootWindow->getWindowsRect(windowsRect);
        camera->setProjectionParameters(60.f, (f32)windowsRect.width, (f32)windowsRect.height, 0.0001f, 1000.f);
        MeshBuilder meshBuilder;
        world->addEntity(mEntity);
        RenderBackend::Mesh *mesh = meshBuilder.createTriangle(VertexType::RenderVertex, BufferAccessType::ReadWrite).getMesh();
        if (nullptr != mesh) {
            RenderComponent *rc = (RenderComponent *)mEntity->getComponent(ComponentType::RenderComponentType);
            rc->addStaticMesh(mesh);
            world->init();
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
        // Rotate the model
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
    if (!myApp.initWindow(10, 10, 1024, 768, "Instancing-Sample", false, false, RenderBackendType::OpenGLRenderBackend)) {
        return 1;
    }

    while (myApp.handleEvents()) {
        myApp.update();
        myApp.requestNextFrame();
    }

    myApp.destroy();

    return 0;
}
