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
