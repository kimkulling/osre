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
#include <osre2/Infrastructure/Threading/AsyncTask.h>
#include <osre2/Infrastructure/Platform/AbstractThread.h>
#include <osre2/Infrastructure/Platform/AbstractThreadEvent.h>

#include "WorkerThread.h"

namespace ZFXCE2 {
namespace Threading {

using namespace ZFXCE2::Platform;

//-------------------------------------------------------------------------------------------------
AsyncTask::AsyncTask( const ce_string &name )
: AbstractTask( name )
, m_WorkingMode( Async )
, m_buffermode( SingleBuffer )
, m_IsRunning( false )
, m_pThreadInstance( nullptr )
, m_pTaskQueue( nullptr )
, m_pFinishedEvent( nullptr ) {
	// empty
}

//-------------------------------------------------------------------------------------------------
AsyncTask::~AsyncTask() {
	m_pThreadInstance = nullptr;
	m_pFinishedEvent = nullptr;
	m_pTaskQueue = nullptr;
}

//-------------------------------------------------------------------------------------------------
void AsyncTask::setWorkingMode( WorkingMode mode ) {
	m_WorkingMode = mode;
}

//-------------------------------------------------------------------------------------------------
AsyncTask::WorkingMode AsyncTask::getWorkingMode() const {
	return m_WorkingMode;
}

//-------------------------------------------------------------------------------------------------
void AsyncTask::setBufferMode( BufferMode buffermode ) {
    m_buffermode = buffermode;
}

//-------------------------------------------------------------------------------------------------
AsyncTask::BufferMode AsyncTask::getBufferMode() const {
    return m_buffermode;
}

//-------------------------------------------------------------------------------------------------
void AsyncTask::setThreadInstance( AbstractThread *pThreadInstance ) {
	if ( m_pThreadInstance ) {
		if ( AbstractThread::Running == m_pThreadInstance->getCurrentState() ) {
			m_pThreadInstance->suspend();
		}
	}

	if ( pThreadInstance ) {
		m_pThreadInstance = (WorkerThread*) pThreadInstance;
		m_pTaskQueue = m_pThreadInstance->getTaskQueue();
		m_pFinishedEvent = m_pThreadInstance->getFinishEvent();
		ce_assert( nullptr != m_pTaskQueue );
		ce_assert( nullptr != m_pFinishedEvent );
	}
}

//-------------------------------------------------------------------------------------------------
bool AsyncTask::preExecute() {
	// Override me!

	return AbstractTask::preExecute();
}

//-------------------------------------------------------------------------------------------------
bool AsyncTask::execute() {
	// Override me!

	return true;
}

//-------------------------------------------------------------------------------------------------
bool AsyncTask::postExecute() {
	// Override me!

	return AbstractTask::postExecute();
}

//-------------------------------------------------------------------------------------------------
void AsyncTask::onUpdate() {

}

//-------------------------------------------------------------------------------------------------
void AsyncTask::await() {
	if ( m_pFinishedEvent ) {
		m_pFinishedEvent->signal();
	}
}

//-------------------------------------------------------------------------------------------------
AsyncTask *AsyncTask::create( const ce_string &taskName ) {
	return new AsyncTask( taskName );
}

//-------------------------------------------------------------------------------------------------
EmptyTask::EmptyTask()
: AsyncTask( "threading/emptytask" ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
EmptyTask::~EmptyTask() {
    // empty
}

//-------------------------------------------------------------------------------------------------
bool EmptyTask::execute() {
    return true;
}

//-------------------------------------------------------------------------------------------------
EmptyTask *EmptyTask::create() {
    return new EmptyTask;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2

