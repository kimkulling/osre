/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/TObjPtr.h>
#include <osre/Common/BaseMath.h>
#include <osre/IO/Stream.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/SceneCommon.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace RenderBackend {
    struct Light;
}

namespace App {

class Entity;
class TransformComponent;

///	@brief
enum class ComponentType {
    InvalidComponent = -1,      ///< Indicates an invalid component.
    RenderComponentType = 0,    ///< A Render-component, will contain any render data.
    TransformComponentType,     ///< For all transformation types.
    LightComponentType,         ///< For light types.
    ScriptComponentType,        ///< For scripting components.

    MaxNumComponents,           ///< The number of components.
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describe the base class for all components.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Component {
public:
    /// @brief The default class destructor.
    virtual ~Component() = default;

    /// @brief 
    /// @return 
    virtual bool preprocess();
    
    /// @brief 
    /// @param dt 
    virtual void update(Time dt);
    
    /// @brief 
    /// @param[in] renderBackendSrv 
    virtual void render(RenderBackend::RenderBackendService *renderBackendSrv);
    
    /// @brief 
    /// @return 
    virtual bool postprocess();
    
    /// @brief 
    /// @return 
    virtual Entity *getOwner() const;
    
    /// @brief 
    /// @return 
    virtual ComponentType getType() const;

    /// @brief 
    /// @param type 
    /// @return 
    static size_t getIndex(ComponentType type);

protected:
    /// @brief 
    /// @param owner 
    /// @param type 
    Component(Entity *owner, ComponentType type);

    /// @brief 
    /// @return 
    virtual bool onPreprocess() = 0;
    
    /// @brief 
    /// @param dt 
    /// @return 
    virtual bool onUpdate(Time dt) = 0;
    
    /// @brief 
    /// @param renderBackendSrv 
    /// @return 
    virtual bool onRender(RenderBackend::RenderBackendService *renderBackendSrv) = 0;
    
    /// @brief 
    /// @return 
    virtual bool onPostprocess() = 0;
    
    /// @brief 
    /// @param stream 
    virtual void serialize(IO::Stream &stream);
    
    /// @brief 
    /// @param stream 
    virtual void deserialize(IO::Stream &stream);

private:
    Entity *m_owner;
    ComponentType mType;
};

inline bool Component::preprocess() {
    return onPreprocess();
}

inline bool Component::postprocess() {
    return onPostprocess();
}

inline Entity *Component::getOwner() const {
    return m_owner;
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
    bool onPreprocess() override;    
    bool onUpdate(Time dt) override;
    bool onRender(RenderBackend::RenderBackendService *rbSrv) override;
    bool onPostprocess() override;

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
    bool onPreprocess() override;
    bool onUpdate(Time dt) override;
    bool onRender(RenderBackend::RenderBackendService *rbSrv) override;
    bool onPostprocess() override;

private:
    RenderBackend::Light *mLight;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
/// @brief
//-------------------------------------------------------------------------------------------------
class ScriptComponent final : public Component {
public:
    /// @brief 
    /// @param owner 
    ScriptComponent(Entity *owner);
    
    /// @brief 
    ~ScriptComponent() override = default;

protected:
    bool onPreprocess() override;
    bool onUpdate(Time dt) override;
    bool onRender(RenderBackend::RenderBackendService *rbSrv) override;
    bool onPostprocess() override;

private:
    // todo!
};

} // namespace App
} // namespace OSRE
