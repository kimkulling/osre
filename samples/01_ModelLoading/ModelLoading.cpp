/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/App/App.h>
#include <osre/Scene/Scene.h>
#include <osre/IO/Uri.h>
#include <osre/Common/BaseMath.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/PlatformOperations.h>
#include <osre/Properties/Settings.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

/// @brief The log-tag
static constexpr c8 Tag[] = "ModelLoadingApp";

/// @brief The example application, will create the renderer and loads a model.
class ModelLoadingApp : public App::AppBase {
    String mAssetFolder;                    ///< The asset folder, here we will locate our assets.
    App::CameraComponent *mCamera;          ///< The camera component.
    TransformMatrixBlock mTransformMatrix;  ///< The tansform block.
    TransformComponent::NodePtr mModelNode; ///< The mode node.

public:
    ModelLoadingApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv, "api", "The render API"),
            mAssetFolder(),
            mCamera(nullptr),
            mTransformMatrix(),
            mModelNode()  {
        // empty
    }

    ~ModelLoadingApp() override = default;

    bool hasModel() const {
        return mModelNode.isValid();
    }

protected:
    void loadAsset(const IO::Uri &modelLoc) {
        AssimpWrapper assimpWrapper(*getIdContainer(), getStage()->getActiveWorld(0));
        if (!assimpWrapper.importAsset(modelLoc, 0)) {
            return;
        }

        RenderBackendService *rbSrv = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
        if (nullptr == rbSrv) {
            return;
        }
        Platform::AbstractWindow *rootWindow = getRootWindow();
        if (nullptr == rootWindow) {
            return;
        }

        Rect2ui windowsRect;
        rootWindow->getWindowsRect(windowsRect);
        World *world = getStage()->addActiveWorld("model");
        Entity *entity = assimpWrapper.getEntity();
        Entity *camEntity = new Entity("camera", *getIdContainer(), world);
        mCamera = (CameraComponent*)camEntity->createComponent(ComponentType::CameraComponentType);
        mCamera->setProjectionParameters(60.f, (f32)windowsRect.width, (f32)windowsRect.height, 0.01f, 1000.f);
        world->setActiveCamera(mCamera);

        world->addEntity(entity);
        mCamera->observeBoundingBox(entity->getAABB());
        mModelNode = entity->getNode();
    }
    
    void onUpdate() override {
        if (AppBase::isKeyPressed(Platform::KEY_O)) {
            IO::Uri modelLoc;
            Platform::PlatformOperations::getFileOpenDialog("Choose asset for import", "*", modelLoc);
            if ( modelLoc.isValid()) {
                loadAsset(modelLoc);
            }
        }

        glm::mat4 rot(1.0);
        if (AppBase::isKeyPressed(Platform::KEY_A)) {
            mTransformMatrix.m_model *= glm::rotate(rot, 0.01f, glm::vec3(1, 0, 0));

        }
        if (AppBase::isKeyPressed(Platform::KEY_S)) {
            mTransformMatrix.m_model *= glm::rotate(rot, -0.01f, glm::vec3(1, 0, 0));
        }
        
        if (AppBase::isKeyPressed(Platform::KEY_W)) {
            mTransformMatrix.m_model *= glm::rotate(rot, 0.01f, glm::vec3(0, 1, 0));
        }

        if (AppBase::isKeyPressed(Platform::KEY_D)) {
            mTransformMatrix.m_model *= glm::rotate(rot, -0.01f, glm::vec3(0, 1, 0));
        }
        RenderBackendService *rbSrv = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);

        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        rbSrv->beginRenderBatch("b1");

        rbSrv->setMatrix(MatrixType::Model, mTransformMatrix.m_model);

        rbSrv->endRenderBatch();
        rbSrv->endPass();

        AppBase::onUpdate();
    }
};

int main(int argc, char *argv[]) {
    ModelLoadingApp myApp(argc, argv);
    if (!myApp.initWindow(10, 10, 1024, 768, "ModelLoader sample! Press o to import an Asset", false, false, App::RenderBackendType::OpenGLRenderBackend)) {
        return 1;
    }

    while (myApp.handleEvents()) {
        myApp.update();
        //if (myApp.hasModel()) {
            myApp.requestNextFrame();
        //}
    }

    myApp.destroy();

    return 0;
}

