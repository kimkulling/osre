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
#include "Modules/ModuleRegistry.h"
#include "Modules/ModuleBase.h"

#include <osre/Debugging/osre_debugging.h>

namespace OSRE {
namespace Editor {

ModuleRegistry::ModuleRegistry() :
        mModules(),
        mPaths() {
    // empty
}

ModuleRegistry::~ModuleRegistry() {
    // empty
}

void ModuleRegistry::addPath(const String &path) {
    if (path.empty()) {
        return;
    }

    mPaths.add(path);
}

bool ModuleRegistry::registerModule(ModuleBase *mod) {
    if (nullptr == mod) {
        return false;
    }

    if (nullptr == findModule(mod->getName())) {
        mModules.add(mod);
    }

    return true;
}

ModuleBase *ModuleRegistry::findModule(const String &name) const {
    if (name.empty()) {
        return nullptr;
    }

    for (ui32 i = 0; i < mModules.size(); ++i) {
        if (name == mModules[i]->getName()) {
            return mModules[i];
        }
    }

    return nullptr;
}

bool ModuleRegistry::unregisterModule(ModuleBase *mod) {
    if (nullptr == mod) {
        return false;
    }

    ModuleArray::Iterator it = mModules.find(mod);
    if (it == mModules.end()) {
        return false;
    }
    mModules.remove(it);

    return true;
}

void ModuleRegistry::update() {
    for (ModuleBase *module : mModules) {
        if (nullptr == module) {
            osre_assert(nullptr != module);
            continue;
        }

        module->update();
    }
}

void ModuleRegistry::render() {
    for (ModuleBase *module : mModules) {
        if (nullptr == module) {
            osre_assert(nullptr != module);
            continue;
        }

        module->render();
    }
}

} // Namespace Editor
} // Namespace OSRE
