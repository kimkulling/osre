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
#include <osre/Threading/ThreadPool.h>
#include <osre/Common/Logger.h>
#include "WorkerThread.h"

namespace OSRE {
namespace Threading {

static const String Tag = "ThreadPool";

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
    static T *alloc( const String &rThreadName, ui32 stacksize ) {
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
            String threadname( "workerthread_" );
#ifndef _WIN32
            sprintf( buffer, "%d", i );
#else
            sprintf_s( buffer, 10, "%d", i );
#endif
            threadname += buffer;
            pThread = ThreadAllocator::alloc( threadname, 4096 );
            assert( nullptr != pThread );
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
            osre_debug( Tag, "Thread " + pThread->getName() + " is already running." );
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
    assert( nullptr != pThread );

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
} // Namespace OSRE
