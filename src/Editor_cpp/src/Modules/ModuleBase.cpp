#include "ModuleBase.h"
#include "IModuleView.h"

#include <osre/App/AppBase.h>

namespace OSRE {
namespace Editor {

ModuleBase::ModuleBase(const String &name, App::AppBase *parentApp) :
        Object(name),
        mState(ModuleState::Init),
        mView(nullptr),
        mParentApp(parentApp) {
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

} // namespace Editor
} // namespace OSRE 
