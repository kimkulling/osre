/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
using namespace ::OSRE::Scene;
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

TEST_F( ProjectTest, accessAssetTests ) {
    Project myProject;
    EXPECT_EQ(0u, myProject.getNumAssets());
    
    myProject.addAsset("model1");
    EXPECT_EQ(1u, myProject.getNumAssets());
    myProject.addAsset("model1");
    EXPECT_EQ(1u, myProject.getNumAssets());

    myProject.addAsset("model2");
    EXPECT_EQ(2u, myProject.getNumAssets());

    for (size_t i = 0; i < myProject.getNumAssets(); i++) {
        String name = myProject.getAssetAt(i);
        EXPECT_FALSE(name.empty());
    }

    bool ok = myProject.removeAsset("model2");
    EXPECT_TRUE(ok);

    ok = myProject.removeAsset("model2");
    EXPECT_FALSE(ok);
    EXPECT_EQ(1u, myProject.getNumAssets());
}

TEST_F( ProjectTest, accessStageTest ) {
    Stage stage("test");
    Project myProject;
    EXPECT_EQ(nullptr, myProject.getStage());

    myProject.setStage(&stage);
    EXPECT_NE(nullptr, myProject.getStage());
}

TEST_F(ProjectTest, loadsaveTest) {
    Project myProject;

}

}
}
