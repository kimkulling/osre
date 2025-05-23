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

#include "Common/Logger.h"

namespace OSRE::UnitTest {

using namespace ::OSRE::Common;

class LoggerTest : public ::testing::Test {};

class TestLogStream : public AbstractLogStream {
public:
    String mText;

    TestLogStream() = default;
    ~TestLogStream() override = default;
    
    void write( const String &message ) override {
        mText.append(message);
        mText.append("\n");
    }
};

TEST_F(LoggerTest, logStreamTest) {
    TestLogStream logStream;
    EXPECT_TRUE(logStream.isActive());
    logStream.desactivate();
    EXPECT_FALSE(logStream.isActive());
}
    
} // namespace OSRE::UnitTest
