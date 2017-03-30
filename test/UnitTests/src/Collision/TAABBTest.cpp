/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Collision/TAABB.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::Collision;

class TAABBTest : public ::testing::Test {
    // empty
};

TEST_F( TAABBTest, createTest ) {
    bool ok( true );
    try {
        TAABB<f32> aabb1;
        Vec3f min( 0, 0, 0 ), max( 1, 1, 1 );
        TAABB<f32> aabb2( min, max );
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( TAABBTest, resetTest ) {
    TAABB<f32> aabb;
    Vec3f min( 0, 0, 0 ), max( 1, 1, 1 );
    aabb.set( min, max );
    aabb.reset();
    EXPECT_NE( min, aabb.getMin() );
    EXPECT_NE( max, aabb.getMax() );
}

TEST_F( TAABBTest, get_set_Test ) {
    TAABB<f32> aabb;
    Vec3f min( 0, 0, 0 ), max( 1, 1, 1 );
    aabb.set( min, max );
    EXPECT_EQ( min, aabb.getMin() );
    EXPECT_EQ( max, aabb.getMax() );
}

TEST_F( TAABBTest, mergeTest ) {
    Vec3f min( 1, 2, 3 ), max( 4, 5, 6 );
    TAABB<f32> aabb( min, max );

    Vec3f newMin( -1, -2, -3 );
    aabb.merge( newMin );
    EXPECT_EQ( newMin, aabb.getMin() );

    Vec3f newMax( 40, 50, 60 );
    aabb.merge( newMax );
    EXPECT_EQ( newMax, aabb.getMax() );
}

TEST_F( TAABBTest, mergeArrayTest_Success ) {
    Vec3f min( 1, 2, 3 ), max( 4, 5, 6 );
    TAABB<f32> aabb( min, max );

    static const ui32 NumVectors = 100;
    Vec3f v[ NumVectors ];
    for ( ui32 i = 0; i < NumVectors; i++ ) {
        v[ i ].set( i, i, i );
    }
    aabb.updateFromVector3Array( v, NumVectors );
    Vec3f newMax( 99, 99, 99 );
    EXPECT_EQ( newMax, aabb.getMax() );
}

TEST_F( TAABBTest, getDiameterTest ) {
    Vec3f min( 0, 0, 0 ), max( 1, 1, 1 );
    TAABB<f32> aabb( min, max );
    const f32 diam = aabb.getDiameter();
	
	EXPECT_FLOAT_EQ( max.getLength(), diam );
}

TEST_F( TAABBTest, getCenterTest ) {
    Vec3f min( 0, 0, 0 ), max( 1, 1, 1 );
    TAABB<f32> aabb( min, max );
    Vec3f center = aabb.getCenter();

	Vec3f res( 0.5, 0.5, 0.5 );
	EXPECT_EQ( res, center );
}

} // Namespace Unittest
} // Namespace OSRE
