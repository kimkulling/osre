/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/UI/Screen.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/RenderBackendService.h>

#include "UIRenderUtils.h"

namespace OSRE {
namespace UI {

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;

UiRenderer::UiRenderer() {
    // empty
}

UiRenderer::~UiRenderer() {
    // empty
}

void UiRenderer::render( Stage* stage, Screen *screen, RenderBackendService * rbSrv) {
    if (nullptr == screen) {
        return;
    }

    UiRenderCmdCache cache;
    screen->render( cache, rbSrv );
    if (cache.isEmpty()) {
        return;
    }

    const TransformMatrixBlock &tmBlock = screen->getTransform();

    rbSrv->beginPass(PipelinePass::getPassNameById(UiPassId));
    rbSrv->beginRenderBatch("b1");
    rbSrv->setMatrix(MatrixType::Model, tmBlock.m_model);
    rbSrv->setMatrix(MatrixType::Projection, tmBlock.m_projection);

    UiVertexCache vc; 
    UiIndexCache ic;
    RenderBackend::Material *mat(nullptr);
    for ( ui32 i = 0; i < cache.size(); ++i ) {
        UiRenderCmd *currentCmd( cache[ i ] );
        if (currentCmd == nullptr) {
            continue;
        }
        
        mat = currentCmd->m_mat;
        const UiVertexCache &currentVC = currentCmd->m_vc;
        const UiIndexCache  &currentIC = currentCmd->m_ic;
        if (currentVC.numVertices() > 0) {
            // Copy all vertices
            vc.add( &currentVC.m_cache[0], currentVC.numVertices());
            
            // Copy all indices
            ic.add( &currentIC.m_cache[0], currentIC.numIndices());
        }
    }

    Mesh *mesh = UIRenderUtils::createGeoFromCache( vc, ic, mat);
    if (nullptr == mesh) {
        return;
    }
    
    rbSrv->addMesh(mesh, 0);

    rbSrv->endRenderBatch();
    rbSrv->endPass();
}

} // Namespace UI
} // Namespace OSRE
