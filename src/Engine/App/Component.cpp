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
#include "App/Component.h"
#include "App/Entity.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/RenderCommon.h"

namespace OSRE {
namespace App {

using namespace ::OSRE::RenderBackend;
using namespace ::cppcore;

static const glm::vec3 Dummy = glm::vec3(-1, -1, -1);

Component::Component(Entity *owner, ComponentType type) :
        mOwner(owner),
        mType(type) {
    osre_assert(nullptr != owner);
}

void Component::update(Time dt) {
    onUpdate(dt);
}

void Component::render(RenderBackendService *renderBackendSrv) {
    onRender(renderBackendSrv);
}

RenderComponent::RenderComponent(Entity *owner) :
        Component(owner, ComponentType::RenderComponentType), m_newGeo() {
    // empty
}

void RenderComponent::addStaticMesh(Mesh *geo) {
    if (nullptr == geo) {
        return;
    }

    m_newGeo.add(geo);
}

void RenderComponent::addStaticMeshArray(const RenderBackend::MeshArray &array) {
    if (array.isEmpty()) {
        return;
    }

    for (size_t i = 0; i < array.size(); ++i) {
        m_newGeo.add(array[i]);
    }
}

size_t RenderComponent::getNumMeshes() const {
    return m_newGeo.size();
}

Mesh *RenderComponent::getMeshAt(size_t idx) const {
    return m_newGeo[idx];
}

void RenderComponent::getMeshArray(RenderBackend::MeshArray &meshArray) {
    meshArray = m_newGeo;
}

bool RenderComponent::onUpdate(Time) {
    return true;
}

bool RenderComponent::onRender(RenderBackendService *renderBackendSrv) {
    if (!m_newGeo.isEmpty()) {
        for (ui32 i = 0; i < m_newGeo.size(); i++) {
            renderBackendSrv->addMesh(m_newGeo[i], 0);
        }
        m_newGeo.resize(0);
    }

    return true;
}

LightComponent::LightComponent(Entity *owner) : Component(owner, ComponentType::LightComponentType), mLight(nullptr) {
    // empty
}

void LightComponent::setLight(RenderBackend::Light *light) {
    mLight = light;
}

bool LightComponent::onUpdate(Time) {
    if (mLight == nullptr) {
        return true;
    }

    return true;
}

bool LightComponent::onRender(RenderBackend::RenderBackendService*) {
    return true;
}

} // namespace App
} // namespace OSRE
