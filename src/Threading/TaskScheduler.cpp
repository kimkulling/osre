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
#include <osre/Threading/TaskScheduler.h>
#include <osre/Threading/AsyncTask.h>
#include <osre/Threading/ThreadPool.h>

#include "WorkerThread.h"

#include <osre/Platform/PlatformInterface.h>
#include <cassert>

namespace OSRE {
namespace Threading {

TaskScheduler *TaskScheduler::s_pInstance = nullptr;

//-------------------------------------------------------------------------------------------------
TaskScheduler::TaskScheduler() 
: Object( "threading/taskscheduler" )
, m_pThreadPool( nullptr )
, m_AsyncTaskQueue() {
	// Create the thread pool
	m_pThreadPool = new ThreadPool( 2 );
}

//-------------------------------------------------------------------------------------------------
TaskScheduler::~TaskScheduler() {
	delete m_pThreadPool;
	m_pThreadPool = NULL;
}

//-------------------------------------------------------------------------------------------------
AsyncTask *TaskScheduler::createTask( const String &rTaskName ) {
	AsyncTask *pTask = NULL;

	return pTask;
}

//-------------------------------------------------------------------------------------------------
bool TaskScheduler::releaseTask( AsyncTask *pTask ) {
	assert( NULL != pTask );

	return false;
}

//-------------------------------------------------------------------------------------------------
AsyncTask *TaskScheduler::getTask( const String &rTaskName ) const {
	return NULL;
}

//-------------------------------------------------------------------------------------------------
bool TaskScheduler::dispatch( AsyncTask *pTask ) {
	assert( NULL != pTask );
	assert( NULL != m_pThreadPool );

	bool result = false;
	if ( m_pThreadPool->getNumSuspendedThreads() > 0 ) {
		result = run( pTask );
	} else {
		m_AsyncTaskQueue.enqueue( pTask );
		osre_debug( "Task " + pTask->getName() + " is enqueued." );
	}

	return result;
}

//-------------------------------------------------------------------------------------------------
void TaskScheduler::update() {
	if ( !m_AsyncTaskQueue.isEmpty() ) {
		AsyncTask *pTask( NULL );
		while ( m_pThreadPool->getNumSuspendedThreads() ) {
			if ( m_AsyncTaskQueue.dequeue( pTask ) ) {
				dispatch( pTask );
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------
TaskScheduler *TaskScheduler::create() {
	return new TaskScheduler();
}

//-------------------------------------------------------------------------------------------------
void TaskScheduler::setInstance( TaskScheduler *pInstance ) {
	if ( NULL != s_pInstance ) {
		s_pInstance->release();
	}

	s_pInstance = pInstance;
	if ( pInstance ) {
		pInstance->get();
	}
}

//-------------------------------------------------------------------------------------------------
TaskScheduler *TaskScheduler::getInstance() {
	return s_pInstance;
}

//-------------------------------------------------------------------------------------------------
void TaskScheduler::destroy() {
	if ( NULL != s_pInstance ) {
		s_pInstance->release();
		s_pInstance = NULL;
	}
}

//-------------------------------------------------------------------------------------------------
bool TaskScheduler::run( AsyncTask *pTask ) {
	bool result = true;
	WorkerThread *pThread = m_pThreadPool->getThread();
	if ( pThread ) {
		pThread->getTaskQueue()->enqueue( pTask );
	}

	return result;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2
