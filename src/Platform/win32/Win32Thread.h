#pragma once

#include <osre/Platform/AbstractThread.h>

#include <Windows.h>

namespace OSRE {
namespace Platform {

class Win32ThreadEvent;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Threading::Win32Thread
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
} // Namespace ZFXCE2
