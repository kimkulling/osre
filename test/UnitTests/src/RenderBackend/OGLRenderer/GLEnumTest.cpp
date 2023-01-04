/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/RenderBackend/RenderStates.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::RenderBackend;

class OGLEnumTest : public ::testing::Test {
    // empty
};

TEST_F(OGLEnumTest, access_cullstate_success) {
    CullState state;
    state.m_cullMode = CullState::CullMode::CW;
    EXPECT_EQ( GL_CW, (GLint) OGLEnum::getOGLCullState(state.m_cullMode));

    state.m_cullMode = CullState::CullMode::CCW;
    EXPECT_EQ(GL_CCW, (GLint) OGLEnum::getOGLCullState(state.m_cullMode));
}

TEST_F( OGLEnumTest, access_cullFace_success ) {
    CullState state;
    state.m_cullFace = CullState::CullFace::Front;
    EXPECT_EQ( GL_FRONT, (GLint) OGLEnum::getOGLCullFace( state.m_cullFace ) );

    state.m_cullFace = CullState::CullFace::Back;
    EXPECT_EQ(GL_BACK, (GLint)OGLEnum::getOGLCullFace(state.m_cullFace));

    state.m_cullFace = CullState::CullFace::FrontAndBack;
    EXPECT_EQ(GL_FRONT_AND_BACK, (GLint)OGLEnum::getOGLCullFace(state.m_cullFace));
}

} // Namespace UnitTest
} // Namespace OSRE

