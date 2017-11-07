/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <cppcore/Container/TArray.h>

#include <glm/glm.hpp>

namespace OSRE {
    
namespace RenderBackend {
    class RenderBackendService;

    struct UniformVar;
    struct Geometry;
}

namespace Scene {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes the render component
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Component {
public:
    virtual ~Component();
    virtual void update( RenderBackend::RenderBackendService *renderBackendSrv ) = 0;
    void setId( ui32 id );
    ui32 getId() const;

protected:
    Component( ui32 id );

private:
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

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes the render component
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT RenderComponent : public Component {
public:
    RenderComponent( ui32 id );
    virtual ~RenderComponent();
    void update( RenderBackend::RenderBackendService *renderBackendSrv ) override;
    void addStaticGeometry( RenderBackend::Geometry *geo );

private:
    CPPCore::TArray<RenderBackend::Geometry*> m_newGeo;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes the transformation component.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT TransformComponent : public Component {
public:
    TransformComponent( ui32 id );
    virtual ~TransformComponent();
    void update( RenderBackend::RenderBackendService *renderBackendSrv ) override;
    void setPosition( const glm::vec3 &pos );
    const glm::vec3 &getPosition() const;
    void setScale( const glm::vec3 &pos );
    const glm::vec3 &getScale() const;

private:
    RenderBackend::TransformBlock m_localTransform;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes the collision component.
//-------------------------------------------------------------------------------------------------
class CollisionComponent : public Component {
public:
    CollisionComponent( ui32 id );
    virtual ~CollisionComponent();
    void update( RenderBackend::RenderBackendService *renderBackendSrv ) override;
};

} // Namespace Scene
} // namespace OSRE
