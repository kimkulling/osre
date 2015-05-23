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
#pragma once

#include <osre/Platform/AbstractThread.h>

#include <Windows.h>

namespace OSRE {
namespace Platform {

class Win32ThreadEvent;

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Threading::Win32Thread
///	@ingroup	Infrastructure
///
///	@brief	This class implements a thread which uses the Win32-API.
//-------------------------------------------------------------------------------------------------
class Win32Thread : public AbstractThread {
public:
    ///	The class constructor.
    Win32Thread( const String &threadName, ui32 stacksize );
    ///	The class destructor.
    virtual ~Win32Thread();
    ///	Starts the thread.
    bool start(  void *pData );
    ///	Stops the thread, must be started before.
    bool stop();
    ///	Returns the current state of the thread.
    ThreadState getCurrentState() const;
    ///	The running thread will be suspended.
    bool suspend();
    ///	The suspended thread will be resumed.
    bool resume();
    ///	Assigns a new name to the thread.
    void setName( const String &rName );
    ///	Returns the thread name.
    const String &getName() const;
    ///	Sets a new stack size.
    void setStackSize( ui32 stacksize );
    ///	Returns the current thread size.
    ui32 getStackSize() const;
    ///	Awaits a timeout.
    void waitForTimeout( ui32 ms );
    ///Awaits a signal.
    void wait();
    ///	Returns a pointer showing to the assigned thread event.
    AbstractThreadEvent *getThreadEvent() const;
    ///	Assigns a new thread priority to the thread.
    void setPriority( Priority prio );
    ///	Returns the current thread priority.
    Priority getPriority() const;
    ///	Returns the name of the thread.
    const String &getThreadName() const;

protected:
    ///	Override this for your own thread function.
    virtual i32 run();
    ///	Set the new state of the thread.
    void setState( ThreadState newState );
    ///	The Win32-specific thread function.
    static ui32 WINAPI ThreadFunc( LPVOID data );
    ///	Will assign a thread name.
    static void setThreadName( const c8 *pName );

private:
    HANDLE m_ThreadHandle;
    Win32ThreadEvent *m_pThreadSignal;
    Priority m_Prio;
    ThreadState m_ThreadState;
    String m_ThreadName;
    ui32 m_StackSize;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace OSRE
