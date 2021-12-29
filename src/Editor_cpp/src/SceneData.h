#pragma once

#include <osre/Common/osre_common.h>
#include <osre/Scene/Node.h>
#include <osre/App/World.h>

namespace OSRE {
namespace Editor {

struct SceneData {
    String Name;
    String ProjectName;
    String AssetName;
    Scene::Node::NodePtr m_modelNode;
    Scene::Camera *mCamera;
    App::World *mWorld;

    SceneData();
};

inline SceneData::SceneData() :
        Name(),
        ProjectName("none"),
        AssetName("none"),
        m_modelNode(),
        mCamera(nullptr),
        mWorld(nullptr) {
    // empty
}

} // namespace Editor
} // namespace OSRE
