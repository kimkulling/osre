#pragma once

#include <osre/App/AppBase.h>
#include <osre/App/World.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/Camera.h>
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

//-------------------------------------------------------------------------------------------------
/// @brief  The main application of the OSRE-Editor.
//-------------------------------------------------------------------------------------------------
class OsreEdApp : public App::AppBase {
public:
    /// Local usings
    using ModuleArray = ::CPPCore::TArray<ModuleBase *>;
    using ModulePathArray = ::CPPCore::TArray<String>;

    /// @brief  The class constructor.
    /// @param  argc    [in] The number of command-line parameters.
    /// @param  argv    [in] The command-line arguments.
    OsreEdApp(int argc, char *argv[]);

    /// @brief  The class destructor.
    ~OsreEdApp() override;

    /// The New-project callback.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void newProject(ui32 cmdId, void *data);

    /// The Load-project callback.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void loadProject(ui32 cmdId, void *data);

    /// The Save-project callback.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void saveProject(ui32 cmdId, void *data);

    /// The Import-Asset callback.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void importAsset(ui32 cmdId, void *data);

    /// The Quit callback.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void quitEditor(ui32 cmdId, void *data);

    /// @brief  Will load an asset from a given uri.
    /// @para   modelLoc    [in] The model location.
    void loadAsset(const IO::Uri &modelLoc);

    /// @brief  Will return true, if a model was loaded.
    bool hasModel() const;

protected:
    /// The onCreate callback
    bool onCreate() override;

    /// The onUpdate callback.
    void onUpdate() override;

    /// The onDestrox callback.
    bool onDestroy() override;

private:
    Scene::Camera *mCamera;
    glm::mat4 m_model;
    RenderBackend::TransformMatrixBlock m_transformMatrix;
    Scene::Node::NodePtr m_modelNode;
    Scene::TrackBall *mTrackBall;
    App::Project *mProject;
    Vec2f mOld;
    bool mLast;
};

inline bool OsreEdApp::hasModel() const {
    return m_modelNode.isValid();
}

} // namespace Editor
} // namespace OSRE
