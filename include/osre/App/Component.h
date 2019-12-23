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
#include <cppcore/Container/TArray.h>

#include <glm/glm.hpp>

namespace OSRE {
namespace Scene {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes the render component
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Component {
public:
    virtual ~Component();
    virtual void update( Time dt ) = 0;
    virtual void draw( RenderBackend::RenderBackendService *renderBackendSrv ) = 0;
    void setId( ui32 id );
    ui32 getId() const;
    Node *getOwnerNode() const;

protected:
    Component( Node *node, ui32 id );

private:
    Node *m_owner;
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
Node *Component::getOwnerNode() const {
    return m_owner;
}

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes the render component
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT RenderComponent : public Component {
public:
    RenderComponent(Node *node, ui32 id );
    virtual ~RenderComponent();
    void update( Time dt ) override;
    void draw( RenderBackend::RenderBackendService *renderBackendSrv ) override;
    size_t getNumGeometry() const;
    RenderBackend::Mesh *getGeoAt(size_t idx) const;
    void addStaticGeometry( RenderBackend::Mesh *geo );

private:
    CPPCore::TArray<RenderBackend::Mesh*> m_newGeo;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes the transformation component.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT TransformComponent : public Component {
public:
    TransformComponent(Node *node, ui32 id );
    virtual ~TransformComponent();
    void update( Time dt ) override;
    void draw( RenderBackend::RenderBackendService *renderBackendSrv ) override;
    void setTranslation( const glm::vec3 &pos );
    const glm::vec3 &getTranslation() const;
    void setScale( const glm::vec3 &pos );
    const glm::vec3 &getScale() const;
    void setTransformationMatrix(const glm::mat4 &m);
    const glm::mat4 &getTransformationMatrix() const;
    glm::mat4 getWorlTransformMatrix();
    const RenderBackend::TransformState &getTransformState() const;

private:
    enum DirtyFrag {
        NotDirty = 0,
        NeedsTransform = 1
    };
    ui32 m_dirty;
    RenderBackend::TransformState m_localTransformState;
    glm::mat4 m_transform;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes the collision component.
//-------------------------------------------------------------------------------------------------
class CollisionComponent : public Component {
public:
    CollisionComponent(Node *node, ui32 id );
    virtual ~CollisionComponent();
    void update( Time dt ) override;
};

class LightComponent : public Component {
public:
    LightComponent(Node *node, ui32 id);
    virtual ~LightComponent();
    void update(Time dt) override;
    void setLight( RenderBackend::Light &light );
    const RenderBackend::Light &getLight() const;
    void setRadius(f32 r);
    f32 getRadius() const;

private:
    RenderBackend::Light m_light;
    f32 m_radius;
};

} // Namespace Scene
} // namespace OSRE
