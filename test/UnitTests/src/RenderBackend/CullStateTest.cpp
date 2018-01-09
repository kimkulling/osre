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
#include "osre_testcommon.h"
#include <osre/RenderBackend/CullState.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::RenderBackend;

class CullStateTest : public ::testing::Test {
    // empty
};

TEST_F( CullStateTest, create_success ) {
    bool ok( true );
    try {
        CullState state1;
        CullState state2( CullState::CullMode::CCW, CullState::CullFace::Back );
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( CullStateTest, access_CullMode_success ) {
    CullState state1, state2, state3( CullState::CullMode::CW, CullState::CullFace::Back );
    state1.setCullMode( CullState::CullMode::CCW );
    EXPECT_EQ( CullState::CullMode::CCW, state1.getCullMode() );

    state2.setCullMode( CullState::CullMode::CCW );
    EXPECT_EQ( state1, state2 );
    EXPECT_NE( state1, state3 );
}

TEST_F( CullStateTest, access_CullFace_success ) {
    CullState state1, state2( CullState::CullMode::CW, CullState::CullFace::FrontAndBack );
    state1.setCullFace( CullState::CullFace::FrontAndBack );
    EXPECT_EQ( CullState::CullFace::FrontAndBack, state1.getCullFace() );

    EXPECT_EQ( state1, state2 );
}

} // Namespace UnitTest
} // Namespace OSRE
