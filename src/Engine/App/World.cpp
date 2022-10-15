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
#include <osre/RenderBackend/MeshProcessor.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/App/Camera.h>

namespace OSRE {
namespace App {

using namespace ::CPPCore;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

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

World::World(const String &worldName) :
        Object(worldName),
        mViews(),
        mLookupViews(),
        mEntities(),
        mActiveCamera(nullptr),
        mRoot(nullptr),
        mIds(),
        mPipeline(nullptr),
        mDirtry(false) {
    // empty
}

World::~World() {
    ContainerClear<TArray<Camera *>>(mViews, lookupMapDeleterFunc);
    mLookupViews.clear();
    mActiveCamera = nullptr;
}

Camera *World::addCamera(const String &name) {
    if (name.empty()) {
        return nullptr;
    }

    mActiveCamera = new Camera(name, mIds, mRoot);
    mViews.add(mActiveCamera);
    const ui32 hash = StringUtils::hashName(mActiveCamera->getName());
    mLookupViews.insert(hash, mActiveCamera);
    mDirtry = true;

    return mActiveCamera;
}

Camera *World::getCameraAt(ui32 i) const {
    if (i >= mViews.size()) {
        return nullptr;
    }

    return mViews[i];
}

Camera *World::setActiveCamera(Camera *activeView) {
    if (mActiveCamera == activeView) {
        return nullptr;
    }

    mActiveCamera = activeView;
    if (nullptr == mViews.find(activeView)) {
        mViews.add(activeView);
    }

    return activeView;
}

Camera *World::setActiveCamera(const String &viewName) {
    const ui32 hash(StringUtils::hashName(viewName));
    if (!mLookupViews.hasKey(hash)) {
        return nullptr;
    }

    Camera *activeView(nullptr);
    if (mLookupViews.getValue(hash, activeView)) {
        setActiveCamera(activeView);
        return activeView;
    }

    return nullptr;
}

Camera *World::getActiveView() const {
    return mActiveCamera;
}

void World::addEntity(Entity *entity) {
    if (nullptr == entity) {
        osre_debug(Tag, "Pointer to entity are nullptr");
        return;
    }
    mDirtry = true;
    mEntities.add(entity);
}

bool World::removeEntity(Entity *entity) {
    if (nullptr == entity) {
        return false;
    }
    
    bool found = false;
    CPPCore::TArray<Entity*>::Iterator it = mEntities.find(entity);
    if (mEntities.end() != it) {
        mEntities.remove(it);
        found = true;
        mDirtry = true;
    }

    return found;
}

Entity *World::getEntityByName(const String &name) const {
    if (name.empty()) {
        return nullptr;
    }

    for (size_t i = 0; i < mEntities.size(); ++i) {
        if (nullptr != mEntities[i]) {
            if (mEntities[i]->getName() == name) {
                return mEntities[i];
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
    mDirtry = true;
}


void World::update(Time dt) {
    if (nullptr != mActiveCamera) {
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

void World::draw(RenderBackendService *rbSrv) {
    osre_assert(nullptr != rbSrv);


    rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
    rbSrv->beginRenderBatch("b1");

    if (nullptr != mActiveCamera) {
        mActiveCamera->draw(rbSrv);
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
