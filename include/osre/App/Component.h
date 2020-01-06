/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Scene/SceneCommon.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Common/TObjPtr.h>

#include <cppcore/Container/TArray.h>

#include <glm/glm.hpp>

namespace OSRE {
namespace App {

class Entity;

enum class ComponentType {
    RenderComponentType,     ///< Renderable component
    TransformComponentType,  ///< Transformable component
    CollisionComponent,      ///< For collision
    ScriptComponent,         ///< For scripting events
    MaxNumComponents,
    InvalidComponent
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes the render component
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Component {
public:
    virtual ~Component();
    virtual void update( Time dt );
    virtual void render( RenderBackend::RenderBackendService *renderBackendSrv );
    virtual void setId( ui32 id );
    virtual ui32 getId() const;
    virtual Entity *getOwner() const;

protected:
    Component( Entity *owner, ui32 id );
    virtual bool onPreprocess() = 0;
    virtual bool onUpdate( Time dt ) = 0;
    virtual bool onRender( RenderBackend::RenderBackendService *renderBackendSrv ) = 0;
    virtual bool onPostprocess() = 0;

private:
    Entity *m_owner;
    ui32 m_id;
};

inline
void Component::setId( ui32 id) {
    m_id = id;
}

inline
ui32 Component::getId() const {
    return m_id;
}

inline
Entity *Component::getOwner() const {
    return m_owner;
}

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes the render component
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT RenderComponent : public Component {
public:
    RenderComponent( Entity *owner, ui32 id );
    ~RenderComponent() override;
    size_t getNumGeometry() const;
    RenderBackend::Mesh *getMeshAt(size_t idx) const;
    void addStaticMesh( RenderBackend::Mesh *geo );
    void addStaticMeshArray( RenderBackend::MeshArray &array );

protected:
    bool onPreprocess() override;
    bool onUpdate( Time dt ) override;
    bool onRender( RenderBackend::RenderBackendService *rbSrv ) override;
    bool onPostprocess() override;

private:
    CPPCore::TArray<RenderBackend::Mesh*> m_newGeo;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes the render component
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT TransformComponent : public Component {
public:
    TransformComponent( Entity *owner, ui32 id );
    ~TransformComponent() override;
    void update( Time dt ) override;
    void setNode( Scene::Node *node );
    Scene::Node *getNode() const;

protected:
    bool onPreprocess() override;
    bool onUpdate( Time dt ) override;
    bool onRender( RenderBackend::RenderBackendService *rbSrv ) override;
    bool onPostprocess() override;

private:
    Common::TObjPtr<Scene::Node> m_node;
};

} // Namespace Scene
} // namespace OSRE
