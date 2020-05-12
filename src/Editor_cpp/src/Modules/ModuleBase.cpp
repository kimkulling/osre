#include "ModuleBase.h"
#include "IModuleView.h"

namespace OSRE {
namespace Editor {

ModuleBase::ModuleBase(const String &name) :
        Object(name),
        mState(Init),
        mView(nullptr) {
    // empty
}

ModuleBase::~ModuleBase() {
    // empty
}

void ModuleBase::setModulelView( IModuleView *view ) {
    mView = view;
}

IModuleView *ModuleBase::getModuleView() const {
    return mView;
}

bool ModuleBase::load() {
    if (mState != Init) {
        return false;
    }

    if (onLoad()) {
        mState = Loaded;
    } else {
        mState = Error;
    }

    return mState == Loaded;
}

bool ModuleBase::unload() {
    if (mState != Loaded) {
        return false;
    }
    if (onUnload()) {
        mState = Unloaded;
    } else {
        mState = Error;
    }

    return mState == Unloaded;
}

void ModuleBase::update() {
    // Update module itself
    onUpdate();
    
    // Update view
    if (nullptr != mView) {
        mView->update();
    }
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

} // namespace Editor
} // namespace OSRE 
