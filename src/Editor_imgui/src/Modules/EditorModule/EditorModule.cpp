/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Modules/EditorModule/EditorModule.h"

#include "App/AppBase.h"
#include "App/ServiceProvider.h"
#include "RenderBackend/RenderBackendService.h"

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
