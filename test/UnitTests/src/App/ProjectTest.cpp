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
#include <gtest/gtest.h>
#include <osre/App/Project.h>
#include <osre/Common/osre_common.h>
#include <osre/IO/Directory.h>

namespace OSRE {
namespace UnitTest {
    
using namespace ::OSRE::App;
using namespace ::OSRE::IO;

class ProjectTest : public ::testing::Test {
    // empty
};

TEST_F(ProjectTest, createTest) {
    bool ok(true);
    try {
        Project myProject;
    }
    catch (...) {
        ok = false;
    }
    EXPECT_TRUE(ok);
}

TEST_F(ProjectTest, loadsaveTest) {
    Project myProject;

    bool res(true);
    // Not created must return false
    String oldPath, newPath;
    oldPath = Directory::getCurrentDirectory();
    res = myProject.save("test", 0);
    EXPECT_FALSE(res);

    //// Save test ////

    // Created must return true
    res = myProject.create("test", 0, 1);
    EXPECT_TRUE(res);
    res = myProject.save("test", 0);
    EXPECT_TRUE(res);
    newPath = Directory::getCurrentDirectory();
    EXPECT_EQ(oldPath, newPath);

    //// Load test ////

    Project myProject1;
    i32 majorVersion(-1), minorVersion(-1);
    oldPath = Directory::getCurrentDirectory();
    res = myProject1.load("test", majorVersion, minorVersion, 0);
    EXPECT_TRUE(res);
    newPath = Directory::getCurrentDirectory();
    EXPECT_EQ(oldPath, newPath);

    EXPECT_EQ(0, myProject1.getMajorVersion());
    EXPECT_EQ(1, myProject1.getMinorVersion());
}

}
}
