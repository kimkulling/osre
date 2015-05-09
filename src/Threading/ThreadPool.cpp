#include <osre/Threading/ThreadPool.h>
#include <osre/Common/Logger.h>
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
            osre_debug( "Thread " + pThread->getName() + " is already running." );
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
} // Namespace ZFXCE2
