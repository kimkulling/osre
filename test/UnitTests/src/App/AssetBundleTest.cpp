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
#include "App/AssetBundle.h"
#include "IO/Uri.h"

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::App;

class AssetBundleTest : public ::testing::Test {};

TEST_F(AssetBundleTest, createTest) {
    bool ok = true;
    try {
        AssetBundle bundle("test.zip");
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE(ok);
}

TEST_F(AssetBundleTest, add_remove_Test) {
    AssetBundle bundle("test.zip");
    EXPECT_EQ(0u, bundle.getNumAssets());
    bundle.add("spider.obj");
    EXPECT_EQ(1u, bundle.getNumAssets());

    bundle.remove("spider.obj");
    EXPECT_EQ(0u, bundle.getNumAssets());
}

} // namespace App
} // namespace OSRE
