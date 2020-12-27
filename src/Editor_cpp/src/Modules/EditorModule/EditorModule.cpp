#include "Modules/EditorModule/EditorModule.h"

#include <osre/App/AppBase.h>
#include <osre/RenderBackend/RenderBackendService.h>

namespace OSRE {
namespace Editor {

using namespace OSRE::App;
using namespace ::OSRE::RenderBackend;

EditorModule::EditorModule(App::AppBase *parentApp) :
        ModuleBase("editor.module", parentApp) {

}

EditorModule::~EditorModule() {

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
    AppBase *parentApp = getParentApp();
    RenderBackend::RenderBackendService *rbService = parentApp->getRenderBackendService();

    rbService->beginPass(PipelinePass::getPassNameById(RenderPassId));
    rbService->beginRenderBatch("b1");

    rbService->setMatrix(MatrixType::Model, m_transformMatrix.m_model);

    rbService->endRenderBatch();
    rbService->endPass();
}

}
} // namespace OSRE
