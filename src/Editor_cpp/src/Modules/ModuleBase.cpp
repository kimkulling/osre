#include "ModuleBase.h"
#include "IModuleView.h"

#include <osre/App/AppBase.h>

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

bool ModuleBase::load(OsreEdApp *parent) {
    if (mState != Init) {
        return false;
    }

    if (onLoad(parent)) {
        mState = Loaded;
    } else {
        mState = Error;
    }

    return mState == Loaded;
}

bool ModuleBase::unload(OsreEdApp *parent) {
    if (mState != Loaded) {
        return false;
    }
    if (onUnload(parent)) {
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

bool ModuleBase::onLoad(OsreEdApp *parent) {
    return true;
}

bool ModuleBase::onUnload(OsreEdApp *parent) {
    return true;
}

void ModuleBase::onUpdate() {
    // empty
}

} // namespace Editor
} // namespace OSRE 
