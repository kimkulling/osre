#pragma once

#include <osre/App/AppBase.h>
#include <osre/App/World.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/View.h>
#include <osre/RenderBackend/RenderCommon.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {

namespace App {
    class Project;
}

namespace UI {
    class Canvas;
    class Panel;
}

namespace IO {
    class Uri;
}

namespace Scene {
    class TrackBall;
}

namespace Editor {

class ModuleBase;

class OsreEdApp : public App::AppBase {
    using ModuleArray = ::CPPCore::TArray<ModuleBase *>;
    using ModulePathArray = ::CPPCore::TArray<String>;

public:
    OsreEdApp(int argc, char *argv[]);
    ~OsreEdApp() override;
    bool addModulePath(const String &path);
    bool registerModule(ModuleBase *mod);
    bool loadModules();
    void newProject(ui32, void *);
    void loadProject(ui32, void *);
    void saveProject(ui32, void *);
    void importAsset(ui32, void *);
    void quitEditor(ui32, void *);
    void loadAsset(const IO::Uri &modelLoc);
    bool hasModel() const {
        return m_modelNode.isValid();
    }

protected:
    bool onCreate() override;
    void onUpdate() override;
    bool onDestroy() override;

private:
    ModuleArray mModuleArray;
    ModulePathArray mModulePathArray;

    Scene::Stage *mStage;
    Scene::View *m_view;
    f32 m_angle;
    glm::mat4 m_model;
    RenderBackend::TransformMatrixBlock m_transformMatrix;
    Scene::Node::NodePtr m_modelNode;
    Scene::TrackBall *mTrackBall;
    App::Project *mProject;
};

} // namespace Editor
} // namespace OSRE
