/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Platform/Windows/MinWindows.h>

namespace OSRE {
namespace Platform {

class Win32ThreadEvent;
class Win32ThreadLocalStorage;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
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
    virtual bool start(void *pData);
    ///	Stops the thread, must be started before.
    virtual bool stop();
    ///	The running thread will be suspended.
    virtual bool suspend();
    ///	The suspended thread will be resumed.
    virtual bool resume();
    ///	Assigns a new name to the thread.
    virtual void setName(const String &rName);
    ///	Returns the thread name.
    virtual const String &getName() const;
    ///	Awaits a timeout.
    virtual void waitForTimeout(ui32 ms);
    ///Awaits a signal.
    virtual void wait();
    ///	Returns a pointer showing to the assigned thread event.
    virtual AbstractThreadEvent *getThreadEvent() const;
    ///	Assigns a new thread priority to the thread.
    virtual void setPriority(Priority prio);
    ///	Returns the current thread priority.
    virtual Priority getPriority() const;
    ///	Returns the name of the thread.
    virtual const String &getThreadName() const;
    /// Returns the thread local storage instance.
    virtual AbstractThreadLocalStorage *getThreadLocalStorage();
    /// Assigns a new thread local storage instance.
    virtual void setThreadLocalStorage( AbstractThreadLocalStorage *tls );
    /// Will set the thread id.
    virtual void setThreadId(const ThreadId &id);
    /// Will return the thread id.
    virtual ThreadId getThreadId();

protected:
    ///	Override this for your own thread function.
    virtual i32 run();
    ///	The Win32-specific thread function.
    static ui32 WINAPI ThreadFunc( LPVOID data );
    ///	Will assign a thread name.
    static void setThreadName( const c8 *pName );

private:
    HANDLE                   m_ThreadHandle;
    Win32ThreadEvent        *m_pThreadSignal;
    Priority                 m_Prio;
    ThreadState              m_ThreadState;
    Win32ThreadLocalStorage *m_tls;
    String                   m_ThreadName;
    ThreadId                 m_id;
};

} // Namespace Platform
} // Namespace OSRE
