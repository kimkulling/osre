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
#include <osre/Assets/AssetDataArchive.h>
#include <osre/Scene/World.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/View.h>
#include <osre/IO/Uri.h>

namespace OSRE {
namespace UnitTest {
        
using namespace ::OSRE::Assets;

class AssetDataTest : public ::testing::Test {
    // empty
};

TEST_F( AssetDataTest, createTest ) {
    bool ok( true );
    try {
        AssetDataArchive myData( 1, 0 );
    }
    catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( AssetDataTest, load_save_Test ) {
    AssetDataArchive archive( 1, 0 );
    IO::Uri uri( "file://test.osr" );
    
    // nullptr for world, must return false
    bool ok = archive.save( nullptr, uri );
    EXPECT_FALSE( ok );

    Scene::World *world = new Scene::World("test");
    Scene::Stage *stage = new Scene::Stage("stage", nullptr);
    world->setActiveStage(stage);
    Common::Ids ids;
    Scene::View *view = new Scene::View("view", ids);
    world->setActiveView(view);
    ok = archive.save(world, uri);
    EXPECT_TRUE(ok);

    world->release();
}

} // Namespace UnitTest
} // Namespace OSRE
