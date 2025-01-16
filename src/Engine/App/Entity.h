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

#include "Common/Object.h"
#include "App/AppCommon.h"
#include "App/Component.h"
#include "App/TransformComponent.h"
#include "Common/TAABB.h"

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace Common {
    class Ids;
}

namespace IO {
    class Stream;
}

namespace App {

class TransformComponent;
class AbstractBehaviour;
class Component;
class RenderComponent;
class AppBase;
class Scene;

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief	
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Entity final : public Common::Object {
    friend class Scene;

public:
    using ComponentArray = cppcore::TArray<Component*>;

    /// @brief  The class constructor.
    /// @param[in] name     The name for the scene.
    /// @param[in] ids      The id container.
    /// @param[in] scene    The owning scene.
    Entity(const String &name, Common::Ids &ids, Scene *scene);

    /// @brief The class destructor.
    ~Entity() override;
    void setNode(TransformComponent *node);
    TransformComponent *getNode() const;
    bool update( Time dt );
    bool render(RenderBackend::RenderBackendService *rbSrv);
    Component *createComponent(ComponentType type);
    Component *getComponent(ComponentType type) const;
    void setAABB( const Common::AABB &aabb );
    const Common::AABB &getAABB() const;

private:
    RenderComponent *mRenderComponent;
    ComponentArray mComponentArray;
    TransformComponent *mTransformNode;
    Common::Ids &mIds;
    Common::AABB mAabb;
    Scene *mOwner;
};

} // Namespace App
} // Namespace OSRE
