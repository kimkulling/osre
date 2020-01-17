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


#include <osre/Common/Common.h>
#include <osre/Scene/SceneCommon.h>
#include <osre/Scene/TAABB.h>
#include <osre/Common/Object.h>
#include <osre/Common/TObjPtr.h>
#include <osre/RenderBackend/RenderCommon.h>

#include <cppcore/Container/TArray.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
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
    using AABB    = ::OSRE::Scene::TAABB<f32>;

    enum class TraverseMode {
        FlatMode,
        RecursiveMode
    };

public:
    Node( const String &name, Common::Ids &ids, Node *parent = nullptr );
    virtual ~Node();
    virtual void setParent( Node *parent );
    virtual Node *getParent() const;
    virtual void addChild( Node *child );
    virtual bool removeChild( const String &name, TraverseMode mode );
    virtual Node *findChild( const String &name ) const;
    virtual size_t getNumChildren() const;
    virtual Node *getChildAt( size_t idx ) const;
    virtual void releaseChildren();
    virtual void update(Time dt);
    virtual void render(RenderBackend::RenderBackendService *renderBackendSrv);
    virtual void setAABB(const AABB &aabb);
    virtual const AABB &getAABB() const;
    virtual void setActive( bool isActive );
    virtual bool isActive() const;
    virtual void setProperty( Properties::Property *prop );
    virtual Properties::Property *getProperty(const String name) const;

    void setTranslation( const glm::vec3 &pos );
    const glm::vec3 &getTranslation() const;
    void setScale( const glm::vec3 &pos );
    const glm::vec3 &getScale() const;
    void setTransformationMatrix( const glm::mat4 &m );
    const glm::mat4 &getTransformationMatrix() const;
    glm::mat4 getWorlTransformMatrix();
    const RenderBackend::TransformState &getTransformState() const;

    void addMeshReference( size_t entityMeshIdx );
    size_t getNumMeshReferences() const;
    size_t getMeshReferenceAt(size_t index) const;

protected:
    virtual void onUpdate(Time dt);
    virtual void onRender(RenderBackend::RenderBackendService *renderBackendSrv);

private:
    using ChildrenArray = CPPCore::TArray<Node*>;
    using MeshReferenceArray = ::CPPCore::TArray<size_t>;

    using PropertyMap = CPPCore::THashMap<ui32, Properties::Property*>;

    ChildrenArray m_children;
    Node *m_parent;
    MeshReferenceArray m_meshRefererenceArray;
    bool m_isActive;
    Common::Ids *m_ids;
    PropertyMap m_propMap;
    AABB m_aabb;
    enum DirtyFrag {
        NotDirty = 0,
        NeedsTransform = 1
    };
    ui32 m_dirty;
    RenderBackend::TransformState m_localTransformState;
    glm::mat4 m_transform;
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

} // Namespace Scene
} // namespace OSRE
