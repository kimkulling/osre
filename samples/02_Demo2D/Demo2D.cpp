/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2022 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/App/AppBase.h>
#include <osre/App/AssetRegistry.h>
#include <osre/Properties/Settings.h>
#include <osre/Common/Logger.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/App/ServiceProvider.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/TransformMatrixBlock.h>

#include "RenderBackend/2D/CanvasRenderer.h"

using namespace ::OSRE;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::App;

// To identify local log entries
static const c8 Tag[] = "ModelLoadingApp";

// The example application, will create the render environment and render a simple triangle onto it
class Demo2DApp : public App::AppBase {
    TransformMatrixBlock mTransformMatrix;
    CanvasRenderer *mCanvasRenderer;

public:
    Demo2DApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv),
            mTransformMatrix(),
            mCanvasRenderer(nullptr) {
        // empty
    }

    ~Demo2DApp() override = default;

    void quitCallback(ui32, void *) {
        AppBase::requestShutdown();
    }

protected:
    bool onCreate() override {
        Properties::Settings *baseSettings(AppBase::getSettings());
        if (baseSettings == nullptr) {
            return false;
        }

        baseSettings->setString(Properties::Settings::WindowsTitle, "Demo in 2D!");
        if (!AppBase::onCreate()) {
            return false;
        }

        RenderBackendService *rbSrv = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
        mCanvasRenderer = new CanvasRenderer(rbSrv, 2);

        Color4 green(0, 1, 0, 1);
        mCanvasRenderer->setcolor(green);
        mCanvasRenderer->drawRect(1, 1, 100, 100, true);

        return true;
    }

    void onRender() {
        RenderBackendService *rbSrv = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        rbSrv->beginRenderBatch("2d");

        mCanvasRenderer->render();
        rbSrv->setMatrix(MatrixType::Model, mTransformMatrix.m_model);
        mCanvasRenderer->render();        

        rbSrv->endRenderBatch();
        rbSrv->endPass();

        AppBase::onUpdate(); 
    }
};

OSRE_MAIN(Demo2DApp)
