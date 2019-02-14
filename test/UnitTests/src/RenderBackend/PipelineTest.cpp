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
#include "osre_testcommon.h"
#include <osre/RenderBackend/Pipeline.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::RenderBackend;

class PipelineTest : public ::testing::Test {
protected:
    PipelinePass *m_pass1;
    PipelinePass *m_pass2;

protected:
    void SetUp() override {
        m_pass1 = new PipelinePass( RenderPassId, nullptr );
        m_pass2 = new PipelinePass( DbgPassId, nullptr );
    }

    void TearDown() override {
        delete m_pass2;
        delete m_pass1;
    }
};

TEST_F( PipelineTest, create_success ) {
    bool ok( true );
    try {
        Pipeline pipeline;
    }
    catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( PipelineTest, accessPass_success ) {
    Pipeline pipeline;

    ui32 numPasses = pipeline.getNumPasses();
    EXPECT_EQ( 0u, numPasses );
    pipeline.addPass( m_pass1 );
    numPasses = pipeline.getNumPasses();
    EXPECT_EQ( 1u, numPasses );

    pipeline.clear();
    numPasses = pipeline.getNumPasses();
    EXPECT_EQ( 0u, numPasses );
}

TEST_F( PipelineTest, iterateThroughPasses_success ) {
    Pipeline pipeline;
    pipeline.addPass( m_pass1 );
    pipeline.addPass( m_pass2 );

    ui32 numPasses = pipeline.beginFrame();
    EXPECT_EQ( 2u, numPasses );
    for ( ui32 i = 0; i < numPasses; i++ ) {
        PipelinePass *pass = pipeline.beginPass( i );
        EXPECT_NE( nullptr, pass );
        pipeline.endPass( i );
    }

    pipeline.endFrame();
    pipeline.clear();
}

TEST_F( PipelineTest, comparePipelinePasses_success ) {
    EXPECT_NE( *m_pass1, *m_pass2 );
}

} // Namespace UnitTest
} // Namespace OSRE
