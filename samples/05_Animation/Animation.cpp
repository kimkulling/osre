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
#include "Animation/AnimatorBase.h"
#include "Animation/AnimatorComponent.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/TransformMatrixBlock.h"

//
using namespace ::OSRE;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::App;

// To identify local log entries we will define this tag.
static constexpr c8 Tag[] = "TerrainRenderingApp";

//-------------------------------------------------------------------------------------------------
///	@ingroup    Samples
///
/// @brief
//-------------------------------------------------------------------------------------------------
class AnimationApp : public App::AppBase {
    /// The transform block, contains the model-, view- and projection-matrix
    TransformMatrixBlock mTransformMatrix;
    /// The entity to render
    Entity *mEntity;
    /// The keyboard controller instance.
    Animation::AnimationControllerBase *mKeyboardTransCtrl;
    Animation::AnimationTrack mTrack;
    f32 mAngle;

public:
    AnimationApp(int argc, char *argv[]) :
            AppBase(argc, const_cast<const char **>(argv)) {}

    ~AnimationApp() override = default;

protected:
    bool onCreate() override {
        if (!AppBase::onCreate()) {
            return false;
        }

        AppBase::setWindowsTitle("Animation sample! Rotate with keyboard: w, a, s, d, scroll with q, e");
        Scene *scene = new Scene("hello_world");
        addScene(scene, true);

        Entity *camEntity = new Entity("camera", *getIdContainer(), scene);
        scene->addEntity(camEntity);
        CameraComponent *camera = static_cast<CameraComponent *>(camEntity->createComponent(ComponentType::CameraComponentType));
        scene->setActiveCamera(camera);

        Animation::AnimatorComponent *animator = static_cast<Animation::AnimatorComponent *>(camEntity->createComponent(ComponentType::AnimationComponentType));
        mTrack.NumVectorChannels = 1;
        mTrack.AnimationChannels = new Animation::AnimationChannel[mTrack.NumVectorChannels];
        mTrack.Duration = 1.0f;
        Animation::AnimationChannel channel;
        Animation::RotationKey rot;
        rot.Quad = glm::angleAxis(glm::radians(mAngle), glm::vec3(0.f, 1.f, 0.f));
        rot.Time = 1.0f;
        mTrack.AnimationChannels[0].RotationKeys.add(rot);
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
