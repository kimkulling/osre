/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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

namespace IO {
    class Uri;
}

namespace Scene {
    class World;
}

namespace Editor {

struct SceneData {
    String Name;
    Scene::Node::NodePtr m_modelNode;
    Scene::Camera *mCamera;
    Scene::World *mWorld;

    SceneData();
};

//-------------------------------------------------------------------------------------------------
/// @brief  The main application of the OSRE-Editor.
//-------------------------------------------------------------------------------------------------
class OsreEdApp : public App::AppBase {
public:
    /// brief   The module list
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
    void createUI();

protected:
    /// The onCreate callback
    bool onCreate() override;
    /// The onUpdate callback.
    void onUpdate() override;
    /// The onDestrox callback.
    bool onDestroy() override;

    bool loadSceneData(const IO::Uri &filename, SceneData &sd);
    
    bool saveSceneData(const IO::Uri &filename, SceneData &sd);

private:
    glm::mat4 m_model;
    RenderBackend::TransformMatrixBlock m_transformMatrix;
    SceneData mSceneData;
    App::Project *mProject;
    ModuleArray mModules;
    Rect2ui mResolution;
    RenderBackend::Mesh *mMesh2D;
};

inline bool OsreEdApp::hasModel() const {
    return mSceneData.m_modelNode.isValid();
}

} // namespace Editor
} // namespace OSRE
