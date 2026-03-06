## Animation
![Animation](../../assets/Images/01_box.png)
This sample demonstrates how to use the animation system with animation tracks and keyframes.

```cpp
#include "Animation/AnimatorBase.h"
#include "Animation/AnimatorComponent.h"
#include "App/App.h"
#include "RenderBackend/MeshBuilder.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/TransformMatrixBlock.h"

using namespace ::OSRE;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::App;

DECL_OSRE_LOG_MODULE(AnimationApp);

class AnimationApp : public App::AppBase {
    TransformMatrixBlock mTransformMatrix;
    Entity *mEntity{ nullptr };
    Animation::AnimationControllerBase *mKeyboardTransCtrl{ nullptr };
    Animation::AnimationTrack mTrack;
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
        rot.Time = 1000.0f;
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
```

## Key Concepts
The animation system uses animation tracks with keyframes. Keys contain transformation data (position, rotation, scale) at specific times, allowing smooth interpolation between keyframes.
