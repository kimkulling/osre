/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "App/Stage.h"

namespace OSRE {
namespace UnitTest {

using namespace OSRE::App;

class StageTest : public ::testing::Test {
    // empty
};

TEST_F(StageTest, createTest) {
    bool ok = true;
    try {
        Stage theStage("test", StageMode::Stage2D);
    } catch(...) {
        ok = false;
    }
    EXPECT_TRUE(ok);
}

TEST_F(StageTest, clearTest) {
    Stage theStage("test", StageMode::Stage2D);
    static_cast<void>(theStage.createWorld("stageTest"));
    EXPECT_EQ(1u, theStage.getNumberOfWorlds());

    theStage.clear();
    EXPECT_EQ(0u, theStage.getNumberOfWorlds());
    EXPECT_TRUE(theStage.isEmpty());
}

TEST_F(StageTest, accessStageModeTest) {
    Stage theStage1("test", StageMode::Stage2D);
    EXPECT_EQ(StageMode::Stage2D, theStage1.getStageMode());

    Stage theStage2("test", StageMode::Stage3D);
    EXPECT_EQ(StageMode::Stage3D, theStage2.getStageMode());
}

} // namespace UnitTest
} // namespace OSRE
