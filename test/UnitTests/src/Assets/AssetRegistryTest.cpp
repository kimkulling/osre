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
#include <gtest/gtest.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/IO/Uri.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::Assets;

class AssetRegistryTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        AssetRegistry *reg( AssetRegistry::create() );
        EXPECT_NE( nullptr, reg );
#ifdef OSRE_WINDOWS
        Assets::AssetRegistry::registerAssetPath( "assets", "../../media" );
#else
        Assets::AssetRegistry::registerAssetPath( "assets", "../media" );
#endif 
    }

    virtual void TearDown() {
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

TEST_F( AssetRegistryTest, resolve_uri_from_mount_Test ) {
    static const String ModelPath = "file://assets/Models/Obj/spider.obj";
    IO::Uri fileUri( ModelPath );
    String loc = AssetRegistry::resolvePathFromUri( fileUri );
#ifdef OSRE_WINDOWS
    static const String expRes = "../../media/Models/Obj/spider.obj";
#else
    static const String expRes = "../media/Models/Obj/spider.obj";
#endif
    EXPECT_EQ( expRes, loc );
}

}
}
