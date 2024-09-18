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
#include "App/SceneCommon.h"

#include <cppcore/Container/TArray.h>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace RenderBackend {
    struct Light;
}

namespace App {

class Entity;
class TransformComponent;

///	@brief This enum describes the component type.
enum class ComponentType {
    Invalid = -1,               ///< Indicates an invalid component.
    RenderComponentType = 0,    ///< A Render-component, will contain any render data.
    TransformComponentType,     ///< For all transformation types.
    LightComponentType,         ///< For light types.
    CameraComponentType,        ///< For camera components.
    AnimationComponentType,     ///<
    Count                       ///< The number of components.
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
    cppcore::TArray<RenderBackend::Mesh*> m_newGeo;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
/// @brief
//-------------------------------------------------------------------------------------------------
class LightComponent final : public Component {
public:
    /// @brief 
    /// @param owner 
    LightComponent(Entity *owner);
    
    /// @brief 
    ~LightComponent() override = default;
    
    /// @brief 
    /// @param light 
    void setLight(RenderBackend::Light *light);

protected:
    bool onUpdate(Time dt) override;
    bool onRender(RenderBackend::RenderBackendService *rbSrv) override;

private:
    RenderBackend::Light *mLight;
};

} // namespace App
} // namespace OSRE
