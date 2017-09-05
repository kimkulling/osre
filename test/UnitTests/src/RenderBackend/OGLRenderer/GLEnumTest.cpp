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
#include "src/Engine/RenderBackend/OGLRenderer/OGLEnum.h"
#include <osre/RenderBackend/CullState.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::RenderBackend;

class OGLEnumTest : public ::testing::Test {
    // empty
};

TEST_F( OGLEnumTest, access_cullstate_success ) {
    CullState state;
    state.setCullMode( CullState::CullMode::CW );
    EXPECT_EQ( GL_CW, OGLEnum::getOGLCullState( state.getCullMode() ) );

    state.setCullMode( CullState::CullMode::CCW );
    EXPECT_EQ( GL_CCW, OGLEnum::getOGLCullState( state.getCullMode() ) );
}

TEST_F( OGLEnumTest, access_cullFace_success ) {
    CullState state;
    state.setCullFace( CullState::CullFace::Front );
    EXPECT_EQ( GL_FRONT, OGLEnum::getOGLCullFace( state.getCullFace() ) );

    state.setCullFace( CullState::CullFace::Back );
    EXPECT_EQ( GL_BACK, OGLEnum::getOGLCullFace( state.getCullFace() ) );

    state.setCullFace( CullState::CullFace::FrontAndBack );
    EXPECT_EQ( GL_FRONT_AND_BACK, OGLEnum::getOGLCullFace( state.getCullFace() ) );
}

} // Namespace UnitTest
} // Namespace OSRE

