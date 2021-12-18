/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/App/Entity.h>
#include <osre/App/World.h>
#include <osre/Common/Logger.h>
#include <osre/Common/StringUtils.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Scene/Camera.h>

namespace OSRE {
namespace App {

using namespace ::CPPCore;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;

static const c8 *Tag = "World";

template <class T>
void lookupMapDeleterFunc(TArray<T> &ctr) {
    for (ui32 i = 0; i < ctr.size(); ++i) {
        if (nullptr != ctr[i]) {
            ctr[i]->release();
            ctr[i] = nullptr;
        }
    }
    ctr.clear();
}

World::World(const String &worldName, RenderMode renderMode) :
        Object(worldName),
        m_views(),
        m_lookupViews(),
        m_entities(),
        m_activeCamera(nullptr),
        mRoot(nullptr),
        m_ids(),
        mPipeline(nullptr),
        m_renderMode(renderMode) {
    // empty
}

World::~World() {
    ContainerClear<TArray<Camera *>>(m_views, lookupMapDeleterFunc);
    m_lookupViews.clear();
    m_activeCamera = nullptr;
}

Scene::Camera *World::addCamera(const String &name) {
    m_activeCamera = new Scene::Camera(name, m_ids, mRoot);
    m_views.add(m_activeCamera);
    const ui32 hash = StringUtils::hashName(m_activeCamera->getName());
    m_lookupViews.insert(hash, m_activeCamera);
    
    return m_activeCamera;
}

Camera *World::getCameraAt(ui32 i) const {
    if (i >= m_views.size()) {
        return nullptr;
    }

    return m_views[i];
}

Camera *World::setActiveCamera(Camera *activeView) {
    if (m_activeCamera == activeView) {
        return nullptr;
    }

    m_activeCamera = activeView;
    if (nullptr == m_views.find(activeView)) {
        m_views.add(activeView);
    }

    return activeView;
}

Camera *World::setActiveCamera(const String &viewName) {
    const ui32 hash(StringUtils::hashName(viewName));
    if (!m_lookupViews.hasKey(hash)) {
        return nullptr;
    }

    Camera *activeView(nullptr);
    if (m_lookupViews.getValue(hash, activeView)) {
        setActiveCamera(activeView);
        return activeView;
    }

    return nullptr;
}

Camera *World::getActiveView() const {
    return m_activeCamera;
}

void World::addEntity(Entity *entity) {
    if (nullptr == entity) {
        osre_debug(Tag, "Pointer to entity are nullptr");
        return;
    }

    m_entities.add(entity);
}

bool World::removeEntity(Entity *entity) {
    if (nullptr == entity) {
        return false;
    }
    
    bool found = false;
    CPPCore::TArray<Entity*>::Iterator it = m_entities.find(entity);
    if (m_entities.end() != it) {
        m_entities.remove(it);
        found = true;
    }

    return found;
}

Entity *World::getEntityByName(const String &name) const {
    if (name.empty()) {
        return nullptr;
    }

    for (size_t i = 0; i < m_entities.size(); ++i) {
        if (nullptr != m_entities[i]) {
            if (m_entities[i]->getName() == name) {
                return m_entities[i];
            }
        }
    }

    return nullptr;
}

void World::setSceneRoot(Node *root ) {
    if (nullptr == root) {
        mRoot = nullptr;
        return;
    }

    mRoot = root;
}


void World::update(Time dt) {
    if (nullptr != m_activeCamera) {
        m_activeCamera->update(dt);
    }

    for (Entity *entity : m_entities) {
        if (nullptr == entity) {
            entity->update(dt);
        }
    }
}

void World::draw(RenderBackendService *rbSrv) {
    osre_assert(nullptr != rbSrv);


    rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
    rbSrv->beginRenderBatch("b1");

    if (nullptr != m_activeCamera) {
        m_activeCamera->draw(rbSrv);
    }

    for (Entity *entity : m_entities) {
        if (nullptr != entity) {
            entity->render(rbSrv);
        }
    }

    rbSrv->endRenderBatch();
    rbSrv->endPass();
}


} // Namespace App
} // namespace OSRE
