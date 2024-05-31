#pragma once

#include <osre/App/Component.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace App {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes the render component
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT RenderComponent : public Component {
public:
    /// @brief
    /// @param owner
    RenderComponent(Entity *owner);

    /// @brief
    ~RenderComponent() override = default;

    /// @brief
    /// @return
    size_t getNumGeometry() const;

    /// @brief
    /// @param idx
    /// @return
    RenderBackend::Mesh *getMeshAt(size_t idx) const;

    /// @brief
    /// @param array
    void getMeshArray(RenderBackend::MeshArray &array);

    /// @brief
    /// @param geo
    void addStaticMesh(RenderBackend::Mesh *geo);

    /// @brief
    /// @param array
    void addStaticMeshArray(const RenderBackend::MeshArray &array);

protected:
    bool onUpdate(Time dt) override;
    bool onRender(RenderBackend::RenderBackendService *rbSrv) override;

private:
    cppcore::TArray<RenderBackend::Mesh *> m_newGeo;
};

} // namespace App
} // namespace OSRE