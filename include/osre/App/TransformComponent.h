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

#include <osre/Common/osre_common.h>
#include <osre/App/Component.h>
#include <osre/Common/Object.h>
#include <osre/Common/TObjPtr.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/SceneCommon.h>
#include <osre/Common/TAABB.h>

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
///	@brief  This class is used to declare user-defined node factories.
//-------------------------------------------------------------------------------------------------
struct AbstractNodeFactory {
    String m_type; ///< The type descriptor.

    /// @brief  The class constructor.
    /// @param  type    [in] The type descriptor.
    AbstractNodeFactory(const String &type) :
            m_type(type) {
        // empty
    }

    /// @brief  The default class destructor, virtual.
    virtual ~AbstractNodeFactory() = default;

    /// @brief  Will return the type.
    /// @return The type.
    virtual const String &getType() const {
        return m_type;
    }

    /// @brief Will create a new node instance.
    /// @param name                 The node name.
    /// @param ids                  The ids container.
    /// @param transformEnabled     True for transformer node.
    /// @param renderEnabled        True for rendering enabled
    /// @param parent               The parent node, nullptr for root.
    /// @return                     The new created node instance.
    virtual TransformComponent *create(const String &name, Common::Ids &ids, bool transformEnabled,
            bool renderEnabled, TransformComponent *parent) = 0;
};

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
    /// @brief The node array type-
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
    virtual void update(Time dt);
    virtual void render(RenderBackend::RenderBackendService *renderBackendSrv);
    virtual void setActive(bool isActive);
    virtual bool isActive() const;
    virtual void setProperty(Properties::Property *prop);
    virtual void getPropertyArray(::cppcore::TArray<Properties::Property *> &propArray);
    virtual Properties::Property *getProperty(const String name) const;

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
    bool onPreprocess() override;
    bool onUpdate(Time dt) override;
    bool onRender(RenderBackend::RenderBackendService *rbSrv) override;
    bool onPostprocess() override;

private:
    NodeArray m_children;
    TransformComponent *m_parent;
    MeshReferenceArray m_meshRefererenceArray;
    bool m_isActive;
    Common::Ids *m_ids;
    ::cppcore::TArray<Properties::Property *> mPropertyArray;
    PropertyMap m_propMap;
    glm::mat4 m_localTransform;
};

inline void TransformComponent::setActive(bool isActive) {
    m_isActive = isActive;
}

inline bool TransformComponent::isActive() const {
    return m_isActive;
}

} // Namespace App
} // namespace OSRE
