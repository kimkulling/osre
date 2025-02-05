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

#include "App/AssetRegistry.h"
#include "IO/Uri.h"

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::App;

class AssetRegistryTest : public ::testing::Test {
protected:
    void SetUp() override{
        AssetRegistry *reg( AssetRegistry::create() );
        EXPECT_NE( nullptr, reg );
#ifdef OSRE_WINDOWS
        AssetRegistry::registerAssetPath( "assets", "../../assets" );
#else
        AssetRegistry::registerAssetPath( "assets", "../assets" );
#endif 
    }

    void TearDown() override {
        AssetRegistry::destroy();
    }
};

TEST_F( AssetRegistryTest, createTest ) {
    bool ok( true );
    try {
        AssetRegistry *reg( AssetRegistry::create() );
        EXPECT_NE( nullptr, reg );

        AssetRegistry::destroy();
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( AssetRegistryTest, resolveUriFromMountTest ) {
    static const String ModelPath = "file://assets/Models/Obj/spider.obj";
    const IO::Uri fileUri( ModelPath );
    const String loc = AssetRegistry::resolvePathFromUri( fileUri );
#ifdef OSRE_WINDOWS
    static const String expRes = "../../assets/Models/Obj/spider.obj";
#else
    static const String expRes = "../assets/Models/Obj/spider.obj";
#endif
    EXPECT_EQ( expRes, loc );
}

} // namespace App
} // namespace OSRE
