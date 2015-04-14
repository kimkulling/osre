/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-2015, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions
and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
and the following disclaimer in the documentation and/or other materials provided with the
distribution.

* Neither the name of the ZFXCE2 team, nor the names of its contributors may be used to endorse or
promote products derived from this software without specific prior written permission of the
ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
#include "Win32Thread.h"
#include "Win32ThreadEvent.h"
#include "Win32CriticalSection.h"

#include <osre2/Infrastructure/Debugging/ce_assert.h>
#include <osre2/Infrastructure/System/SystemInfo.h>
#include <osre2/Infrastructure/Common/Logger.h>

#include <windows.h>
#include <process.h>

namespace ZFXCE2 {
namespace Platform {

//-------------------------------------------------------------------------------------------------
Win32Thread::Win32Thread(  const ce_string &name, ui32 stacksize  ) 
: m_ThreadHandle( NULL )
, m_pThreadSignal( NULL )
, m_ThreadState( New )
, m_Prio( Normal )
, m_ThreadName( name )
, m_StackSize( stacksize ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
Win32Thread::~Win32Thread() {
    if ( Running == m_ThreadState ) {
        ce_debug( "Thread " + getName() + " is still running." );
        Win32Thread::stop();
    }
}

//-------------------------------------------------------------------------------------------------
bool Win32Thread::start( void *pData ) {
    if ( Running == m_ThreadState || m_ThreadHandle ) {
        ce_debug( "Thread " + getName() + " is already running." );
        return false;
    }

    if ( !pData ) {
        pData = this;
    }

    m_pThreadSignal = new Win32ThreadEvent;
    m_ThreadHandle = (HANDLE) _beginthreadex( NULL,
        m_StackSize,
        Win32Thread::ThreadFunc,
        pData,
        NULL,
        NULL );

    ce_assert( NULL != m_ThreadHandle );
    setPriority( Normal );
    resume();
    setState( Running );

    return true;
}

//-------------------------------------------------------------------------------------------------
bool Win32Thread::stop() {
    if ( !Running == m_ThreadState ) {
        ce_debug( "Thread " + getName() + " is not running." );
        return false;
    }

    // Stop the thread
    if ( m_ThreadHandle != NULL ) {
        ::TerminateThread( m_ThreadHandle, 0 );
        ::CloseHandle( m_ThreadHandle );
        m_ThreadHandle = NULL;
    }

    // update state and release signal
    setState( Terminated );
    delete m_pThreadSignal;
    m_pThreadSignal = NULL;

    System::SystemInfo::ThreadId id;
    id.Id = ::GetCurrentThreadId();
    System::SystemInfo::unregisterThreadName( id );

    return true;
}

//-------------------------------------------------------------------------------------------------
Win32Thread::ThreadState Win32Thread::getCurrentState() const {
    return m_ThreadState;
}

//-------------------------------------------------------------------------------------------------
bool Win32Thread::suspend() {
    // check for a valid thread state
    if ( !m_ThreadHandle || Running == m_ThreadState ) {
        ce_debug( "Thread " + getName() + " is not running." );
        return false;
    }

    // suspend the thread
    DWORD retCode = ::SuspendThread( m_ThreadHandle );
    if ( -1 == retCode )
        return false;

    setState( Suspended );

    return true;
}

//-------------------------------------------------------------------------------------------------
bool Win32Thread::resume() {
    // check for a valid thread state
    if ( !m_ThreadHandle || Waiting != m_ThreadState || New != m_ThreadState ) {
        ce_debug( "Thread " + getName() + " is not suspended." );
        return false;
    }

    // resume the thread
    DWORD retCode = ::ResumeThread( m_ThreadHandle );
    if( -1 == retCode ) {
        ce_error( "Error while try to resume thread." );
        return false;
    }

    setState( Running );

    return true;
}

//-------------------------------------------------------------------------------------------------
void Win32Thread::setName( const ce_string &rName ) {
    ce_assert( !rName.empty() );

    m_ThreadName = rName;
}

//-------------------------------------------------------------------------------------------------
const ce_string &Win32Thread::getName() const {
    return m_ThreadName;
}

//-------------------------------------------------------------------------------------------------
void Win32Thread::setStackSize( ui32 stacksize ) {
    m_StackSize = stacksize;
}

//-------------------------------------------------------------------------------------------------
ui32 Win32Thread::getStackSize() const {
    return m_StackSize;
}

//-------------------------------------------------------------------------------------------------
void Win32Thread::waitForTimeout( ui32 ms ) {
    if ( !m_pThreadSignal ) {
        ce_debug( "Invalid pointer to thread signal." );
        return;
    } else {
        m_pThreadSignal->waitForTimeout( ms );
    }
}

//-------------------------------------------------------------------------------------------------
void Win32Thread::wait() {
    if ( !m_pThreadSignal ) {
        ce_debug( "Invalid pointer to thread signal." );
        return;
    } else {
        m_pThreadSignal->wait();
    }
}

//-------------------------------------------------------------------------------------------------
AbstractThreadEvent *Win32Thread::getThreadEvent() const {
    return m_pThreadSignal;
}

//-------------------------------------------------------------------------------------------------
void Win32Thread::setPriority( Priority prio ) {
    m_Prio = prio;
    BOOL result( TRUE );
    switch( m_Prio ) {
        case Low:
            result = ::SetThreadPriority( m_ThreadHandle, THREAD_PRIORITY_BELOW_NORMAL );
            break;

        case Normal:
            result = ::SetThreadPriority( m_ThreadHandle, THREAD_PRIORITY_NORMAL );
            break;

        case High:
            result = ::SetThreadPriority( m_ThreadHandle, THREAD_PRIORITY_ABOVE_NORMAL );
            break;

        default:
            break;
    }

    // log errors
    if( !result ) {
        ce_error( "Error while setting thread prio." );
    }
}

//-------------------------------------------------------------------------------------------------
Win32Thread::Priority Win32Thread::getPriority() const {
    return m_Prio;
}

//-------------------------------------------------------------------------------------------------
const ce_string &Win32Thread::getThreadName() const {
    return m_ThreadName;
}

//-------------------------------------------------------------------------------------------------
ui32 WINAPI Win32Thread::ThreadFunc( LPVOID data ) {
    Win32Thread *pThread = ( Win32Thread* ) data;
    ce_assert( NULL != pThread );

    setThreadName( pThread->getName().c_str() );
    System::SystemInfo::ThreadId id;
    id.Id = ::GetCurrentThreadId();
    System::SystemInfo::registerThreadName( id, pThread->getName() );

    pThread->run();

    return 0;
}

//-------------------------------------------------------------------------------------------------
i32 Win32Thread::run() {
    // Override me!

    return 0;
}
//-------------------------------------------------------------------------------------------------
void Win32Thread::setState( ThreadState newState ) {
    m_ThreadState = newState;
}

//-------------------------------------------------------------------------------------------------
void Win32Thread::setThreadName( const c8 *pName ) {
    if ( !pName )
        return;

    struct THREADNAME_INFO {
        DWORD dwType;     // must be 0x1000
        LPCSTR szName;    // pointer to name (in user address space)
        DWORD dwThreadID; // thread ID (-1 = caller thread)
        DWORD dwFlags;    // reserved for future use, must be zero
    };

    THREADNAME_INFO info;
    info.dwType     = 0x1000;
    info.szName     = pName;
    info.dwThreadID = ::GetCurrentThreadId();
    info.dwFlags    = 0;
    __try {
        RaiseException( 0x406D1388, 0, sizeof( info ) / sizeof( DWORD ), (DWORD*)&info );
    } __except( EXCEPTION_CONTINUE_EXECUTION ) {
        // Nothing to do!
    }
}

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2
