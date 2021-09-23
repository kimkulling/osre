/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/AbstractEventHandler.h>
#include <osre/Common/Event.h>
#include <osre/Common/EventBus.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::Common;

class EventBusTest : public ::testing::Test {
    // empty
};

DECL_EVENT(TestEvent1);
DECL_EVENT(TestEvent2);

class TestEventHandler : public AbstractEventHandler {
public:
    ui32 EventCount1 = 0;
    ui32 EventCount2 = 0;
    TestEventHandler() :
            AbstractEventHandler() {
        // empty
    }

    bool onEvent(const Event &ev, const EventData *) override {
        bool handled = false;
        if (ev == TestEvent1) {
            EventCount1++;
            handled = true;
        } else if (ev == TestEvent2) {
            EventCount2++;
            handled = true;
        }

        return handled;
    }

protected:
    bool onAttached(const EventData *) override {
        return true;
    }

    bool onDetached(const EventData *) override {
        return true;
    }
};

TEST_F(EventBusTest, createTest) {
    EventBus bus;
    EXPECT_FALSE(bus.isCreated());
    EXPECT_TRUE(bus.create());
    EXPECT_TRUE(bus.isCreated());
    EXPECT_TRUE(bus.destroy());
    EXPECT_FALSE(bus.isCreated());
    EXPECT_FALSE(bus.destroy());
}

TEST_F(EventBusTest, enqueueEventTest) {
    EventBus bus;
    bus.create();
    AbstractEventHandler *handler = new TestEventHandler;
    TestEventHandler *h = (TestEventHandler *)handler;
    EXPECT_EQ(0, h->EventCount1);
    EXPECT_EQ(0, h->EventCount1);

    bus.suscribeEventHandler(handler, TestEvent1);
    bus.suscribeEventHandler(handler, TestEvent2);
    bus.enqueueEvent(TestEvent1, nullptr);
    bus.enqueueEvent(TestEvent2, nullptr);
    bus.update();
    EXPECT_EQ(1, h->EventCount1);
    EXPECT_EQ(1, h->EventCount1);
    bus.destroy();
    delete handler;
}

} // namespace UnitTest
} // namespace OSRE
