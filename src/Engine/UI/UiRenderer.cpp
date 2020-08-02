/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "UIRenderUtils.h"
#include <osre/Debugging/MeshDiagnostic.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/Pipeline.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/MeshBuilder.h>
#include <osre/UI/Canvas.h>
#include <osre/UI/UiRenderer.h>

#include "FontRenderer.h"

namespace OSRE {
namespace UI {

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;

UiRenderer::UiRenderer() :
        m_uiMaterial(nullptr),
        mFontRenderer(nullptr) {
    mFontRenderer = new FontRenderer;
}

UiRenderer::~UiRenderer() {
    delete mFontRenderer;
}

void UiRenderer::layout(Canvas *canvas) {
    if (nullptr == canvas) {
        return;
    }

    canvas->layout();
}

void UiRenderer::render(Canvas *canvas, RenderBackendService *rbSrv) {
    if (nullptr == canvas) {
        return;
    }

    UiRenderCmdCache cache;
    canvas->render(cache, rbSrv);
    if (cache.m_renderCmds.isEmpty()) {
        return;
    }

    rbSrv->beginPass(PipelinePass::getPassNameById(UiPassId));
    rbSrv->beginRenderBatch("b1");

    Debugging::MeshDiagnostic::dumpUiIndexCache(cache.m_ic);
    Debugging::MeshDiagnostic::dumpUiVertexCache(cache.m_vc);
    Material *mat = nullptr;

    if (nullptr != cache.m_renderCmds[0]->mMaterial) {
        mat = cache.m_renderCmds[0]->mMaterial;
    }

    Mesh *mesh = UIRenderUtils::createGeoFromCache(cache.m_vc, cache.m_ic, mat);
    if (nullptr == mesh) {
        return;
    }

    rbSrv->addMesh(mesh, 0);
    for (size_t i = 0; i < cache.mTextCmdArray.size(); ++i) {
        renderFont(cache.mTextCmdArray[i], static_cast<ui32>(i), rbSrv);
    }

    rbSrv->endRenderBatch();
    rbSrv->endPass();
}

void UiRenderer::renderFont(TextEntry *entry, ui32 id, RenderBackendService *rbSrv) {
    if (nullptr == entry) {
        return;
    }

    mFontRenderer->AddRenderText(entry->x, entry->y, id, entry->text, rbSrv);
}

} // Namespace UI
} // Namespace OSRE
