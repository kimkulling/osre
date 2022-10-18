/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/TAABB.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::App;

class TAABBTest : public ::testing::Test {
    // empty
};

TEST_F( TAABBTest, createTest ) {
    bool ok( true );
    try {
        AABB aabb1;
        glm::vec3 min(0, 0, 0), max(1, 1, 1);
        AABB aabb2(min, max);
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( TAABBTest, resetTest ) {
    AABB aabb;
    glm::vec3 min(0, 0, 0), max(1, 1, 1);
    aabb.set( min, max );
    aabb.reset();
    EXPECT_NE( min, aabb.getMin() );
    EXPECT_NE( max, aabb.getMax() );
}

TEST_F( TAABBTest, get_set_Test ) {
    AABB aabb;
    glm::vec3 min(0, 0, 0), max(1, 1, 1);
    aabb.set( min, max );
    EXPECT_EQ( min, aabb.getMin() );
    EXPECT_EQ( max, aabb.getMax() );
}

TEST_F( TAABBTest, mergeTest ) {
    glm::vec3 min(1, 2, 3), max(4, 5, 6);
    AABB aabb( min, max );

    glm::vec3 newMin( -1, -2, -3 );
    aabb.merge( newMin );
    EXPECT_EQ( newMin, aabb.getMin() );

    glm::vec3 newMax(40, 50, 60);
    aabb.merge( newMax );
    EXPECT_EQ( newMax, aabb.getMax() );
}

TEST_F( TAABBTest, mergeArrayTest_Success ) {
    glm::vec3 min(1, 2, 3), max(4, 5, 6);
    AABB aabb(min, max);

    static const ui32 NumVectors = 100;
    glm::vec3 v[NumVectors];
    for ( ui32 i = 0; i < NumVectors; i++ ) {
        const f32 val( static_cast< f32 >( i ) );
        v[i].x = v[i].y = v[i].z = val;
    }
    aabb.updateFromVector3Array( v, NumVectors );
    glm::vec3 newMax(99, 99, 99);
    EXPECT_EQ( newMax, aabb.getMax() );
}

TEST_F( TAABBTest, getDiameterTest ) {
    glm::vec3 min(0, 0, 0), max(1, 1, 1);
    AABB aabb( min, max );
    const f32 diam = aabb.getDiameter();
	
	EXPECT_FLOAT_EQ( glm::length(max), diam );
}

TEST_F( TAABBTest, getCenterTest ) {
    glm::vec3 min(0, 0, 0), max(1, 1, 1);
    AABB aabb( min, max );
    glm::vec3 center = aabb.getCenter();

	glm::vec3 res(0.5, 0.5, 0.5);
	EXPECT_EQ( res, center );
}

TEST_F(TAABBTest, isInTest) {
    glm::vec3 min(-1, -1, -1), max(1, 1, 1);
    AABB aabb(min, max);

    glm::vec3 pt(0, 0, 0);
    bool result = aabb.isIn(pt);
    EXPECT_TRUE(result);

    pt = glm::vec3(-2, -2, -2);
    result = aabb.isIn(pt);
    EXPECT_FALSE(result);

    pt = glm::vec3(-2, 0, 0);
    result = aabb.isIn(pt);
    EXPECT_FALSE(result);

    pt = glm::vec3(0, -2, 0);
    result = aabb.isIn(pt);
    EXPECT_FALSE(result);

    pt = glm::vec3(0, 0, -2);
    result = aabb.isIn(pt);
    EXPECT_FALSE(result);

    pt = glm::vec3(2, 2, 2);
    result = aabb.isIn(pt);
    EXPECT_FALSE(result);

    pt = glm::vec3(2, 0, 0);
    result = aabb.isIn(pt);
    EXPECT_FALSE(result);

    pt = glm::vec3(0, 2, 0);
    result = aabb.isIn(pt);
    EXPECT_FALSE(result);

    pt = glm::vec3(0, 0, 2);
    result = aabb.isIn(pt);
    EXPECT_FALSE(result);
}

} // Namespace Unittest
} // Namespace OSRE
