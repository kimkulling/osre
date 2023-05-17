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
#include <osre/Modules/ModuleBase.h>
#include <osre/Modules/IModuleView.h>

#include <osre/App/AppBase.h>

namespace OSRE {
namespace Modules {

using namespace ::OSRE::Common;
using namespace ::OSRE::App;

ModuleBase::ModuleBase(const String &name, AppBase *parentApp) :
        Object(name),
        mState(ModuleState::Init),
        mView(nullptr),
        mParentApp(parentApp) {
    // empty
}

void ModuleBase::setModulelView(IModuleView *view) {
    mView = view;
}

IModuleView *ModuleBase::getModuleView() const {
    return mView;
}

bool ModuleBase::load() {
    if (mState != ModuleState::Init) {
        return false;
    }

    if (onLoad()) {
        mState = ModuleState::Loaded;
    } else {
        mState = ModuleState::Error;
    }

    return mState == ModuleState::Loaded;
}

bool ModuleBase::unload() {
    if (mState != ModuleState::Loaded) {
        return false;
    }
    if (onUnload()) {
        mState = ModuleState::Unloaded;
    } else {
        mState = ModuleState::Error;
    }

    return mState == ModuleState::Unloaded;
}

void ModuleBase::update() {
    // Update module itself
    onUpdate();
    
    // Update view
    if (nullptr != mView) {
        mView->update();
    }
}

void ModuleBase::render() {
    onRender();
}

bool ModuleBase::onEvent(const Event&, const EventData*){
    return true;
}


App::AppBase *ModuleBase::getParentApp() const {
    return mParentApp;
}

bool ModuleBase::onLoad() {
    return true;
}

bool ModuleBase::onUnload() {
    return true;
}

void ModuleBase::onUpdate() {
    // empty
}

void ModuleBase::onRender() {
    // empty
}

bool ModuleBase::onAttached( const Common::EventData * ) {
    return true;
}

bool ModuleBase::onDetached( const Common::EventData * ) {
    return true;
}

} // namespace Modules
} // namespace OSRE 
