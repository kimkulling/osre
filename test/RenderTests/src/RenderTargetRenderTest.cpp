/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "AbstractRenderTest.h"
#include "RenderTestUtils.h"

#include "Common/glm_common.h"
#include "RenderBackend/Mesh.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/TransformMatrixBlock.h"
#include "RenderBackend/MaterialBuilder.h"
#include "RenderBackend/MeshBuilder.h"

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

//-------------------------------------------------------------------------------------------------
///	@ingroup	RenderTest
///
///	@brief
//-------------------------------------------------------------------------------------------------
class RenderTargetRenderTest : public AbstractRenderTest {
public:
    RenderTargetRenderTest() : 
            AbstractRenderTest("rendertest/rendertargetrendertest") {
        // empty
    }

    ~RenderTargetRenderTest() override = default;

protected:
    bool onCreate(RenderBackendService *rbSrv) override {
        rbSrv->sendEvent(&OnAttachViewEvent, nullptr);
        
        FrameBuffer *fb = new FrameBuffer(100, 100, 32);
        
        MeshBuilder meshBuilder;
        rbSrv->beginPass("render_target");
        {
            rbSrv->setRenderTarget(fb);
            rbSrv->beginRenderBatch("b1");
            
            Mesh *triangle = meshBuilder.createTriangle(VertexType::RenderVertex, BufferAccessType::ReadOnly).getMesh();
            rbSrv->addMesh(triangle, 1);

            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();

        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("b2");
            {
                Mesh *quad = meshBuilder.allocQuads(VertexType::RenderVertex, BufferAccessType::ReadOnly).getMesh();
                rbSrv->addMesh(quad, 1);
            }
        }

        return true;
    }

    bool onDestroy( RenderBackendService * ) override {
        return true;
    }

    bool onRender( RenderBackendService * ) override {
        return true;
    }
};

ATTACH_RENDERTEST(RenderTargetRenderTest)

} // Namespace RenderTest
} // Namespace OSRE
