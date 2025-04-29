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
#include "App/App.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/TransformMatrixBlock.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/RenderPass.h"
#include "RenderBackend/2D/CanvasRenderer.h"
#include "Common/osre_common.h"
#include "Common/Logger.h"
#include "Properties/Settings.h"
#include "UI/WidgetBase.h"
#include "UI/Button.h"
#include "UI/Panel.h"

using namespace OSRE;
using namespace OSRE::App;
using namespace OSRE::RenderBackend;
using namespace OSRE::Common;

static Ui::WidgetBase *createUi() {
    Rect2i r1(10, 10, 200, 100);
    Ui::Panel *panel = new Ui::Panel(r1, nullptr);
    Rect2i r2(20, 20, 160, 40);
    panel->addWidget(new Ui::Button("Quit", r2, panel));
    return panel;
}

// To identify local log entries we will define this tag.
DECL_OSRE_LOG_MODULE(Widgets)

class WidgetsApp final : public AppBase {
public:
    /// The shader interface for all kind of transformations.
    TransformMatrixBlock  mTransformMatrix;
    /// The 2D renderer for the ui overlay
    CanvasRenderer *mCanvasRenderer = nullptr;
    ///
    Ui::WidgetBase *mRoot = nullptr;

    /// @brief
    WidgetsApp(int argc, char *argv[]) : AppBase(argc, const_cast<const char **>(argv)) {
    }

    /// The class destructor.
    ~WidgetsApp() override = default;

protected:
    /// @brief The onCreate callback.
    bool onCreate() override {
        Properties::Settings *baseSettings  = AppBase::getSettings();
        if (baseSettings == nullptr) {
            return false;
        }

        baseSettings->setString(Properties::Settings::WindowsTitle, "Demo in 2D!");
        if (!AppBase::onCreate()) {
            return false;
        }

        mCanvasRenderer = AppBase::getCanvasRenderer();
        mRoot = createUi();
        
        return true;
    }

    void onUpdate() override {
        auto *rbService = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
        mRoot->render(mCanvasRenderer);

        rbService->beginPass(RenderPass::getPassNameById(UiPassId));
        rbService->beginRenderBatch("2d.b1");
        rbService->setMatrix(MatrixType::Model, mTransformMatrix.mModel);
        mCanvasRenderer->render(rbService);
        rbService->endRenderBatch();
        rbService->endPass();

        AppBase::onUpdate();
    }
};

int main( int argc, char *argv[] )  {
    WidgetsApp myApp( argc, argv );
    if ( !myApp.create() ) {
        return 1;
    }
    while (myApp.handleEvents()) {
        myApp.update();
        myApp.requestNextFrame();
    }
    myApp.destroy();

    return 0;
}
