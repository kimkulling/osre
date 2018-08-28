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
#include <gtest/gtest.h>
#include <osre/UI/Widget.h>
#include <osre/RenderBackend/RenderBackendService.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::UI;
using namespace ::OSRE::RenderBackend;

class WidgetTest : public ::testing::Test {
    // empty
};

class TestWidget : public Widget {
public:
    TestWidget( const String &name, Widget *parent )
    : Widget( name, parent )
    , m_x( 0 )
    , m_y( 0 )
    , m_w( 0 )
    , m_h( 0 ) {
        // empty
    }

    ~TestWidget() {
        // empty
    }

protected:
    void onRender( UiRenderCmdCache &, RenderBackendService *rbSrv ) override {
        EXPECT_NE(nullptr, rbSrv);
    }

    void onResize( ui32 x, ui32 y, ui32 w, ui32 h ) override {
        m_x = x;
        m_y = y;
        m_w = w;
        m_h = h;
    }
    ui32 m_x, m_y, m_w, m_h;
};
TEST_F( WidgetTest, createTest ) {
    bool ok( true );
    try {
        TestWidget testWidget( "test", nullptr );
    } catch ( ... ) {
        ok = false;
    }
}

TEST_F( WidgetTest, access_rect_Test ) {
    TestWidget testWidget( "test", nullptr );
    Rect2ui myrect( 0, 1, 2, 3 );
    testWidget.setRect( 0, 1, 2, 3 );
    EXPECT_EQ( myrect, testWidget.getRect() );
}

TEST_F( WidgetTest, access_children_Test ) {
    TestWidget testWidget( "test", nullptr );
    EXPECT_EQ( 0U, testWidget.getNumChildren() );

    String names[ 3 ] = {
        "child1",
        "child2",
        "child3"
    };
    TestWidget child1( names[ 0 ], &testWidget );
    TestWidget child2( names[ 1 ], &testWidget );
    TestWidget child3( names[ 2 ], &testWidget );

    EXPECT_EQ( 3U, testWidget.getNumChildren() );
    for ( ui32 i = 0; i < testWidget.getNumChildren(); i++ ) {
        Widget *current( testWidget.getChildWidgetAt( i ) );
        EXPECT_NE( nullptr, current );
        EXPECT_EQ( names[ i ], current->getName() );
        EXPECT_EQ( &testWidget, current->getParent() );
    }

    bool ok = testWidget.removeChildWidget( &child2 );
    EXPECT_TRUE( ok );

    ok = testWidget.removeChildWidget( &child2 );
    EXPECT_FALSE( ok );

    EXPECT_EQ( 2, testWidget.getNumChildren() );
}

TEST_F(WidgetTest, hasChild_ReturnsTrue) {
    TestWidget parent("test", nullptr);
    TestWidget testWidget("test", nullptr);

    EXPECT_FALSE( parent.hasChild( &testWidget ) );

    parent.addChildWidget(&testWidget);
    EXPECT_TRUE(parent.hasChild(&testWidget));
}

TEST_F( WidgetTest, request_redraw_Test ) {
    TestWidget testWidget( "test", nullptr );
    EXPECT_TRUE( testWidget.redrawRequested() );
    UiVertexCache vertexCache( 10 );
    UiIndexCache indexCache( 10 );
    RenderBackend::RenderBackendService *rb = new RenderBackend::RenderBackendService;
    UiRenderCmdCache renderCmdCache;
    testWidget.render( renderCmdCache, rb );
    EXPECT_FALSE( testWidget.redrawRequested() );
    delete rb;
}

TEST_F( WidgetTest, WidgetCoordMappingTest ) {
    Rect2ui rect( 0, 0, 100, 200 );
    f32 mappedX, mappedY;
    WidgetCoordMapping::mapPosToWorld( rect, 0, 0, mappedX, mappedY );
    EXPECT_FLOAT_EQ( -1.0f, mappedX );
    EXPECT_FLOAT_EQ( 1.0f, mappedY );

    WidgetCoordMapping::mapPosToWorld( rect, 100, 200, mappedX, mappedY );
    EXPECT_FLOAT_EQ( 1.0f, mappedX );
    EXPECT_FLOAT_EQ( -1.0f, mappedY );

    WidgetCoordMapping::mapPosToWorld( rect, 50, 100, mappedX, mappedY );
    EXPECT_FLOAT_EQ( 0.0f, mappedX );
    EXPECT_FLOAT_EQ( 0.0f, mappedY );
}

} // Namespace UnitTest
} // Namespace OSRE
