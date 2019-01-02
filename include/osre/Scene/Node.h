/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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


#include <osre/Common/Common.h>
#include <osre/Scene/SceneCommon.h>
#include <osre/Common/Object.h>
#include <osre/Common/TObjPtr.h>
#include <osre/Collision/TAABB.h>
#include <osre/RenderBackend/RenderCommon.h>

#include <cppcore/Container/TArray.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {

// Forward declarations

namespace Assets {
    class Model;
}

namespace Properties {
    class Property;
}

namespace RenderBackend {
    struct TransformState;
}

namespace Scene {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class is used to represents a simple node in the stage hierarchy. You can add  
/// several functionalities by adding components to is. Each component implements functionality 
/// like render geometry or transformation information.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Node : public Common::Object {
public:
    using NodePtr = ::OSRE::Common::TObjPtr<::OSRE::Scene::Node>;
    using AABB    = ::OSRE::Collision::TAABB<f32>;

    /// The type of component
    enum class ComponentType {
        RenderComponentType,    ///< Renderable component
        TransformComponentType, ///< Transformable component
        CollisionComponent      ///< For collision 
    };

    enum class RenderCompRequest {
        RenderCompRequested,
        NoRenderComp
    };

    enum class TransformCompRequest {
        TransformCompRequested,
        NoTransformComp
    };

    enum class TraverseMode {
        FlatMode,
        RecursiveMode
    };

public:
    Node( const String &name, Common::Ids &ids, RenderCompRequest renderEnabled, 
            TransformCompRequest transformEnabled, Node *parent = nullptr );
    virtual ~Node();
    virtual void setParent( Node *parent );
    virtual Node *getParent() const;
    virtual void addChild( Node *child );
    virtual bool removeChild( const String &name, TraverseMode mode );
    virtual Node *findChild( const String &name ) const;
    virtual ui32 getNumChildren() const;
    virtual Node *getChildAt( ui32 idx ) const;
    virtual void releaseChildren();
    virtual void addModel( Assets::Model *model );
    virtual void addMesh( RenderBackend::Mesh *geo );
    virtual ui32 getNumMeshes() const;
    virtual RenderBackend::Mesh *getMeshAt(ui32 idx) const;
    virtual void update(Time dt);
    virtual void draw(RenderBackend::RenderBackendService *renderBackendSrv);
    virtual void setAABB(const AABB &aabb);
    virtual const AABB &getAABB() const;
    virtual Component *getComponent( ComponentType type ) const;
    virtual void setActive( bool isActive );
    virtual bool isActive() const;
    virtual void setProperty( Properties::Property *prop );
    virtual Properties::Property *getProperty(const String name) const;

protected:
    virtual void onUpdate(Time dt);
    virtual void onDraw(RenderBackend::RenderBackendService *renderBackendSrv);

private:
    using ChildrenArray = CPPCore::TArray<Node*>;
    using PropertyMap = CPPCore::THashMap<ui32, Properties::Property*>;

    ChildrenArray m_children;
    Node *m_parent;
    bool m_isActive;
    RenderComponent *m_renderComp;
    TransformComponent *m_transformComp;
    CPPCore::TArray<Component*> m_components;
    Common::Ids *m_ids;
    PropertyMap m_propMap;
    AABB m_aabb;
};

inline
void Node::setActive(bool isActive) {
    m_isActive = isActive;
}

inline
bool Node::isActive() const {
    return m_isActive;
}

inline
void Node::setAABB(const AABB &aabb) {
    m_aabb = aabb;
}

inline
const Node::AABB &Node::getAABB() const {
    return m_aabb;
}

class OSRE_EXPORT LightNode : public Node {
public:
    LightNode(const String &name, Common::Ids &ids, RenderCompRequest renderEnabled,
        TransformCompRequest transformEnabled, Node *parent = nullptr);
    ~LightNode();
    void setLight(const RenderBackend::Light &light);
    const RenderBackend::Light &getLight() const;

protected:
    void onUpdate(Time dt) override;
    virtual void onDraw(RenderBackend::RenderBackendService *renderBackendSrv);

private:
    RenderBackend::Light m_light;
};

} // Namespace Scene
} // namespace OSRE
