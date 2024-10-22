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

#include "Common/osre_common.h"
#include "App/Component.h"
#include "Common/Object.h"
#include "Common/TObjPtr.h"
#include "RenderBackend/RenderCommon.h"
#include "App/SceneCommon.h"
#include "Common/TAABB.h"

#include <cppcore/Container/TArray.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace Common {
    class Ids;
}

namespace Properties {
    class Property;
}

namespace RenderBackend {
    struct TransformState;
}

namespace App {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class is used to represents a simple node in the stage hierarchy. You can add
/// several functionalities by adding components to is. Each component implements functionality
/// like render geometry or transformation information.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT TransformComponent : public Common::Object, public Component {
public:
    /// @brief The node pointer type.
    using NodePtr = ::OSRE::Common::TObjPtr<::OSRE::App::TransformComponent>;
    /// @brief The node array type.
    using NodeArray = cppcore::TArray<TransformComponent *>;
    /// @brief Used to declare mesh-array instances.
    using MeshReferenceArray = ::cppcore::TArray<size_t>;
    /// @brief Used to declare properties.
    using PropertyMap = cppcore::THashMap<ui32, Properties::Property *>;

    ///	@brief  Describes the traversal mode.
    enum class TraverseMode {
        FlatMode,           ///< Flat, ignore children.
        RecursiveMode       ///< Recursive, iterate through children.
    };

public:
    TransformComponent(const String &name, Entity *owner, Common::Ids &ids, TransformComponent *parent = nullptr);
    virtual ~TransformComponent();
    virtual void setParent(TransformComponent *parent);
    virtual TransformComponent *getParent() const;
    virtual TransformComponent *createChild(const String &name);
    virtual void addChild(TransformComponent *child);
    virtual bool removeChild(const String &name, TraverseMode mode);
    virtual TransformComponent *findChild(const String &name) const;
    virtual size_t getNumChildren() const;
    virtual TransformComponent *getChildAt(size_t idx) const;
    virtual void releaseChildren();
    virtual void render(RenderBackend::RenderBackendService *renderBackendSrv);
    virtual void setActive(bool isActive);
    virtual bool isActive() const;

    void translate(const glm::vec3 &pos);
    void scale(const glm::vec3 &pos);
    void rotate(f32 angle, const glm::vec3 &axis);
    void setRotation(glm::quat &rotation);
    void setTransformationMatrix(const glm::mat4 &m);
    const glm::mat4 &getTransformationMatrix() const;
    glm::mat4 getWorlTransformMatrix();

    void addMeshReference(size_t entityMeshIdx);
    size_t getNumMeshReferences() const;
    size_t getMeshReferenceAt(size_t index) const;

protected:
    bool onUpdate(Time dt) override;
    bool onRender(RenderBackend::RenderBackendService *rbSrv) override;

private:
    NodeArray mChildren;
    TransformComponent *mParent;
    MeshReferenceArray mMeshRefererenceArray;
    bool mIsActive;
    Common::Ids *mIds;
    glm::mat4 mLocalTransform;
    glm::mat4 mWorldTransform;
};

inline void TransformComponent::setActive(bool isActive) {
    mIsActive = isActive;
}

inline bool TransformComponent::isActive() const {
    return mIsActive;
}

} // Namespace App
} // namespace OSRE
