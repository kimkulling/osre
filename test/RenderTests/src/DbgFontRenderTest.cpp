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
#include "AbstractRenderTest.h"
#include "RenderTestUtils.h"

#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Scene/DbgRenderer.h>
#include <osre/Scene/MeshBuilder.h>

#include <iomanip>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

//-------------------------------------------------------------------------------------------------
/// @ingroup	RenderTest
///
/// @brief  A debug font - rendering test
//-------------------------------------------------------------------------------------------------
class DbgFontRenderTest : public AbstractRenderTest {
    TransformMatrixBlock m_transformMatrix;
    ui32 m_frameCount;

public:
    DbgFontRenderTest() :
            AbstractRenderTest("rendertest/DbgFontRenderTest"),
            m_frameCount(0) {
        // empty
    }

    ~DbgFontRenderTest() override {
        // empty
    }

    bool onCreate(RenderBackendService *rbSrv) override {
        rbSrv->sendEvent(&OnAttachViewEvent, nullptr);

        Scene::DbgRenderer::getInstance()->renderDbgText(1, 1, 1U, "XXX");

        return true;
    }

    bool onDestroy(RenderBackendService *) override {
        Scene::DbgRenderer::getInstance()->clear();
        return true;
    }

    bool onRender(RenderBackend::RenderBackendService *) override {
        m_frameCount++;
        std::stringstream stream;
        stream << std::setfill('0') << std::setw(3) << m_frameCount;
        Scene::DbgRenderer::getInstance()->renderDbgText(10, 10, 1, stream.str());
        return true;
    }
};

ATTACH_RENDERTEST(DbgFontRenderTest)

} // namespace RenderTest
} // namespace OSRE
