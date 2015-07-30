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
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::RenderBackend;

class RenderCommonTest : public ::testing::Test {
    // empty
};

TEST_F( RenderCommonTest, createVertComponentTest ) {
    bool ok( true );
    try {
        VertComponent comp;
    } catch( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );

}

TEST_F( RenderCommonTest, createVertexLayoutTest ) {
    bool ok( true );
    try {
        VertexLayout layout;
    } catch( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( RenderCommonTest, addCompVertexLayoutTest ) {
    VertComponent *comp = new VertComponent;
    VertexLayout layout;
    EXPECT_EQ( 0, layout.size() );
    layout.add( comp );
    EXPECT_EQ( 1, layout.size() );
}

TEST_F( RenderCommonTest, clearCompVertexLayoutTest ) {
    VertComponent *comp = new VertComponent;
    VertexLayout layout;
    layout.add( comp );
    EXPECT_EQ( 1, layout.size() );
    layout.clear();
    EXPECT_EQ( 0, layout.size() );
}

}
}
