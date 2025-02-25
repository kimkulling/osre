/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "App/Entity.h"
#include "App/Scene.h"
#include "Common/Logger.h"
#include "Common/StringUtils.h"
#include "Debugging/osre_debugging.h"
#include "RenderBackend/MeshProcessor.h"
#include "RenderBackend/RenderBackendService.h"
#include "App/CameraComponent.h"

namespace OSRE {
namespace App {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

static constexpr c8 Tag[] = "Scene";

Scene::Scene(const String &worldName) :
        Object(worldName),
        mActiveCamera(nullptr),
        mRoot(nullptr),
        mPipeline(nullptr),
        mDirtry(false) {
    // empty
}

void Scene::addEntity(Entity *entity) {
    if (nullptr == entity) {
        osre_debug(Tag, "Pointer to entity are nullptr");
        return;
    }
    mDirtry = true;
    mEntities.add(entity);
}

Entity *Scene::findEntity(const String &name) {
    if (name.empty()) {
        return nullptr;
    }

    Entity *currentEntity{ nullptr };
    for (size_t i=0; i<mEntities.size(); ++i ) {
        currentEntity = mEntities[i];
        if (currentEntity != nullptr) {
            if (currentEntity->getName() == name) {
                return currentEntity;
            }
        }
    }

    return nullptr;
}

bool Scene::removeEntity(Entity *entity) {
    if (nullptr == entity) {
        return false;
    }
    
    bool found = false;
    TArray<Entity*>::Iterator it = mEntities.linearSearch(entity);
    if (mEntities.end() != it) {
        mEntities.remove(it);
        found = true;
        mDirtry = true;
    }

    return found;
}

size_t Scene::getNumEntities() const {
    return mEntities.size();
}

Entity *Scene::getEntityAt(size_t index) const {
    if (index >= mEntities.size()) {
        return nullptr;
    }

    return mEntities[index];
}

bool Scene::setActiveCamera(CameraComponent *camera) {
    if (camera == nullptr) {
        return false;
    }

    mActiveCamera = camera;

    return true;
}

Entity *Scene::getEntityByName(const String &name) const {
    if (name.empty()) {
        return nullptr;
    }

    Entity *entity = nullptr;
    for (size_t i = 0; i < mEntities.size(); ++i) {
        if (nullptr != mEntities[i]) {
            if (mEntities[i]->getName() == name) {
                entity = mEntities[i];
                break;
            }
        }
    }

    return entity;
}

void Scene::setSceneRoot(TransformComponent *root ) {
    mRoot = root;
    mDirtry = true;
}

void Scene::init() {
    Time dt;
    update(dt);
}

void Scene::update(Time dt) {
    if (mActiveCamera != nullptr) {
        mActiveCamera->update(dt);
    }

    if (mDirtry) {
        updateBoundingTrees();
    }

    for (Entity *entity : mEntities) {
        if (nullptr != entity) {
            entity->update(dt);
        }
    }
}

void Scene::render(RenderBackendService *rbSrv) {
    osre_assert(nullptr != rbSrv);

    rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
    rbSrv->beginRenderBatch("b1");

    if (mActiveCamera != nullptr) {
        mActiveCamera->render(rbSrv);
    }

    for (Entity *entity : mEntities) {
        if (nullptr != entity) {
            entity->render(rbSrv);
        }
    }

    rbSrv->endRenderBatch();
    rbSrv->endPass();
}

void Scene::updateBoundingTrees() {
    for (ui32 i = 0; i < mEntities.size(); ++i) {
        auto *entity = mEntities[i];
        if (entity == nullptr) {
            continue;
        }
        MeshProcessor processor;
        RenderComponent *rc = (RenderComponent *)entity->getComponent(ComponentType::RenderComponentType);
        for (ui32 j = 0; j < rc->getNumMeshes(); ++j) {
            processor.addMesh(rc->getMeshAt(j));
        }
        if (processor.execute()) {
            entity->setAABB(processor.getAABB());
        }
    }
    mDirtry = false;
}

} // Namespace App
} // namespace OSRE
