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
#include <gtest/gtest.h>
#include "RenderBackend/2D/CanvasRenderer.h"

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::RenderBackend;

class CanvasRendererTest : public ::testing::Test {};

TEST_F(CanvasRendererTest, create_destroy_test) {
    bool ok = true;
    CanvasRenderer *canvasRenderer = nullptr;
    try {
        canvasRenderer = new CanvasRenderer(2, 0, 0, 1024, 768);
    } catch (...) {
        ok = false;
        canvasRenderer = nullptr;
    }
    delete canvasRenderer;
    ASSERT_TRUE(ok);
}

TEST_F(CanvasRendererTest, change_resolution_test) {
    CanvasRenderer canvasRenderer(2, 0, 0, 1024, 768);
    ASSERT_TRUE(canvasRenderer.isDirty());
    canvasRenderer.setClean();
    ASSERT_FALSE(canvasRenderer.isDirty());

    canvasRenderer.setResolution(1, 1, 100, 200);
    ASSERT_TRUE(canvasRenderer.isDirty());
}

TEST_F(CanvasRendererTest, set_layer_test) {
    CanvasRenderer canvasRenderer(2, 0, 0, 1024, 768);
    ASSERT_TRUE(canvasRenderer.selectLayer(0));
    ASSERT_TRUE(canvasRenderer.selectLayer(1));
    ASSERT_FALSE(canvasRenderer.selectLayer(2));
}

} // Namespace UnitTest
} // Namespace OSRE

    