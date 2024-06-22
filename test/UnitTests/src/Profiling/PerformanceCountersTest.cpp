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
#include "Profiling/PerformanceCounterRegistry.h"

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::Profiling;

class PerformanceCountersTest : public ::testing::Test {
    // empty
};

static const String TestKey = "test";

TEST_F( PerformanceCountersTest, createTest ) {
    bool ok( true );
    ok = PerformanceCounterRegistry::create();
    EXPECT_TRUE( ok );
    ok = PerformanceCounterRegistry::create();
    EXPECT_FALSE( ok );
    ok = PerformanceCounterRegistry::destroy();
    EXPECT_TRUE( ok );
    ok = PerformanceCounterRegistry::destroy();
    EXPECT_FALSE( ok );
}

TEST_F( PerformanceCountersTest, registerCounterTest ) {
    bool ok = PerformanceCounterRegistry::create();
    EXPECT_TRUE( ok );

    ui32 v( 0 );
    ok = PerformanceCounterRegistry::queryCounter( TestKey, v );
    EXPECT_FALSE( ok );

    ok = PerformanceCounterRegistry::registerCounter( TestKey );
    EXPECT_TRUE( ok );

    ok = PerformanceCounterRegistry::queryCounter( TestKey, v );
    EXPECT_TRUE( ok );
    EXPECT_EQ( v, 0U );

    ok = PerformanceCounterRegistry::unregisterCounter( TestKey );
    EXPECT_TRUE( ok );

    ok = PerformanceCounterRegistry::unregisterCounter( TestKey );
    EXPECT_FALSE( ok );

    ok = PerformanceCounterRegistry::destroy();
    EXPECT_TRUE( ok );
}

TEST_F( PerformanceCountersTest, setCounterTest ) {
    bool ok = PerformanceCounterRegistry::create();
    EXPECT_TRUE( ok );

    ok = PerformanceCounterRegistry::registerCounter( TestKey );
    EXPECT_TRUE( ok );

    PerformanceCounterRegistry::addValueToCounter( TestKey, 10 );
    EXPECT_TRUE( ok );

    ui32 v( 0 );
    ok = PerformanceCounterRegistry::queryCounter( TestKey, v );
    EXPECT_TRUE( ok );
    EXPECT_EQ( v, 10U );

    ok = PerformanceCounterRegistry::destroy();
    EXPECT_TRUE( ok );
}

TEST_F( PerformanceCountersTest, resetCounterTest ) {
    bool ok = PerformanceCounterRegistry::create();
    EXPECT_TRUE( ok );

    ok = PerformanceCounterRegistry::registerCounter( TestKey );
    EXPECT_TRUE( ok );

    PerformanceCounterRegistry::addValueToCounter( TestKey, 10 );
    EXPECT_TRUE( ok );

    ui32 v( 0 );
    ok = PerformanceCounterRegistry::queryCounter( TestKey, v );
    EXPECT_TRUE( ok );
    EXPECT_EQ( v, 10U );

    PerformanceCounterRegistry::resetCounter( "test" );
    ok = PerformanceCounterRegistry::destroy();
    EXPECT_TRUE( ok );
}

} // Namespace UnitTest
} // Namespace OSRE
