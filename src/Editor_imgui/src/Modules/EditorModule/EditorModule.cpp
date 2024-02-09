#include "Modules/EditorModule/EditorModule.h"

#include <osre/App/AppBase.h>
#include <osre/App/ServiceProvider.h>
#include <osre/RenderBackend/RenderBackendService.h>

namespace OSRE {
namespace Editor {

using namespace ::OSRE::App;
using namespace ::OSRE::Modules;
using namespace ::OSRE::RenderBackend;

EditorModule::EditorModule(App::AppBase *parentApp) :
        ModuleBase("editor.module", parentApp) {
    // empty
}

bool EditorModule::onLoad() {
    AppBase *parentApp = getParentApp();
    if (nullptr == parentApp) {
        return false;
    }

    return true;
}

bool EditorModule::onUnload() {
    return true;
}

void EditorModule::onUpdate() {
}

void EditorModule::onRender() {
    RenderBackendService *rbService = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);

    rbService->beginPass(RenderPass::getPassNameById(RenderPassId));
    rbService->beginRenderBatch("b1");

    rbService->setMatrix(MatrixType::Model, mTransformMatrix.mModel);

    rbService->endRenderBatch();
    rbService->endPass();
}

}
} // namespace OSRE
