/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/osre_common.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE;

class CommonTest : public ::testing::Test {
    //
};

TEST_F( CommonTest, TypesTest ) {
    EXPECT_EQ( 4U, sizeof( ui32 ) );
    EXPECT_EQ( 4U, sizeof( i32 ) );
    EXPECT_EQ( 4U, sizeof( f32 ) );

    EXPECT_EQ( 2U, sizeof( ui16 ) );
    EXPECT_EQ( 2U, sizeof( i16 ) );

    EXPECT_EQ( 1U, sizeof( uc8 ) );
    EXPECT_EQ( 1U, sizeof( c8 ) );
}

TEST_F( CommonTest, HandleTest ) {
    Handle testHandle;
    EXPECT_EQ( -1, testHandle.m_idx );

    testHandle.m_idx = 1;
    testHandle.init( 1);
    EXPECT_EQ( 1U, testHandle.m_idx );
}

TEST_F( CommonTest, HandleEualTest ) {
    Handle testHandle1, testHandle2;
    testHandle1.init( 1 );
    testHandle2.init( 1 );
    EXPECT_EQ( testHandle1, testHandle2 );

    testHandle2.init( 2 );
    EXPECT_NE( testHandle1, testHandle2 );
}

TEST_F( CommonTest, Color4Test ) {
    Color4 col1( 0.1f, 0.2f, 0.3f, 0.4f ), col2( 0.1f, 0.2f, 0.3f, 0.4f ), col3( 0.5f, 0.6f, 0.7f, 0.8f );
    EXPECT_EQ( col1, col2 );
    EXPECT_NE( col2, col3 );
}

TEST_F( CommonTest, TVec2Test ) {
    Vec2f vec2_1( 1, 2 ), vec2_2(1,2);
    EXPECT_FLOAT_EQ( 1, vec2_1.getX() );
    EXPECT_FLOAT_EQ( 2, vec2_1.getY() );
    EXPECT_EQ( vec2_1, vec2_2 );

    Vec2f v2;
    v2.setX( 1 );
    EXPECT_EQ( 1, v2.getX() );

    v2.setY( 2 );
    EXPECT_EQ( 2, v2.getY() );
}

TEST_F( CommonTest, TVec2_add_sub_Test ) {
    Vec2f vec2_1( 1, 2 ), vec2_2( 3, 4 );
    Vec2f res1 = vec2_1 + vec2_2;
    EXPECT_FLOAT_EQ( 4, res1.getX() );
    EXPECT_FLOAT_EQ( 6, res1.getY() );

    Vec2f res2 = vec2_1 - vec2_2;
    EXPECT_FLOAT_EQ( -2, res2.getX() );
    EXPECT_FLOAT_EQ( -2, res2.getY() );
}

TEST_F( CommonTest, TVec2_mul_with_scalar_Test ) {
    Vec2f vec2_1( 1, 2 );
    Vec2f res1 = 2.0f*vec2_1;
    EXPECT_FLOAT_EQ( 2, res1.getX() );
    EXPECT_FLOAT_EQ( 4, res1.getY() );

    Vec2f res2 = vec2_1 * 2;
    EXPECT_FLOAT_EQ( 2, res2.getX() );
    EXPECT_FLOAT_EQ( 4, res2.getY() );
}

TEST_F( CommonTest, TVec3Test ) {
    Vec3f vec3_1( 1, 2, 3 ), vec3_2( 1, 2, 3 );
    EXPECT_FLOAT_EQ( 1, vec3_1.getX() );
    EXPECT_FLOAT_EQ( 2, vec3_1.getY() );
    EXPECT_FLOAT_EQ( 3, vec3_1.getZ() );
    EXPECT_EQ( vec3_1, vec3_2 );

    Vec3f v3;
    v3.setX( 1 );
    EXPECT_EQ( 1, v3.getX() );

    v3.setY( 2 );
    EXPECT_EQ( 2, v3.getY() );

    v3.setZ( 3 );
    EXPECT_EQ( 3, v3.getZ() );
}

TEST_F( CommonTest, TVec3_add_sub_Test ) {
    Vec3f vec3_1( 1, 2, 3 ), vec3_2( 4, 5, 6 );
    Vec3f res1 = vec3_1 + vec3_2;
    EXPECT_FLOAT_EQ( 5, res1.getX() );
    EXPECT_FLOAT_EQ( 7, res1.getY() );
    EXPECT_FLOAT_EQ( 9, res1.getZ() );

    Vec3f res2 = vec3_1 - vec3_2;
    EXPECT_FLOAT_EQ( -3, res2.getX() );
    EXPECT_FLOAT_EQ( -3, res2.getY() );
    EXPECT_FLOAT_EQ( -3, res2.getZ() );
}

TEST_F( CommonTest, TVec3_mul_with_scalar_Test ) {
    Vec3f vec3_1( 1, 2, 3 );
    Vec3f res1 = 2.0f * vec3_1;
    EXPECT_FLOAT_EQ( 2, res1.getX() );
    EXPECT_FLOAT_EQ( 4, res1.getY() );
    EXPECT_FLOAT_EQ( 6, res1.getZ() );

    Vec3f res2 = vec3_1 * 2.0f;
    EXPECT_FLOAT_EQ( 2, res2.getX() );
    EXPECT_FLOAT_EQ( 4, res2.getY() );
    EXPECT_FLOAT_EQ( 6, res2.getZ() );
}

TEST_F( CommonTest, TVec4Test ) {
    Vec4f vec4_1( 1, 2, 3, 1 ), vec4_2( 1, 2, 3, 1 );
    EXPECT_FLOAT_EQ( 1, vec4_1.getX() );
    EXPECT_FLOAT_EQ( 2, vec4_1.getY() );
    EXPECT_FLOAT_EQ( 3, vec4_1.getZ() );
    EXPECT_FLOAT_EQ( 1, vec4_1.getW() );
    EXPECT_EQ( vec4_1, vec4_2 );

    Vec4f v4;
    v4.setX( 1 );
    EXPECT_EQ( 1, v4.getX() );

    v4.setY( 2 );
    EXPECT_EQ( 2, v4.getY() );

    v4.setZ( 3 );
    EXPECT_EQ( 3, v4.getZ() );

    v4.setW( 1 );
    EXPECT_EQ( 1, v4.getW() );
}

TEST_F( CommonTest, TVec4_add_sub_Test ) {
    Vec4f vec4_1( 1, 2, 3, 1 ), vec4_2( 4, 5, 6, 1 );
    Vec4f res1 = vec4_1 + vec4_2;
    EXPECT_FLOAT_EQ( 5, res1.getX() );
    EXPECT_FLOAT_EQ( 7, res1.getY() );
    EXPECT_FLOAT_EQ( 9, res1.getZ() );
    EXPECT_FLOAT_EQ( 1, res1.getW() );

    Vec4f res2 = vec4_1 - vec4_2;
    EXPECT_FLOAT_EQ( -3, res2.getX() );
    EXPECT_FLOAT_EQ( -3, res2.getY() );
    EXPECT_FLOAT_EQ( -3, res2.getZ() );
    EXPECT_FLOAT_EQ(  1, res2.getW() );
}

TEST_F( CommonTest, TQuatTest ) {
    Quatf q;
    EXPECT_FLOAT_EQ( 0, q.m_x );
    EXPECT_FLOAT_EQ( 0, q.m_y );
    EXPECT_FLOAT_EQ( 0, q.m_z );
    EXPECT_FLOAT_EQ( 1, q.m_w );
}

TEST_F( CommonTest, TPoint2_Test ) {
    Point2ui pt1;
    EXPECT_EQ(0, pt1.m_x);
    EXPECT_EQ(0, pt1.m_y);
    
    Point2ui pt2( 1, 2 );
    EXPECT_EQ( 1, pt2.m_x );
    EXPECT_EQ( 2, pt2.m_y );

    EXPECT_NE( pt1, pt2 );
    EXPECT_EQ( pt2, pt2 );
}

TEST_F( CommonTest, TRect2D_Create ) {
    Rect2ui rect( 0, 0, 100, 20 );
    EXPECT_EQ( 0, rect.m_x1 );
    EXPECT_EQ( 0, rect.m_y1 );
    EXPECT_EQ( 100, rect.m_x2 );
    EXPECT_EQ( 20, rect.m_y2 );
    EXPECT_EQ( 100, rect.m_width );
    EXPECT_EQ( 20, rect.m_height );
}

TEST_F(CommonTest, TRect2D_IsIn ) {
    Rect2ui rect(0, 0, 100, 20);
    Point2ui pt1(10, 2);
    Point2ui pt2(102, 200);
    EXPECT_TRUE( rect.isIn( pt1 ) );
    EXPECT_FALSE( rect.isIn( pt2 ) );
}

} // Namespace UnitTest
} // Namespace OSRE
