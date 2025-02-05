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
#pragma once

#include "SceneData.h"
#include "App/AppBase.h"
#include "App/Project.h"
#include "App/CameraComponent.h"
#include "App/Entity.h"
#include "App/Scene.h"
#include "RenderBackend/TransformMatrixBlock.h"

namespace OSRE {
namespace Editor {

class OsreEdApp final : public App::AppBase {
public:
    OsreEdApp(int argc, char *argv[]);
    ~OsreEdApp() override = default;
    App::CameraComponent *setupCamera(App::Scene *world);
    bool onCreate() override;
    void onUpdate() override;
    void newProjectCmd(ui32, void *data);
    void loadAsset(const IO::Uri &modelLoc);

private:
    struct Config {
        f32 mFov = 60.f;
        f32 mNear = 0.01f;
        f32 mFar = 10000.0f;
    } mConfig;

    App::Project *mProject;
    RenderBackend::TransformMatrixBlock mTransformMatrix;
    App::Entity *mEntity;
    App::Entity *mGuiEntity;
    Animation::AnimationControllerBase *mKeyboardTransCtrl;
    SceneData mSceneData;
};

} // namespace Editor
} // namespace OSRE
