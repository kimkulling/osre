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
#include <osre/Common/osre_common.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE;

class CommonTest : public ::testing::Test {
    //
};

TEST_F(CommonTest, TypesTest) {
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

TEST_F( CommonTest, TPoint2_Test ) {
    Point2ui pt1;
    EXPECT_EQ(0, pt1.x);
    EXPECT_EQ(0, pt1.y);
    
    Point2ui pt2( 1, 2 );
    EXPECT_EQ( 1, pt2.x );
    EXPECT_EQ( 2, pt2.y );

    EXPECT_NE( pt1, pt2 );
    EXPECT_EQ( pt2, pt2 );
}

constexpr ui32 X = 10;
constexpr ui32 Y = 10;
constexpr ui32 W = 100;
constexpr ui32 H = 200;

TEST_F( CommonTest, TRect2D_Create ) {
    Rect2ui rect( 0, 0, 100, 20 );
    EXPECT_EQ( 0, rect.x1 );
    EXPECT_EQ( 0, rect.y1 );
    EXPECT_EQ( 100, rect.x2 );
    EXPECT_EQ( 20, rect.y2 );
    EXPECT_EQ( 100, rect.width );
    EXPECT_EQ( 20, rect.height );
}

TEST_F(CommonTest, TRect2D_IsIn ) {
    Rect2ui rect(0, 0, 100, 20);
    Point2ui pt1(10, 2);
    Point2ui pt2(102, 200);
    EXPECT_TRUE( rect.isIn( pt1 ) );
    EXPECT_FALSE( rect.isIn( pt2 ) );
}


TEST_F(CommonTest, TRect2D_set) {
    Rect2ui rect(0, 0, 1, 1);

    rect.set(X, Y, W, H);
    EXPECT_EQ(rect.getX1(), X);
    EXPECT_EQ(rect.getY1(), Y);
    EXPECT_EQ(rect.getX2(), X + W);
    EXPECT_EQ(rect.getY2(), X + H);
    EXPECT_EQ(rect.getWidth(), W);
    EXPECT_EQ(rect.getHeight(), H);
}

TEST_F(CommonTest, TRect2D_setEdges) {
    Rect2ui rect(0, 0, 1, 1);

    rect.setEdges(X, Y, X + W, Y + H);
    EXPECT_EQ(rect.getX1(), X);
    EXPECT_EQ(rect.getY1(), Y);
    EXPECT_EQ(rect.getWidth(), W);
    EXPECT_EQ(rect.getHeight(), H);
}

} // Namespace UnitTest
} // Namespace OSRE
