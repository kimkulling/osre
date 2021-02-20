#pragma once

#include "Modules/ModuleBase.h"

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

//-------------------------------------------------------------------------------------------------
/// @brief  The main application of the OSRE-Editor.
//-------------------------------------------------------------------------------------------------
class OsreEdApp : public App::AppBase {
public:
    using ModulePathArray = ::CPPCore::TArray<String>;

    /// @brief  The class constructor with the incoming command line arguments.
    /// @param  argc    [in] The number of command-line parameters.
    /// @param  argv    [in] The command-line arguments.
    OsreEdApp(int argc, char *argv[]);

    /// @brief  The class destructor.
    ~OsreEdApp() override;

    /// The New-project command.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void newProjectCmd(ui32 cmdId, void *data);

    /// The Load-project command.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void loadProjectCmd(ui32 cmdId, void *data);

    /// The Save-project command.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void saveProjectCmd(ui32 cmdId, void *data);

    /// The Import-Asset command.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void importAssetCmd(ui32 cmdId, void *data);

    /// The Quit command.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void quitEditorCmd(ui32 cmdId, void *data);

    /// @brief  Will load an asset from a given uri.
    /// @para   modelLoc    [in] The model location.
    void loadAsset(const IO::Uri &modelLoc);

    /// @brief  The Getting help command implementation.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void gettingHelpCmd(ui32 cmdId, void *data);

    /// @brief  The show version command.
    /// @param  cmdId   [in] The command id.
    /// @param  data    [in] The parameters.
    void showVersionCmd(ui32 cmdId, void *data);

    /// @brief  Will return true, if a model was loaded.
    /// @return true, if a model is stored.
    bool hasModel() const;

    bool registerModule(ModuleBase *mod);
    ModuleBase *findModule(const String &name) const;
    bool unregisterModule(ModuleBase *moc);

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
    ModuleArray mModules;
};

inline bool OsreEdApp::hasModel() const {
    return m_modelNode.isValid();
}

} // namespace Editor
} // namespace OSRE
