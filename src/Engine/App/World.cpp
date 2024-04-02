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
#include <osre/App/Entity.h>
#include <osre/App/World.h>
#include <osre/Common/Logger.h>
#include <osre/Common/StringUtils.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/RenderBackend/MeshProcessor.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/App/CameraComponent.h>

namespace OSRE {
namespace App {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

static constexpr c8 Tag[] = "World";

World::World(const String &worldName) :
        Object(worldName),
        mEntities(),
        mActiveCamera(nullptr),
        mRoot(nullptr),
        mIds(),
        mPipeline(nullptr),
        mDirtry(false) {
    // empty
}

World::~World() {
    // empty
}

void World::addEntity(Entity *entity) {
    if (nullptr == entity) {
        osre_debug(Tag, "Pointer to entity are nullptr");
        return;
    }
    mDirtry = true;
    mEntities.add(entity);
}

Entity *World::findEntity(const String &name) {
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

bool World::removeEntity(Entity *entity) {
    if (nullptr == entity) {
        return false;
    }
    
    bool found = false;
    cppcore::TArray<Entity*>::Iterator it = mEntities.find(entity);
    if (mEntities.end() != it) {
        mEntities.remove(it);
        found = true;
        mDirtry = true;
    }

    return found;
}

bool World::setActiveCamera(CameraComponent *camera) {
    if (camera == nullptr) {
        return false;
    }

    mActiveCamera = camera;

    return true;
}

Entity *World::getEntityByName(const String &name) const {
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

void World::setSceneRoot(TransformComponent *root ) {
    mRoot = root;
    mDirtry = true;
}


void World::init() {
    Time dt;
    update(dt);
}

void World::update(Time dt) {
    if (mActiveCamera != nullptr) {
        mActiveCamera->update(dt);
    }

    if (mDirtry) {
        updateBoundingTrees();
    }

    for (Entity *entity : mEntities) {
        if (nullptr == entity) {
            entity->update(dt);
        }
    }
}

void World::render(RenderBackendService *rbSrv) {
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

void World::updateBoundingTrees() {
    for (ui32 i = 0; i < mEntities.size(); ++i) {
        auto *entity = mEntities[i];
        if (entity == nullptr) {
            continue;
        }
        MeshProcessor processor;
        RenderComponent *rc = (RenderComponent *)entity->getComponent(ComponentType::RenderComponentType);
        for (ui32 j = 0; j < rc->getNumGeometry(); ++j) {
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
