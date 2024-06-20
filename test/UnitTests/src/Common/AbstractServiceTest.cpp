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

#include "Common/AbstractService.h"

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::Common;

class TestService : public AbstractService {
    bool m_shallFail;

public:
    TestService()
        : AbstractService( "TestService" )
        , m_shallFail( false ) {
        // empty
    }

    virtual ~TestService() {
        // empty
    }

    void setShallFail( bool shallfail ) {
        m_shallFail = shallfail;
    }
protected:
    virtual bool onOpen() override {
        return !m_shallFail;
    }

    virtual bool onClose()  override {
        return !m_shallFail;
    }

    virtual bool onUpdate() override {
        return !m_shallFail;
    }
};

class AbstractServiceTest : public ::testing::Test {
public:
    TestService *m_ts;

protected:
    virtual void SetUp() {
        m_ts = new TestService;
    }

    virtual void TearDown() {
        m_ts->setShallFail( false );
        if ( m_ts->isOpen() ) {
            m_ts->close();
        }
        delete m_ts;
        m_ts = nullptr;
    }
};

TEST_F( AbstractServiceTest, createTest ) {
    EXPECT_NE( m_ts, nullptr );
}

TEST_F( AbstractServiceTest, openTest ) {
    bool result( m_ts->open() );
    EXPECT_TRUE( result );

    result = m_ts->open();
    EXPECT_FALSE( result );
}

TEST_F( AbstractServiceTest, openWithErrorTest ) {
    m_ts->setShallFail( true );

    bool result( m_ts->open() );
    EXPECT_FALSE( result );

    result = m_ts->isOpen();
    EXPECT_FALSE( result );
}

TEST_F( AbstractServiceTest, closeTest ) {
    bool result( m_ts->open() );
    EXPECT_TRUE( result );

    result = m_ts->close();
    EXPECT_TRUE( result );

    result = m_ts->close();
    EXPECT_FALSE( result );
}

} // Namespace UnitTest
} // Namespace OSRE
