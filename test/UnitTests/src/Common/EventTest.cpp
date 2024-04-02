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
#include <osre/Common/Event.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::Common;

class EventTest : public ::testing::Test {
    // empty
};

TEST_F(EventTest, createTest) {
    const c8 *name( "test_event" );
    Event myEvent(name);
    EXPECT_NE( static_cast<ui32>(0), myEvent.getHash() );
    Event myEvent1(name);
    EXPECT_EQ( myEvent, myEvent1 );
}

TEST_F(EventTest, calculateHashFromEventNameTest) {
    const c8 *name("test_event");
    Event myEvent(name);
    const HashId hash = myEvent.getHash();
    EXPECT_NE( hash, (ui32) 0 );

    const char *empty_name = "";
    Event myEvent1(empty_name);

    const HashId hash_empty = myEvent1.getHash();
    EXPECT_EQ(hash_empty, static_cast<HashId>(0));
}

} // Namespace UnitTest
} // Namespace OSRE
