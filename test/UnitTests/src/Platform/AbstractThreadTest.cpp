/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Platform/AbstractThread.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::Platform;

class TestThread : public AbstractThread {
public:
    TestThread() : AbstractThread() {
    }

    virtual bool start(void *pData) {
        return true;
    }

    virtual bool stop() {
        return true;
    }

    virtual bool suspend() {
        return true;
    }

    virtual bool resume() {
        return true;
    }

    virtual void setName(const String &name) {
        // empty
    }

    virtual const String &getName() const {
        static const String Name = "test";
        return Name;
    }

    virtual void waitForTimeout( ui32 ms ) {
        // empty
    }

    virtual void wait() {
        // empty
    }

    virtual AbstractThreadEvent *getThreadEvent() const {
        return nullptr;
    }

    virtual void setPriority( Priority prio ) {
        // empty
    }

    virtual Priority getPriority() const {
        return AbstractThread::Priority::Normal;
    }

    virtual const String &getThreadName() const {
        static const String ThreadName = "testthread";
        return ThreadName;
    }

    virtual AbstractThreadLocalStorage *getThreadLocalStorage() {
        return nullptr;
    }

    virtual void setThreadLocalStorage( AbstractThreadLocalStorage *tls ) {
        // empty
    }
    
    virtual void setThreadId( const ThreadId &id ) {
        // empty
    }
    
    virtual ThreadId getThreadId() {
        static const ThreadId id;
        return id;
    }

protected:
    virtual i32 run() {
        return 0;
    }
};

class AbstractThreadTest : public ::testing::Test {
    // empty
};

TEST_F( AbstractThreadTest, ThreadIdTest ) {
    ThreadId tid1, tid2, tid3;
    tid1.Id = 1;
    tid2.Id = 2;
    tid3.Id = 1;
    EXPECT_NE( tid1, tid2 );
    EXPECT_EQ( tid1, tid3 );
}

TEST_F( AbstractThreadTest, createTest ) {
    bool ok( true );
    try {
        TestThread test_thread;
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( AbstractThreadTest, accessStateTest ) {
    TestThread test_thread;

    EXPECT_EQ( AbstractThread::ThreadState::New, test_thread.getCurrentState() );
}

} // Namespace UnitTest
} // Namespace OSRE
