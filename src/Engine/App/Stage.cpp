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
#include "App/Stage.h"
#include "App/World.h"
#include "Common/Logger.h"

namespace OSRE {
namespace App {

static constexpr c8 Tag[] = "Stage";

Stage::Stage(const String &stageName) :
        Object(stageName),
        mWorlds(),
        mRenderWorlds() {
    // empty
}

Stage::~Stage() {
    for (ui32 i = 0; i < mWorlds.size(); ++i) {
        mWorlds[i]->release();
    }
    mWorlds.clear();
    mRenderWorlds.clear();
}

World *Stage::createWorld(const String &name) {
    if (name.empty()) {
        osre_debug(Tag, "Invalid name for a new world.");
        return nullptr;
    }

    World *world = new World(name);
    mWorlds.add(world);

    return world;
}

World *Stage::findWorld(const String &name) const {
    if (mWorlds.isEmpty()) {
        return nullptr;
    }

    for (size_t i = 0; i < mWorlds.size(); ++i) {
        if (mWorlds[i]->getName() == name) {
            return mWorlds[i];
        }
    }

    return nullptr;
}

size_t Stage::getNumberOfWorlds() const {
    return mWorlds.size();
}

World *Stage::getWorldAt( ui32 index ) const {
    if (index >= mWorlds.size()) {
        return nullptr;
    }

    return mWorlds[index];
}

World *Stage::addActiveWorld(const String &name) {
    World *world = new World(name);
    mRenderWorlds.add(world);

    return world;
}

const Stage::WorldArray &Stage::getActiveWorlds() const {
    return mRenderWorlds;
}

World *Stage::getActiveWorld(size_t index) const {
    if (index >= mRenderWorlds.size()) {
        return nullptr;
    }

    return mRenderWorlds[index];
}

void Stage::update(Time dt) {
    for (size_t i=0; i<mRenderWorlds.size(); ++i){
        World *w = mRenderWorlds[i];
        if (w != nullptr) {
            w->update(dt);
        }
    }
}

void Stage::render(RenderBackend::RenderBackendService *rbService) {
    if (nullptr == rbService) {
        return;
    }

    for (size_t i=0; i<mRenderWorlds.size(); ++i){
        World *w = mRenderWorlds[i];
        if (w != nullptr) {
            w->render(rbService);
        }
    }
}

} // namespace App
} // namespace OSRE
