/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/AbstractProcessor.h>

namespace OSRE {
namespace UnitTest {

class AbstractProcessorTest : public ::testing::Test {
    // empty
};

class TestAbstractProcessor : public Common::AbstractProcessor {
public:
    TestAbstractProcessor() 
    : AbstractProcessor() {
        // empty
    }

    virtual ~TestAbstractProcessor() {
        // empty
    }

    bool execute() override {
        return true;
    }
};

TEST_F( AbstractProcessorTest, create_sussess ) {
    bool ok = true;
    try {
        TestAbstractProcessor processor;
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( AbstractProcessorTest, addDependency_Success ) {
    TestAbstractProcessor pre_processor;
    TestAbstractProcessor processor;
    processor.addDependency( &pre_processor );

    const Common::AbstractProcessor::ProcessorArray &arr( processor.getPreconditions() );
    EXPECT_EQ( 1U, arr.size() );
}

} // Namespace UnitTest
} // Namespace OSRE
