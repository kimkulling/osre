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
#include "osre_testcommon.h"
#include <osre/Common/Frustum.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::Common;

class FrustumTest : public ::testing::Test {
    // empty
};

TEST_F( FrustumTest, createTest ) {
    bool ok = true;
    try {
        Frustum f;
    } catch (...) {
        ok = false;
    }
    EXPECT_TRUE(ok);
}

TEST_F( FrustumTest, extractFromTest ) {
    glm::vec3 pos(-10, 10, 0), center(0, 0, 0), up(0, 0, 1);
    glm::mat4 v = glm::lookAt(pos, center, up);
    glm::mat4 p = glm::perspective(1.2f, 1.f, 0.1f, 100.0f);
    glm::mat4 vp = p * v;
    Frustum f;
    f.extractFrom(vp);
    glm::vec3 point1(5, 5, 0);
    bool result = f.isIn(point1);
    EXPECT_TRUE(result);

    glm::vec3 point2(-11, 5, 0);
    result = f.isIn(point2);
    EXPECT_FALSE(result);
}

} // namespace UnitTest
} // namespace OSRE

