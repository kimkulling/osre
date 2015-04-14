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
--------------------------------------------------------------------------------------------------*/
#include "WorkerThread.h"

#include <osre/Threading/AsyncTask.h>
#include <osre/Platform/PlatformInterface.h>

namespace OSRE {
namespace Threading {

using namespace ZFXCE2::Platform;

//-------------------------------------------------------------------------------------------------
WorkerThread::WorkerThread( const ce_string &name, ui32 stacksize ) 
: m_pFinishEvent( nullptr )
, m_pTaskQueue( nullptr ) {
    Platform::AbstractThreadFactory *pThreadFactory( Platform::AbstractThreadFactory::getInstance() );
    if ( pThreadFactory ) {
        m_pTaskQueue = new Threading::TAsyncQueue<AsyncTask*>( pThreadFactory );
        m_pFinishEvent = pThreadFactory->createThreadEvent();
        m_pImpl = pThreadFactory->createThread( name, stacksize );
    } else {
        ce_error( "Invalid pointer to the thread factory." );
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
    ce_assert( nullptr != m_pImpl );

    return m_pImpl->start( pData );
}

//-------------------------------------------------------------------------------------------------
bool WorkerThread::stop() {
    ce_assert( nullptr != m_pImpl );

    return m_pImpl->stop();
}

//-------------------------------------------------------------------------------------------------
Platform::AbstractThread::ThreadState WorkerThread::getCurrentState( ) const {
    ce_assert( nullptr != m_pImpl );

    return m_pImpl->getCurrentState();
}

//-------------------------------------------------------------------------------------------------
void WorkerThread::suspend() {
    ce_assert( nullptr != m_pImpl );

    m_pImpl->suspend();
}

//-------------------------------------------------------------------------------------------------
const ce_string & WorkerThread::getName() const {
    ce_assert( nullptr != m_pImpl );

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
