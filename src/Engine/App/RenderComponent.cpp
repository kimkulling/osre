#include "RenderComponent.h"
#include <osre/RenderBackend/RenderBackendService.h>

namespace OSRE {
namespace App {

using namespace ::OSRE::RenderBackend;

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

size_t RenderComponent::getNumGeometry() const {
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

} // namespace App
} // namespace OSRE
