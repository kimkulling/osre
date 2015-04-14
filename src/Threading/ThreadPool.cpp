/*
ZFX Community Engine 2  (ZFXCE2)
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
---------------------------------------------------------------------------------------------------
*/
#include <osre2/Threading/ThreadPool.h>
#include <osre2/Common/Logger.h>
#include "WorkerThread.h"

namespace OSRE {
namespace Threading {

//-------------------------------------------------------------------------------------------------
///	The thread allocator template class, will offer the platform specific allocation.
//-------------------------------------------------------------------------------------------------
template<class T>
class TThreadAllocator {
public:
    ///	The constructor.
    TThreadAllocator() {
        // empty
    }
    /// The destructor.
    ~TThreadAllocator()	{
        // empty
    }
    ///	Allocates the thread
    static T *alloc( const ce_string &rThreadName, ui32 stacksize ) {
        return new T( rThreadName, stacksize );
    }
};

typedef TThreadAllocator<WorkerThread> ThreadAllocator;

//-------------------------------------------------------------------------------------------------
ThreadPool::ThreadPool( ui32 numThreads )
: Object( "threading/threadpool" )
, m_ThreadArray()
, m_SuspendedThreadArray() {
    c8 buffer[ 10 ];
    WorkerThread *pThread( nullptr );
    if ( numThreads > 0 ) {
        for ( ui32 i=0; i<numThreads; ++i ){
            ce_string threadname( "workerthread_" );
#ifndef _WIN32
            sprintf( buffer, "%d", i );
#else
            sprintf_s( buffer, 10, "%d", i );
#endif
            threadname += buffer;
            pThread = ThreadAllocator::alloc( threadname, 4096 );
            ce_assert( nullptr != pThread );
            m_ThreadArray.add( pThread );
            m_SuspendedThreadArray.add( pThread );
        }
    }
}

//-------------------------------------------------------------------------------------------------
ThreadPool::~ThreadPool() {
    WorkerThread *pThread( nullptr );
    for ( ui32 i=0; i<m_ThreadArray.size(); ++i ) {
        pThread = m_ThreadArray[ i ];
        if( Platform::AbstractThread::Running == pThread->getCurrentState( ) ) {
            ce_debug( "Thread " + pThread->getName() + " is already running." );
            pThread->stop();
        }
        delete pThread;
    }
}

//-------------------------------------------------------------------------------------------------
ui32 ThreadPool::getNumSuspendedThreads() const {
    return m_SuspendedThreadArray.size();
}

//-------------------------------------------------------------------------------------------------
ui32 ThreadPool::getPoolSize() const {
    return m_ThreadArray.size();
}

//-------------------------------------------------------------------------------------------------
WorkerThread *ThreadPool::getThread() {
    if ( m_SuspendedThreadArray.isEmpty() ) {
        return nullptr;
    }

    WorkerThread *pThread = m_SuspendedThreadArray.back();
    m_SuspendedThreadArray.removeBack();

    return pThread;
}

//-------------------------------------------------------------------------------------------------
void ThreadPool::releaseThread( WorkerThread *pThread ) {
    ce_assert( nullptr != pThread );

    bool found = false;
    for ( ui32 i=0; i<m_ThreadArray.size(); ++i ) {
        if ( m_ThreadArray[ i ] == pThread ) {
            found = true;
            break;
        }
    }

    if ( found ) {
        if( Platform::AbstractThread::Running == pThread->getCurrentState() ) {
            pThread->suspend();
        }
        m_SuspendedThreadArray.add( pThread );
    }
}

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2
