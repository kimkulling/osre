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
#include "osre_testcommon.h"
#include <osre/Common/Ids.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::Common;

class IdsTest : public ::testing::Test {
    // empty
};

TEST_F( IdsTest, createTest ) {
    bool ok( true );
    try {
        Ids myIds( 0 );
        Ids myIdsStartingAtZero;
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( IdsTest, allocIdsTest ) {
    Ids myIds( 0 );

    const ui32 id1 = myIds.getUniqueId();
    const ui32 id2 = myIds.getUniqueId();
    EXPECT_NE( id1, id2 );
}

TEST_F( IdsTest, releaseIdsTest ) {
    Ids myIds( 0 );

    const ui32 id1 = myIds.getUniqueId();
    const ui32 id2 = myIds.getUniqueId();
    EXPECT_NE( id1, id2 );

    myIds.releaseId( id2 );
    const ui32 id3 = myIds.getUniqueId();
    EXPECT_EQ( id2, id3 );
}

} // Namespace UnitTest
} // Namespace OSRE
