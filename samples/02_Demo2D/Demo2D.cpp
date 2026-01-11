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
DECL_OSRE_LOG_MODULE(ModelLoadingApp);

//-------------------------------------------------------------------------------------------------
///	@ingroup    Samples
///
/// @brief This smaple shows how to render 2D scenes combinet with 3d scenes.
//-------------------------------------------------------------------------------------------------
class Demo2DApp : public App::AppBase {
    /// The shader interface for all kind of transformations.
    TransformMatrixBlock  mTransformMatrix;
    /// The 2D renderer for the ui overlay
    CanvasRenderer *mCanvasRenderer = nullptr;
    /// The entity to render the 3d-scene
    Entity *mEntity = nullptr;

public:
    /// @brief The class constructor with the incoming arguments from the command line.
    Demo2DApp(int argc, char *argv[]) : AppBase(argc, const_cast<const char **>(argv)) {
        // empty
    }
    
    /// @brief The class destructor.
    ~Demo2DApp() override = default;

protected:
    /// @brief Will create the 2D scene and the 3D scene.
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

        const Color4 green(0, 1, 0, 1);
        
        mCanvasRenderer->setColor(Red);
        mCanvasRenderer->drawRect(1, 1, 1000, 760, false);

        mCanvasRenderer->setColor(green);
        mCanvasRenderer->drawRect(100, 100, 100, 40, true);

        mCanvasRenderer->drawRect(100, 200, 90, 50, true);

        mCanvasRenderer->drawRect(100, 300, 80, 60, true);

        mCanvasRenderer->drawRect(100, 400, 70, 70, true);

        mCanvasRenderer->drawRect(100, 500, 60, 60, false);

        mCanvasRenderer->selectLayer(2);
        const Color4 yellow(1, 1, 0, 1);
        mCanvasRenderer->setColor(yellow);
        mCanvasRenderer->drawRect(100, 100, 110, 30, true);

        mCanvasRenderer->drawRect(100, 600, 110, 124, true);

        mCanvasRenderer->drawRect(100, 700, 110, 124, true);
        
        mCanvasRenderer->drawRect(100, 1000, 110, 124, true);
        mCanvasRenderer->getActiveFont()->Size = 50;
        mCanvasRenderer->drawText(300, 100, 50, "Test\nAnother Test");

        mCanvasRenderer->drawText(300, 500, 50, "Test\nAnother Test");

        auto *scene = new Scene("hello_world");
        addScene(scene, true);
        mEntity = new Entity("entity", *AppBase::getIdContainer(), scene);
        MeshBuilder meshBuilder;
        auto *mesh = meshBuilder.createCube(VertexType::ColorVertex, .5, BufferAccessType::ReadOnly).getMesh();
        if (mesh != nullptr) {
            auto *rc = (RenderComponent *)mEntity->getComponent(ComponentType::RenderComponentType);
            rc->addStaticMesh(mesh);

            ui32 w = 0, h = 0;
            AppBase::getResolution(w, h);
            Rect2ui r(0,0,w,h);
            CameraComponent *camera = AppBase::setupCamera("camera", scene, r, *getIdContainer());
            scene->init();
            camera->observeBoundingBox(mEntity->getAABB());
        }

        return true;
    }
    
    /// @brief Our render update.
    void onUpdate() override {
        auto *rbService = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
        
        rbService->beginPass(RenderPass::getPassNameById(RenderPassId));
        rbService->beginRenderBatch("b1");
        rbService->setMatrix(MatrixType::Model, mTransformMatrix.mModel);
        rbService->endRenderBatch();
        rbService->endPass();

        rbService->beginPass(RenderPass::getPassNameById(UiPassId));
        rbService->beginRenderBatch("2d.b1");
        rbService->setMatrix(MatrixType::Model, mTransformMatrix.mModel);
        mCanvasRenderer->render(rbService);
        rbService->endRenderBatch();
        rbService->endPass();

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
