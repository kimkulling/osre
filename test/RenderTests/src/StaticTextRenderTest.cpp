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

#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/DbgRenderer.h"

#include <iomanip>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

//-------------------------------------------------------------------------------------------------
///	@ingroup	RenderTest
///
///	@brief  A static text - rendering test
//-------------------------------------------------------------------------------------------------
class StaticTextRenderTest : public AbstractRenderTest {
    TransformMatrixBlock m_transformMatrix;
    ui32 m_frameCount;

public:
    StaticTextRenderTest() : AbstractRenderTest( "rendertest/StaticTextRenderTest" )
        , m_frameCount( 0 ) {
        // empty
    }

    virtual ~StaticTextRenderTest() = default;

    bool onCreate( RenderBackendService *rbSrv ) override {
        rbSrv->sendEvent( &OnAttachViewEvent, nullptr );

        DbgRenderer::getInstance()->renderDbgText( 2U, 0, 0, "This is a test-text!\nAnd another one!" );

        return true;
    }

    bool onDestroy( RenderBackendService * ) override {
        DbgRenderer::getInstance()->clear();
        return true;
    }
};

ATTACH_RENDERTEST( StaticTextRenderTest )

} // Namespace RenderTest
} // Namespace OSRE
