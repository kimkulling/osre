/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/UI/UiRenderer.h>
#include <osre/RenderBackend/Pipeline.h>
#include <osre/UI/Canvas.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Debugging/MeshDiagnostic.h>
#include "UIRenderUtils.h"

namespace OSRE {
namespace UI {

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;

UiRenderer::UiRenderer() 
: m_uiMaterial( nullptr ) {
    // empty
}

UiRenderer::~UiRenderer() {
    // empty
}

void UiRenderer::layout( Canvas *canvas ) {
    if (nullptr == canvas) {
        return;
    }

    canvas->layout();
}

void UiRenderer::render( Canvas *canvas, RenderBackendService * rbSrv) {
    if (nullptr == canvas) {
        return;
    }

    UiRenderCmdCache cache;
    canvas->render( cache, rbSrv );
    if (cache.m_renderCmds.isEmpty()) {
        return;
    }

    const TransformMatrixBlock &tmBlock = canvas->getTransform();

    rbSrv->beginPass(PipelinePass::getPassNameById(UiPassId));
    rbSrv->beginRenderBatch("b1");

    Debugging::MeshDiagnostic::dumpUiIndexCache(cache.m_ic);
    Debugging::MeshDiagnostic::dumpUiVertexCache(cache.m_vc);
    Texture* tex(nullptr);
    if (nullptr != cache.m_renderCmds[0]->m_texture) {
        tex = cache.m_renderCmds[0]->m_texture;
    }
    Mesh *mesh = UIRenderUtils::createGeoFromCache(cache.m_vc, cache.m_ic, tex);
    if (nullptr == mesh) {
        return;
    }
    
    rbSrv->addMesh(mesh, 0);

    rbSrv->endRenderBatch();
    rbSrv->endPass();
}

} // Namespace UI
} // Namespace OSRE
