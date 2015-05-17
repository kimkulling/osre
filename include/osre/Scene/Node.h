#pragma once

#include <osre/Common/Object.h>

#include <cppcore/Container/TArray.h>
#include <glm/glm.hpp>

namespace OSRE {

namespace RenderBackend {
    struct TransformBlock;
}

namespace Scene {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Scene::Node
///	@ingroup	Application
///
///	@brief
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Node : public Common::Object{
public:
    enum TraverseMode {
        FlatMode,
        RecursiveMode
    };

public:
    Node( const String &name, Node *parent = nullptr );
    virtual ~Node();
    virtual void setParent( Node *parent );
    virtual Node *getParent() const;
    virtual void addChild( Node *child );
    virtual bool removeChild( const String &name, TraverseMode mode );
    virtual Node *findChild( const String &name ) const;
    virtual ui32 getNumChilds() const;
    virtual Node *getChildAt( ui32 idx ) const;
    virtual void releaseChildren();
    virtual void setTransformBlock( RenderBackend::TransformBlock *transformBlock );
    virtual void setPosition( const glm::vec3 &pos );
    virtual const glm::vec4 &getPosition() const;
    virtual void setScale( const glm::vec3 &pos );
    virtual const glm::vec4 &getScale() const;

private:
    CPPCore::TArray<Node*> m_childs;
    Node *m_parent;
    RenderBackend::TransformBlock *m_transform;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Scene
} // namespace OSRE
