#pragma once

#include "SceneData.h"
#include "App/AppBase.h"
#include "App/Project.h"
#include "App/CameraComponent.h"
#include "App/Entity.h"
#include "App/World.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/TransformMatrixBlock.h"
#include "RenderBackend/RenderBackendService.h"

namespace OSRE {
namespace Editor {

class OsreEdApp : public App::AppBase {
public:
    OsreEdApp(int argc, char *argv[]);
    ~OsreEdApp() override = default;
    App::CameraComponent *setupCamera(App::World *world);
    bool onCreate() override;
    void onUpdate() override;
    void newProjectCmd(ui32, void *data);
    void loadAsset(const IO::Uri &modelLoc);

private:
    App::Project *mProject;
    RenderBackend::TransformMatrixBlock mTransformMatrix;
    App::Entity *mEntity;
    Animation::AnimationControllerBase *mKeyboardTransCtrl;
    SceneData mSceneData;
};

} // namespace Editor
} // namespace OSRE
