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
#include "App/App.h"
#include "IO/Uri.h"
#include "Common/BaseMath.h"
#include "Platform/AbstractWindow.h"
#include "Platform/PlatformOperations.h"
#include "Properties/Settings.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/TransformMatrixBlock.h"

#include <assimp/scene.h>

#include <iostream>

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

DECL_OSRE_LOG_MODULE(ModelLoadingApp)

class SceneDumper {
    /// The intention. 
    int mIntention = 0;
    
public:
    /// @brief The class constructor.
    SceneDumper() = default;

    /// @brief Will push intention.
    void pushIntention() {
        mIntention++;
    }
    
    /// @brief Will pop intention.
    void popIntention() {
        mIntention--;
    }

    /// @brief Evaluates the name of the node.
    /// @param[in] name     The name to evaluate.
    void checkName(String &name) {
        if (name.empty()) {
            name = "No Name";
        }
    }

    /// @brief Will dump the single name.
    /// @param[in] node     The node to dump.
    void dumpNode(aiNode &node) {
        String name = node.mName.C_Str();
        checkName(name);
        for (int i=0; i<mIntention; ++i) {
            std::cout << "+-";
        }
        
        std::cout << "Node name: " <<  name << "\n";
        for (unsigned int i=0; i < node.mNumChildren; ++i) {
            pushIntention();
            dumpNode(*node.mChildren[i]);
            popIntention();
        }
    }

    /// @brief Will show the statistic.
    /// @param[in] scene    The scene to show statistics.
    void showStatistics(const aiScene &scene) {
        std::cout << "Model name: " << scene.mName.C_Str() << "\n";
        std::cout << "=============================================================\n";

        if (scene.mRootNode != nullptr) {
            dumpNode(*scene.mRootNode);
        }

        std::cout << "Number of meshes     : " << scene.mNumMeshes << "\n";
        std::cout << "Number of materials  : " << scene.mNumMaterials << "\n";
        std::cout << " +-Number of embedded textures : " << scene.mNumTextures << "\n";
        std::cout << "Number of animations : " << scene.mNumAnimations << "\n";
    }
};

//-------------------------------------------------------------------------------------------------
///	@ingroup    Samples
///
/// @brief  This sample shows how to load a model using the Assimp wrapper.
///  
/// It will also show some statistics about the loaded model. The model will get loaded, the
/// camera will be placed to get an optimal view onto the model. The model will be rendered.
//-------------------------------------------------------------------------------------------------
class ModelLoadingApp : public App::AppBase {
    /// The asset folder, here we will locate our assets.
    String mAssetFolder;
    /// The camera component.
    CameraComponent *mCamera = nullptr;
    /// The transform block.
    TransformMatrixBlock mTransformMatrix;
    /// The mode node.
    TransformComponent::NodePtr mModelNode;
    /// The controller for the keyboard.
    Animation::AnimationControllerBase *mKeyboardTransCtrl = nullptr;
    /// The mouse controller for rotating the model.
    OrbitalMouseControl *mOrbitalMouseControl = nullptr;

public:
    /// @brief The class constructor.
    /// @param argc     The number of arguments.
    /// @param argv     The argument array.
    ModelLoadingApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv, "api", "The render API") {
        // empty
    }

    /// @brief The class destructor.
    ~ModelLoadingApp() override {
        delete mOrbitalMouseControl;
    }

    /// @brief  Will return true, if a model was loaded.
    /// @return true for model loaded.
    bool hasModel() const {
        return mModelNode != nullptr;
    }


protected:
    bool onCreate() override {
        if (!AppBase::onCreate()) {
            return false;
        }

        mKeyboardTransCtrl = AppBase::getTransformController(mTransformMatrix);
        if(mKeyboardTransCtrl == nullptr) {
            osre_error(Tag, "Failed to initialize keyboard transform controller.");
            return false;
        }

        mOrbitalMouseControl = new OrbitalMouseControl(&mTransformMatrix);
        return true;
    }

    void loadAsset(const IO::Uri &modelLoc) {
        AssimpWrapper assimpWrapper(*getIdContainer(), getActiveScene());
        if (!assimpWrapper.importAsset(modelLoc, 0)) {
            return;
        }

        auto *rbSrv = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
        if (nullptr == rbSrv) {
            osre_error(Tag, "RenderBackendService not available.");
            return;
        }
        Platform::AbstractWindow *rootWindow = getRootWindow();
        if (nullptr == rootWindow) {
            osre_error(Tag, "Root window not available.");
            return;
        }

        Rect2i rect;
        rootWindow->getWindowsRect(rect);
        auto *scene = new Scene("model");
        addScene(scene, true);
        mCamera = setupCamera("camera", scene, rect, *getIdContainer());

        auto *entity = assimpWrapper.getEntity();
        scene->addEntity(entity);
        mCamera->observeBoundingBox(entity->getAABB());
        mModelNode = entity->getNode();

        SceneDumper sd;
        sd.showStatistics(*assimpWrapper.getScene());
    }

    void onUpdate() override {
        if (AppBase::isKeyPressed(Platform::KEY_O) || AppBase::isKeyPressed(Platform::KEY_o)) {
            IO::Uri modelLoc;
            Platform::PlatformOperations::getFileOpenDialog("Choose asset for import", "*", modelLoc);
            if ( modelLoc.isValid()) {
                loadAsset(modelLoc);
            }
        }

        if (Platform::Key key = AppBase::getKeyboardEventListener()->getLastKey(); key != Platform::KEY_UNKNOWN) {
            mKeyboardTransCtrl->update(mKeyboardTransCtrl->getKeyBinding(key));
        }

        auto *rbSrv = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
        if (nullptr == rbSrv) {
            osre_error(Tag, "RenderBackendService not available.");
            return;
        }

        if (mCamera != nullptr) {
            mOrbitalMouseControl->update(AppBase::getMouseEventListener(), mCamera->getRight(), mCamera->getUp());
        }
        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        rbSrv->beginRenderBatch("b1");

        rbSrv->setMatrix(MatrixType::Model, mTransformMatrix.mModel);

        rbSrv->endRenderBatch();
        rbSrv->endPass();

        AppBase::onUpdate();
    }
};

int main(int argc, char *argv[]) {
    ModelLoadingApp myApp(argc, argv);
    if (!myApp.initWindow(10, 10, 1024, 768, "ModelLoader sample! Press o to import an Asset", 
            WindowMode::Windowed, WindowType::Root, RenderBackendType::OpenGLRenderBackend)) {
        osre_error(Tag, "Error while creating window.");
        return 1;
    }

    while (myApp.handleEvents()) {
        myApp.update();
        myApp.requestNextFrame();
    }

    myApp.destroy();

    return 0;
}
