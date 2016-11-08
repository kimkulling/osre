/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Win32Thread.h"
#include "Win32ThreadEvent.h"
#include "Win32CriticalSection.h"
#include "Win32ThreadLocalStorage.h"

#include <osre/Platform/SystemInfo.h>
#include <osre/Common/Logger.h>

#include <windows.h>
#include <process.h>
#include <cassert>

namespace OSRE {
namespace Platform {

static const String Tag = "Win32Thread";

Win32Thread::Win32Thread(  const String &name, ui32 stacksize  ) 
: AbstractThread()
, m_ThreadHandle( NULL )
, m_pThreadSignal( NULL )
, m_ThreadState( ThreadState::New )
, m_Prio( Priority::Normal )
, m_ThreadName( name )
, m_tls( nullptr ) {
    // empty
}

Win32Thread::~Win32Thread() {
    if ( ThreadState::Running == m_ThreadState ) {
        osre_debug( Tag, "Thread " + getName() + " is still running." );
        Win32Thread::stop();
    }
}

bool Win32Thread::start( void *pData ) {
    if ( ThreadState::Running == m_ThreadState || m_ThreadHandle ) {
        osre_debug( Tag, "Thread " + getName() + " is already running." );
        return false;
    }

    if ( !pData ) {
        pData = this;
    }

    m_pThreadSignal = new Win32ThreadEvent;
    m_ThreadHandle = (HANDLE) _beginthreadex( NULL,
        1024*1024,
        Win32Thread::ThreadFunc,
        pData,
        NULL,
        NULL );

    assert( NULL != m_ThreadHandle );
    setPriority( Priority::Normal );
    resume();
    setState( ThreadState::Running );

    return true;
}

bool Win32Thread::stop() {
    if ( ThreadState::Running != m_ThreadState ) {
        osre_debug( Tag, "Thread " + getName() + " is not running." );
        return false;
    }

    // Stop the thread
    //::TerminateThread(m_ThreadHandle, 0);
    if (m_ThreadHandle != NULL) {
        ::CloseHandle( m_ThreadHandle );
        m_ThreadHandle = NULL;
    }

    // update state and release signal
    setState( ThreadState::Terminated );
    delete m_pThreadSignal;
    m_pThreadSignal = NULL;

    SystemInfo::unregisterThreadName( m_id );

    return true;
}

bool Win32Thread::suspend() {
    // check for a valid thread state
    if ( !m_ThreadHandle || ThreadState::Running == m_ThreadState ) {
        osre_debug( Tag, "Thread " + getName() + " is not running." );
        return false;
    }

    // suspend the thread
    DWORD retCode = ::SuspendThread( m_ThreadHandle );
    if ( -1 == retCode )
        return false;

    setState( ThreadState::Suspended );

    return true;
}

bool Win32Thread::resume() {
    // check for a valid thread state
    if ( !m_ThreadHandle || ThreadState::Waiting != m_ThreadState || ThreadState::New != m_ThreadState ) {
        osre_debug( Tag, "Thread " + getName() + " is not suspended." );
        return false;
    }

    // resume the thread
    DWORD retCode = ::ResumeThread( m_ThreadHandle );
    if( -1 == retCode ) {
        osre_error( Tag, "Error while try to resume thread." );
        return false;
    }

    setState( ThreadState::Running );

    return true;
}

void Win32Thread::setName( const String &rName ) {
    assert( !rName.empty() );

    m_ThreadName = rName;
}

const String &Win32Thread::getName() const {
    return m_ThreadName;
}

void Win32Thread::waitForTimeout( ui32 ms ) {
    if ( !m_pThreadSignal ) {
        osre_debug( Tag, "Invalid pointer to thread signal." );
        return;
    } else {
        m_pThreadSignal->waitForTimeout( ms );
    }
}

void Win32Thread::wait() {
    if ( !m_pThreadSignal ) {
        osre_debug( Tag, "Invalid pointer to thread signal." );
        return;
    } 
    m_pThreadSignal->waitForOne();
}

AbstractThreadEvent *Win32Thread::getThreadEvent() const {
    return m_pThreadSignal;
}

void Win32Thread::setPriority( Priority prio ) {
    m_Prio = prio;
    BOOL result( TRUE );
    switch( m_Prio ) {
        case Priority::Low:
            result = ::SetThreadPriority( m_ThreadHandle, THREAD_PRIORITY_BELOW_NORMAL );
            break;

        case Priority::Normal:
            result = ::SetThreadPriority( m_ThreadHandle, THREAD_PRIORITY_NORMAL );
            break;

        case Priority::High:
            result = ::SetThreadPriority( m_ThreadHandle, THREAD_PRIORITY_ABOVE_NORMAL );
            break;

        default:
            break;
    }

    if( !result ) {
        osre_error( Tag, "Error while setting thread prio." );
    }
}

Win32Thread::Priority Win32Thread::getPriority() const {
    return m_Prio;
}

const String &Win32Thread::getThreadName() const {
    return m_ThreadName;
}

AbstractThreadLocalStorage *Win32Thread::getThreadLocalStorage() {
    return m_tls;
}

void Win32Thread::setThreadLocalStorage( AbstractThreadLocalStorage *tls ) {
    m_tls = ( Win32ThreadLocalStorage* )  tls;
}

ui32 WINAPI Win32Thread::ThreadFunc( LPVOID data ) {
    Win32Thread *thread = ( Win32Thread* ) data;
    if ( NULL == thread ) {
        return 1;
    }

    setThreadName( thread->getName().c_str() );
    ThreadId id = thread->getThreadId();
    id.Id = ::GetCurrentThreadId();
    SystemInfo::registerThreadName( id, thread->getName() );
    thread->setThreadId( id );
    const i32 retCode( thread->run() );

    return retCode;
}

void Win32Thread::setThreadId(const ThreadId &id) {
    m_id = id;
}

ThreadId Win32Thread::getThreadId() {
    return m_id;
}

i32 Win32Thread::run() {
    // Override me!

    return 0;
}

void Win32Thread::setThreadName( const c8 *name ) {
    if ( nullptr == name ) {
        return;
    }

    struct THREADNAME_INFO {
        DWORD dwType;     // must be 0x1000
        LPCSTR szName;    // pointer to name (in user address space)
        DWORD dwThreadID; // thread ID (-1 = caller thread)
        DWORD dwFlags;    // reserved for future use, must be zero
    };

    THREADNAME_INFO info;
    info.dwType     = 0x1000;
    info.szName     = name;
    info.dwThreadID = ::GetCurrentThreadId();
    info.dwFlags    = 0;
    __try {
        RaiseException( 0x406D1388, 0, sizeof( info ) / sizeof( DWORD ), (DWORD*)&info );
    } __except( EXCEPTION_CONTINUE_EXECUTION ) {
        // Nothing to do!
    }
}


} // Namespace Threading
} // Namespace OSRE
