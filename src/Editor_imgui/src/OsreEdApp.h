#pragma once

#include "SceneData.h"
#include <osre/App/AppBase.h>
#include <osre/App/Project.h>
#include <osre/App/CameraComponent.h>
#include <osre/App/Entity.h>
#include <osre/App/World.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/TransformMatrixBlock.h>
#include <osre/RenderBackend/RenderBackendService.h>

namespace OSRE {
namespace Editor {

class OsreEdApp : public App::AppBase {
public:
    OsreEdApp(int argc, char *argv[]);
    ~OsreEdApp() override = default;
    App::Camera *setupCamera(App::World *world);
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
