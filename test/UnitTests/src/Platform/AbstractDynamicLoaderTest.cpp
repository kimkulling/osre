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
#include "osre_testcommon.h"
#include "Platform/AbstractDynamicLoader.h"

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::Platform;

class TestAbstractDynamicLoader : public AbstractDynamicLoader {
public:
    TestAbstractDynamicLoader() : AbstractDynamicLoader() {}

    virtual ~TestAbstractDynamicLoader() override = default;
    
    virtual LibHandle *load( const String & ) override {
        return nullptr;
    }

    virtual LibHandle *lookupLib( const String & ) override {
        return nullptr;
    }

    virtual void unload( const String & ) override {
        // empty
    }

    virtual void *loadFunction( const String & ) override {
        return nullptr;
    }
};

class AbstractDynamicLoaderTest : public ::testing::Test {};

TEST_F( AbstractDynamicLoaderTest, createTest ) {
    bool ok( true );
    try {
        TestAbstractDynamicLoader myLoader;
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( AbstractDynamicLoaderTest, accessLibHandleTest ) {
    LibHandle *handle = new LibHandle;
    TestAbstractDynamicLoader myLoader;

    String libName( "testlib" );
    LibHandle *found( myLoader.findLib( libName ) );
    EXPECT_EQ( found, nullptr );

    myLoader.addLib( libName, handle );
    found = myLoader.findLib( libName );
    EXPECT_NE( found, nullptr );
    EXPECT_EQ( found, handle );

    myLoader.removeLib( libName );
    found = myLoader.findLib( libName );
    EXPECT_EQ( found, nullptr );
}

TEST_F( AbstractDynamicLoaderTest, accessActiveLibTest ) {
    LibHandle *handle = new LibHandle;
    TestAbstractDynamicLoader myLoader;
    EXPECT_EQ( myLoader.getActiveLib(), nullptr );

    String libName( "testlib" );
    myLoader.addLib( libName, handle );
    EXPECT_EQ( myLoader.getActiveLib(), handle );

    myLoader.removeLib( libName );
    EXPECT_EQ( myLoader.getActiveLib(), nullptr );
}

} // Namespace UnitTest
} // Namespace OSRE
