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
#include "App/AppBase.h"
#include "App/Entity.h"
#include "App/ServiceProvider.h"
#include "App/Component.h"
#include "App/CameraComponent.h"
#include "Properties/Settings.h"
#include "Common/Logger.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/TransformMatrixBlock.h"
#include "RenderBackend/MeshBuilder.h"
#include "RenderBackend/2D/CanvasRenderer.h"
#include "RenderBackend/2D/RenderPass2D.h"

using namespace ::OSRE;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::App;

// To identify local log entries
static constexpr c8 Tag[] = "ModelLoadingApp";

//-------------------------------------------------------------------------------------------------
///	@ingroup    Editor
///
/// @brief 
//-------------------------------------------------------------------------------------------------
class Demo2DApp : public App::AppBase {
    TransformMatrixBlock  mTransformMatrix;
    /// The 2D renderer for the ui overlay
    CanvasRenderer *mCanvasRenderer;
    /// The entity to render the 3d-scene
    Entity *mEntity;

public:
    Demo2DApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv),
            mTransformMatrix(),
            mCanvasRenderer(nullptr),
            mEntity(nullptr) {
        // empty
    }

    ~Demo2DApp() override = default;

    void quitCallback(ui32, void *) {
        AppBase::requestShutdown();
    }

protected:
    CameraComponent *setupCamera(Scene *scene) {
        Entity *camEntity = new Entity("camera", *getIdContainer(), scene);
        scene->addEntity(camEntity);
        CameraComponent *camera = (CameraComponent *)camEntity->createComponent(ComponentType::CameraComponentType);
        scene->setActiveCamera(camera);

        ui32 w, h;
        AppBase::getResolution(w, h);
        camera->setProjectionParameters(60.f, (f32)w, (f32)h, 0.001f, 1000.f);

        return camera;
    }

    bool onCreate() override {
        Properties::Settings *baseSettings  = AppBase::getSettings();
        if (baseSettings == nullptr) {
            return false;
        }

        baseSettings->setString(Properties::Settings::WindowsTitle, "Demo in 2D!");
        if (!AppBase::onCreate()) {
            return false;
        }
        
        mCanvasRenderer = AppBase::getCanvasRenderer();
        
        mCanvasRenderer->selectLayer(0);
        const Color4 Red(1, 0, 0, 0);
        mCanvasRenderer->setColor(Red);
        mCanvasRenderer->drawRect(10, 10, 500, 40, true);
        
        Color4 green(0, 1, 0, 1);
        
        mCanvasRenderer->setColor(Red);
        mCanvasRenderer->drawRect(1, 1, 1000, 760, false);

        mCanvasRenderer->setColor(green);
        mCanvasRenderer->drawRect(100, 100, 100, 40, true);

        mCanvasRenderer->drawRect(100, 200, 90, 50, true);

        mCanvasRenderer->drawRect(100, 300, 80, 60, true);

        mCanvasRenderer->drawRect(100, 400, 70, 70, true);

        mCanvasRenderer->drawRect(100, 500, 60, 60, false);

        mCanvasRenderer->selectLayer(2);
        Color4 yellow(1, 1, 0, 1);
        mCanvasRenderer->setColor(yellow);
        mCanvasRenderer->drawRect(100, 100, 110, 30, true);

        mCanvasRenderer->drawRect(100, 600, 110, 124, true);

        mCanvasRenderer->drawRect(100, 700, 110, 124, true);
        
        mCanvasRenderer->drawRect(100, 1000, 110, 124, true);

        mCanvasRenderer->drawText(300, 100, "Test");

        Scene *scene = new Scene("hello_world", *getIdContainer());
        addScene(scene, true);
        mEntity = scene->createEntity("entity");
        MeshBuilder meshBuilder;
        Mesh *mesh = meshBuilder.createCube(VertexType::ColorVertex, .5, .5, .5, BufferAccessType::ReadOnly).getMesh();
        if (mesh != nullptr) {
            RenderComponent *rc = (RenderComponent *)mEntity->getComponent(ComponentType::RenderComponentType);
            rc->addStaticMesh(mesh);

            CameraComponent *camera = setupCamera(scene);
            scene->init();
            camera->observeBoundingBox(mEntity->getAABB());
        }

        return true;
    }

    bool onDestroy() override {
        delete mCanvasRenderer;
        mCanvasRenderer = nullptr;

        return true;
    }

    void onUpdate() override {
        RenderBackendService *rbSerive = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
        
        rbSerive->beginPass(RenderPass::getPassNameById(RenderPassId));
        rbSerive->beginRenderBatch("b1");
        rbSerive->setMatrix(MatrixType::Model, mTransformMatrix.mModel);
        rbSerive->endRenderBatch();
        rbSerive->endPass();

        rbSerive->beginPass(RenderPass::getPassNameById(UiPassId));
        rbSerive->beginRenderBatch("2d.b1");
        rbSerive->setMatrix(MatrixType::Model, mTransformMatrix.mModel);
        mCanvasRenderer->render(rbSerive);
        rbSerive->endRenderBatch();
        rbSerive->endPass();

        AppBase::onUpdate();
    }
};

int main(int argc, char *argv[]) {
    Demo2DApp myApp(argc, argv);
    if (!myApp.create()) {
        return 1;
    }

    while (myApp.handleEvents()) {
        myApp.update();
        myApp.requestNextFrame();
    }
    myApp.destroy();

    return 0;
}
