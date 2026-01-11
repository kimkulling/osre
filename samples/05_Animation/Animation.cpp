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
#include "Animation/AnimatorBase.h"
#include "Animation/AnimatorComponent.h"
#include "App/App.h"
#include "RenderBackend/MeshBuilder.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/TransformMatrixBlock.h"

//
using namespace ::OSRE;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::App;

// To identify local log entries we will define this tag.
DECL_OSRE_LOG_MODULE(AnimationApp);

//-------------------------------------------------------------------------------------------------
///	@ingroup    Samples
///
/// @brief
//-------------------------------------------------------------------------------------------------
class AnimationApp : public App::AppBase {
    /// The transform block, contains the model-, view- and projection-matrix
    TransformMatrixBlock mTransformMatrix;
    /// The entity to render
    Entity *mEntity{ nullptr };
    /// The keyboard controller instance.
    Animation::AnimationControllerBase *mKeyboardTransCtrl{ nullptr };
    /// The used animation track
    Animation::AnimationTrack mTrack;
    /// The rotation angle
    f32 mAngle{0.1};

public:
    AnimationApp(int argc, char *argv[]) :
            AppBase(argc, const_cast<const char **>(argv)) {}

    ~AnimationApp() override = default;

protected:
    bool onCreate() override {
        if (!AppBase::onCreate()) {
            return false;
        }

        AppBase::setWindowsTitle("Animation sample! Rotate with keyboard: w, a, s, d, zoom with q, e");
        Scene *scene = new Scene("hello_world");
        addScene(scene, true);

        Entity *camEntity = new Entity("camera", *getIdContainer(), scene);
        mEntity = new Entity("entity", *AppBase::getIdContainer(), scene);
        scene->addEntity(camEntity);
        MeshBuilder meshBuilder;
        
        if (Mesh *mesh = meshBuilder.createCube(VertexType::ColorVertex, 0.8, BufferAccessType::ReadOnly).getMesh(); mesh != nullptr) {
            auto *rc = static_cast<RenderComponent*>(mEntity->getComponent(ComponentType::RenderComponentType));
            rc->addStaticMesh(mesh);
        }
        CameraComponent *camera = static_cast<CameraComponent*>(camEntity->createComponent(ComponentType::CameraComponentType));
        scene->setActiveCamera(camera);

        Animation::AnimatorComponent *animator = static_cast<Animation::AnimatorComponent *>(camEntity->createComponent(ComponentType::AnimationComponentType));
        mTrack.numVectorChannels = 1;
        mTrack.animationChannels = new Animation::AnimationChannel[mTrack.numVectorChannels];
        mTrack.duration = 1000.0f;
        Animation::AnimationChannel channel;
        Animation::RotationKey rot;
        rot.Quad = angleAxis(glm::radians(mAngle), glm::vec3(1.f, 0.0f, 0.f));
        rot.Time = 1.0f;
        mTrack.animationChannels[0].RotationKeys.add(rot);
        animator->addTrack(&mTrack);

        return true;
    }
};

int main(int argc, char *argv[]) {
    AnimationApp myApp(argc, argv);
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
