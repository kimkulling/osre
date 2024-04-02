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
#include <osre/App/TAbstractCtrlBase.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::App;

enum class TestState {
    United,
    Inited
};

class TestCtrl : public TAbstractCtrlBase<TestState> {
public:
    TestCtrl() : TAbstractCtrlBase<TestState>( TestState::Inited ), m_entered( false ), m_handled( false ), m_leave( false ), m_lastTimetick(0), m_enteredNewState( TestState::United ) {
        // empty
    }

    ~TestCtrl() {
        // empty
    }

    void clear() {
        m_entered = false;
        m_handled = false;
        m_leave = false;
    }

protected:
    virtual bool onStateEnter( TestState newState ) {
        m_enteredNewState = newState;
        m_entered = true;

        return TAbstractCtrlBase<TestState>::onStateEnter( newState );
    }

    virtual bool onState() {
        m_handled = true;
        
        return m_handled;
    }

    virtual bool onUpdate( d32 timetick ) {
        m_lastTimetick = timetick;
        return onUpdate( timetick );
    }

    virtual bool onStateLeave( TestState oldState ) {
        m_leave = true;

        return TAbstractCtrlBase<TestState>::onStateLeave( oldState );
    }

public:
    bool m_entered, m_handled, m_leave;
    d32 m_lastTimetick;
    TestState m_enteredNewState;
};

class TestCtrlListener : public TAbstractCtrlStateListener<TestState> {
public:
    TestCtrlListener( TestCtrl *ctrl )
    : TAbstractCtrlStateListener<TestState>( ctrl )
    , m_notified( false )
    , m_newState( TestState::United ) {
        // empty
    }

    virtual ~TestCtrlListener() {
        // empty
    }

    void clear() {
        m_notified = false;
    }

    virtual void onStateChanged( TestState newState ) {
        m_notified = true;
        m_newState = newState;
    }

    bool m_notified;
    TestState m_newState;
};

class TAbstractCtrlBaseTest : public ::testing::Test {
    // empty
};


TEST_F( TAbstractCtrlBaseTest, createTest ) {
    bool ok( true );
    try {
        TestCtrl ctrl;
    } catch( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( TAbstractCtrlBaseTest, gotoStateTest ) {
    TestCtrl ctrl;
    bool res( false );
    res = ctrl.gotoState( TestState::Inited );
    EXPECT_TRUE( res );

    EXPECT_EQ( TestState::Inited, ctrl.getState() );
    EXPECT_TRUE( ctrl.m_entered );
    EXPECT_TRUE( ctrl.m_handled );
    EXPECT_TRUE( ctrl.m_leave );
}

TEST_F( TAbstractCtrlBaseTest, notifyListenerTest ) {
    TestCtrl ctrl;
    TestCtrlListener *listener = new TestCtrlListener( &ctrl );
    EXPECT_FALSE( listener->m_notified );
    bool res( false );
    res = ctrl.registerListener( listener );
    EXPECT_TRUE( res );

    res = ctrl.gotoState( TestState::Inited );
    EXPECT_TRUE( res );
    EXPECT_TRUE( listener->m_notified );

    res = ctrl.unregisterListener( listener );
    EXPECT_TRUE( res );

    ctrl.clear();
    listener->clear();
    res = ctrl.gotoState( TestState::Inited );
    EXPECT_FALSE( listener->m_notified );
    delete listener;
}

}
}
