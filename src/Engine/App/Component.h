/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "Common/TObjPtr.h"
#include "Common/BaseMath.h"
#include "IO/Stream.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/RenderBackendService.h"

#include <cppcore/Container/TArray.h>
#include <map>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace RenderBackend {
    struct Light;
}

namespace App {

class Entity;
class SceneNode;

///	@brief This enum describes the component type.
enum class ComponentType {
    Invalid = -1,               ///< Indicates an invalid component.
    RenderComponentType = 0,    ///< A Render-component, will contain any render data.
    TransformComponentType,     ///< For all transformation types.
    CameraComponentType,        ///< For camera components.
    AnimationComponentType,     ///< For animation support.
    Count                       ///< The number of components.
};

using EntityId = guid;

struct Transform {
    EntityId mId;
    EntityId mParentId;
    glm::vec3 mTranslate;
    glm::vec3 mScale;
    glm::quat mRotation;
    glm::mat4 mLocalTransform;
    glm::mat4 mWorldTransform;

    Transform() :
            mId(), mParentId(), mTranslate(0), mScale(1), mRotation(), mLocalTransform(1), mWorldTransform(1) {
        // empty
    }
};

struct Renderable {
    EntityId mId;
    cppcore::TArray<RenderBackend::Mesh*> mRenderMeshes;
    cppcore::TArray<RenderBackend::Mesh*> mRenderUpdates;
};

struct ComponentRegistry {
    cppcore::TArray<Transform> mTransformComponents;
    cppcore::TArray<Renderable> mRenderComponents;

    ComponentRegistry() = default;
    ~ComponentRegistry() = default;
};

struct TransformSystem {
    void update(ComponentRegistry &reg) {
        for (size_t i = 0; i < reg.mTransformComponents.size(); ++i) {
            Transform &t = reg.mTransformComponents[i];
            glm::translate(t.mLocalTransform, t.mTranslate);
            glm::scale(t.mLocalTransform, t.mScale);
            const glm::mat4 r = glm::toMat4(t.mRotation);
            t.mLocalTransform *= r;
        }
    }
};

struct RenderableSystem {
    void render(ComponentRegistry& reg, RenderBackend::RenderBackendService* rbSrv) {
        for (size_t i = 0; i < reg.mRenderComponents.size(); ++i) {
            Renderable &r = reg.mRenderComponents[i];
            if (!r.mRenderMeshes.isEmpty()) {
                for (size_t j = 0; j < r.mRenderMeshes.size(); ++j) {
                    rbSrv->addMesh(r.mRenderMeshes[j], 0);
                }
                r.mRenderMeshes.clear();
                for (size_t j = 0; j < r.mRenderUpdates.size(); ++j) {
                    rbSrv->updateMesh(r.mRenderUpdates[j]);
                }
                r.mRenderUpdates.clear();
            }
        }
    }
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes the base class for all components.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Component {
public:
    /// @brief The default class destructor.
    virtual ~Component() = default;

    /// @brief The update callback
    /// @param[in] timeSlice The time slice
    virtual void update(Time timeSlice);

    /// @brief The render callback.
    /// @param[in] renderBackendSrv  The render backend service.
    virtual void render(RenderBackend::RenderBackendService *renderBackendSrv);

    /// @brief  Will return the owning entity.
    /// @return Pointer showing to the entity instance, nullptr in none.
    virtual Entity *getOwner() const;

    /// @brief  Will return the component type.
    /// @return The component type.
    virtual ComponentType getType() const;

    /// @brief  Will return the component type.
    /// @param[in] The component type.
    /// @return The type as the index.
    static size_t getIndex(ComponentType type);

protected:
    /// @brief The class constructor.
    /// @param[in] owner  The component type.
    /// @param[in] type   The owning entity.
    Component(Entity *owner, ComponentType type);

    /// @brief The update callback.
    /// @param[in] dt  The time diff.
    /// @return true if successful.
    virtual bool onUpdate(Time dt) = 0;

    /// @brief The render callback.
    /// @param renderBackendSrv   The render backend server.
    /// @return true if successful.
    virtual bool onRender(RenderBackend::RenderBackendService *renderBackendSrv) = 0;

private:
    Entity *mOwner;
    ComponentType mType;
};

inline Entity *Component::getOwner() const {
    return mOwner;
}

inline ComponentType Component::getType() const {
    return mType;
}

inline size_t Component::getIndex( ComponentType type ) {
    return static_cast<size_t>(type);
}

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes the render component
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT RenderComponent : public Component {
public:
    /// @brief The class constructor.
    /// @param owner    The owning entity.
    RenderComponent(Entity *owner);

    /// @brief The class destructor.
    ~RenderComponent() override = default;

    /// @brief  Returns the number of meshes
    /// @return The number of stored meshes.
    size_t getNumMeshes() const;

    /// @brief  Returns the mesh at a given index.
    /// @param[in] idx   The requested index.
    /// @return The mesh os a nullptr if the index in invalid.
    RenderBackend::Mesh *getMeshAt(size_t idx) const;

    /// @brief  Returns the mesh array.
    /// @param[inout] array The mesh array managed by the component.
    void getMeshArray(RenderBackend::MeshArray &array);

    /// @brief Will add a new mesh.
    /// @param mesh     Thew mesh to add.
    void addStaticMesh(RenderBackend::Mesh *mesh);

    /// @brief Will add an array of new meshes.
    /// @param array    The array with enw meshes.
    void addStaticMeshArray(const RenderBackend::MeshArray &array);

protected:
    /// The update callback.
    bool onUpdate(Time dt) override;
    /// The render callback.
    bool onRender(RenderBackend::RenderBackendService *rbSrv) override;

private:
    cppcore::TArray<RenderBackend::Mesh*> m_newGeo;
};

} // namespace App
} // namespace OSRE
