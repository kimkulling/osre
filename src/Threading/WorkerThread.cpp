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
#include "WorkerThread.h"

#include <osre/Threading/AsyncTask.h>
#include <osre/Platform/PlatformInterface.h>

namespace OSRE {
namespace Threading {

using namespace ::OSRE::Platform;

//-------------------------------------------------------------------------------------------------
WorkerThread::WorkerThread( const String &name, ui32 stacksize ) 
: m_pFinishEvent( nullptr )
, m_pTaskQueue( nullptr ) {
    Platform::AbstractThreadFactory *pThreadFactory( Platform::AbstractThreadFactory::getInstance() );
    if ( pThreadFactory ) {
        m_pTaskQueue = new Threading::TAsyncQueue<AsyncTask*>( pThreadFactory );
        m_pFinishEvent = pThreadFactory->createThreadEvent();
        m_pImpl = pThreadFactory->createThread( name, stacksize );
    } else {
        osre_error( "Invalid pointer to the thread factory." );
    }
}

//-------------------------------------------------------------------------------------------------
WorkerThread::~WorkerThread() {
    delete m_pImpl;
    m_pImpl = nullptr;

    delete m_pFinishEvent;
    m_pFinishEvent = nullptr;

    delete m_pTaskQueue;
    m_pTaskQueue = nullptr;
}

//-------------------------------------------------------------------------------------------------
Platform::AbstractThreadEvent *WorkerThread::getFinishEvent( ) const {
    return m_pFinishEvent;
}

//-------------------------------------------------------------------------------------------------
Threading::TAsyncQueue<AsyncTask*> *WorkerThread::getTaskQueue() const {
    return m_pTaskQueue;
}

//-------------------------------------------------------------------------------------------------
bool WorkerThread::start( void *pData ) {
    assert( nullptr != m_pImpl );

    return m_pImpl->start( pData );
}

//-------------------------------------------------------------------------------------------------
bool WorkerThread::stop() {
    assert( nullptr != m_pImpl );

    return m_pImpl->stop();
}

//-------------------------------------------------------------------------------------------------
Platform::AbstractThread::ThreadState WorkerThread::getCurrentState( ) const {
    assert( nullptr != m_pImpl );

    return m_pImpl->getCurrentState();
}

//-------------------------------------------------------------------------------------------------
void WorkerThread::suspend() {
    assert( nullptr != m_pImpl );

    m_pImpl->suspend();
}

//-------------------------------------------------------------------------------------------------
const String & WorkerThread::getName() const {
    assert( nullptr != m_pImpl );

    return m_pImpl->getName();
}

//-------------------------------------------------------------------------------------------------
i32 WorkerThread::run() {
    i32 retCode( 0 );
    for ( ;; ) {
        bool ended( false  );
        m_pTaskQueue->awaitEnqueuedItem();
        while ( !m_pTaskQueue->isEmpty() )	{
            // get the next task from the task queue
            AsyncTask *pNextTask = m_pTaskQueue->dequeue();
            if ( pNextTask ) {
                pNextTask->preExecute();
                pNextTask->execute();
                pNextTask->postExecute();
            } else {
                break;
            }
            
            // signals the finished job
            if ( m_pFinishEvent ) {
                m_pFinishEvent->signal();
                ended = true;
            }
        }
        if( ended ) {
            break;
        }
    }

    return retCode;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2
