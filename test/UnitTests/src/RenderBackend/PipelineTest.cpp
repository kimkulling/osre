/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "RenderBackend/Pipeline.h"
#include "RenderBackend/RenderBackendService.h"

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::RenderBackend;

static constexpr guid FrameBufferId = 0;
class PipelineTest : public ::testing::Test {
protected:
    RenderBackendService *mRbService;
    RenderPass *mPass1;
    RenderPass *mPass2;

    void SetUp() override {
        mRbService = new RenderBackendService;
        mPass1 = RenderPassFactory::create(RenderPassId, FrameBufferId);
        mPass2 = RenderPassFactory::create(DbgPassId, FrameBufferId);
    }

    void TearDown() override {
        delete mRbService;
    }
};

TEST_F( PipelineTest, create_success ) {
    bool ok( true );
    try {
        Pipeline *pipeline = new Pipeline("p1");
        delete (pipeline);
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( PipelineTest, accessPass_success ) {
    Pipeline *pipeline = new Pipeline("p1");

    size_t numPasses = pipeline->getNumPasses();
    EXPECT_EQ( 0u, numPasses );
    pipeline->addPass( mPass1 );
    numPasses = pipeline->getNumPasses();
    EXPECT_EQ( 1u, numPasses );

    pipeline->clear();
    numPasses = pipeline->getNumPasses();
    EXPECT_EQ( 0u, numPasses );
}

TEST_F( PipelineTest, iterateThroughPasses_success ) {
    Pipeline *pipeline = new Pipeline("p1");
    pipeline->addPass(mPass1);
    pipeline->addPass(mPass2);

    const size_t numPasses = pipeline->beginFrame();
    EXPECT_EQ( 2u, numPasses );
    for ( ui32 i = 0; i < numPasses; i++ ) {
        RenderPass *pass = pipeline->beginPass(i);
        EXPECT_NE( nullptr, pass );
        pipeline->endPass( i );
    }

    pipeline->endFrame();
    pipeline->clear();

    delete pipeline;
}

TEST_F( PipelineTest, comparePipelinePasses_success ) {
    EXPECT_NE( *mPass1, *mPass2 );
}

TEST_F( PipelineTest, getPassByIndexTest ) {
    Pipeline *pipeline = new Pipeline("p1");
    pipeline->addPass(mPass1);    
    pipeline->addPass(mPass2);

    RenderPass *foundPass = pipeline->getPassById(mPass1->getId());
    EXPECT_EQ(mPass1, foundPass);

    foundPass = pipeline->getPassById(mPass2->getId());
    EXPECT_EQ(mPass2, foundPass);

    delete pipeline;
}

} // Namespace UnitTest
} // Namespace OSRE
