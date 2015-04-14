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
#include <osre2/Infrastructure/Threading/SystemTask.h>
#include <osre2/Infrastructure/Threading/TAsyncQueue.h>
#include <osre2/Infrastructure/Threading/TaskJob.h>
#include <osre2/Infrastructure/Debugging/ce_assert.h>
#include <osre2/Infrastructure/Common/AbstractEventHandler.h>
#include <osre2/Infrastructure/Common/Event.h>

#ifdef CE_WINDOWS
#   include <Code/Infrastructure/Platform/win32/Win32Thread.h>
#else
#   include <Code/Infrastructure/Platform/sdl2/SDL2Thread.h>
#endif

#include <sstream>

namespace ZFXCE2 {
namespace Threading {

using namespace ZFXCE2::Common;

//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Threading::TaskThread
///	@ingroup	Infrastructure
///
///	@brief
//-------------------------------------------------------------------------------------------------
#ifdef _WIN32
class SystemTaskThread : public Platform::Win32Thread {
#else
class SystemTaskThread : public Platform::SDL2Thread {
#endif

public:
	enum {
		StackSize = 4096
	};

public:
	//---------------------------------------------------------------------------------------------
#ifdef _WIN32
	SystemTaskThread( const ce_string &rThreadName, TAsyncQueue<const TaskJob*> *pJobQueue )
    : Win32Thread( rThreadName, StackSize )
#else
	SystemTaskThread( const ce_string &rThreadName, TAsyncQueue<const TaskJob*> *pJobQueue )
    : SDL2Thread( rThreadName, StackSize )
#endif
    , m_pUpdateEvent( nullptr )
    , m_pActiveJobQueue( pJobQueue )
    , m_pEventHandler( nullptr ) {
		ce_assert( nullptr != pJobQueue );

		Platform::AbstractThreadFactory *pThreadFactory( Platform::AbstractThreadFactory::getInstance() );
		if ( pThreadFactory ) {
			m_pUpdateEvent = pThreadFactory->createThreadEvent();
		} else {
			ce_error( "Invalid pointer to thread factory." );
		}
	}

	//---------------------------------------------------------------------------------------------
	~SystemTaskThread()	{
		if ( m_pEventHandler ) {
            m_pEventHandler->detach( nullptr );
        }

        delete m_pUpdateEvent;
		m_pUpdateEvent = nullptr;
	}

	//---------------------------------------------------------------------------------------------
	void setEventHandler( Common::AbstractEventHandler *pEventHandler ) {
		m_pEventHandler = pEventHandler;
        if ( m_pEventHandler ) {
            m_pEventHandler->attach( nullptr );
        }
	}

	//---------------------------------------------------------------------------------------------
	Common::AbstractEventHandler *getEventHandler() const {
		return m_pEventHandler;
	}

	//---------------------------------------------------------------------------------------------
	void setActiveJobQueue( Threading::TAsyncQueue<const TaskJob*> *pJobQueue ) {
		m_pActiveJobQueue = pJobQueue;
	}

	//---------------------------------------------------------------------------------------------
	Threading::TAsyncQueue<const TaskJob*> *getActiveJobQueue() const {
		return m_pActiveJobQueue;
	}

	//---------------------------------------------------------------------------------------------
	Platform::AbstractThreadEvent *getUpdateEvent() const {
		return m_pUpdateEvent;
	}

protected:
	//---------------------------------------------------------------------------------------------
	i32 run() {
        ce_debug( "SystemThread::run" );
		bool running = true;
		while ( running ) {
			m_pActiveJobQueue->awaitEnqueuedItem();
			while ( !m_pActiveJobQueue->isEmpty() )	{
                // for debugging
                //ui32 size = m_pActiveJobQueue->size();
                //std::stringstream stream;
                //stream << "queue size = " << size << std::endl;
                //ce_debug( stream.str() );

				const TaskJob *pJob = m_pActiveJobQueue->dequeue();
				const Common::Event *pEvent = pJob->getEvent();
				if ( !pEvent ) {
					running = false;
					ce_assert( nullptr != pEvent );
				}

				if ( m_pEventHandler ) {
					m_pEventHandler->onEvent( *pEvent, pJob->getEventData() );
				}

				if ( m_pUpdateEvent ) {
					m_pUpdateEvent->signal();
				}
			}
		}

		return 0;
	}

private:
	Platform::AbstractThreadEvent *m_pUpdateEvent;
	Threading::TAsyncQueue<const TaskJob*> *m_pActiveJobQueue;
	Common::AbstractEventHandler *m_pEventHandler;
};

//-------------------------------------------------------------------------------------------------
SystemTask::SystemTask( const ce_string &taskName )
: AbstractTask( taskName )
, m_WorkingMode( Async )
, m_buffermode( SingleBuffer )
, m_pTaskThread( nullptr )
, m_pAsyncQueue( nullptr ) {
	// empty
}

//-------------------------------------------------------------------------------------------------
SystemTask::~SystemTask() {
	ce_assert( !isRunning() );
}

//-------------------------------------------------------------------------------------------------
void SystemTask::setWorkingMode( AbstractTask::WorkingMode mode ) {
	if ( isRunning() ) {
		ce_error( "The working mode cannot be chanced in a running task." );
		return;
	}

	m_WorkingMode = mode;
}

//-------------------------------------------------------------------------------------------------
AbstractTask::WorkingMode SystemTask::getWorkingMode() const {
	return m_WorkingMode;
}

//-------------------------------------------------------------------------------------------------
void SystemTask::setBufferMode( BufferMode buffermode ) {
    m_buffermode = buffermode;
}

//-------------------------------------------------------------------------------------------------
SystemTask::BufferMode SystemTask::getBufferMode() const {
    return m_buffermode;
}

//-------------------------------------------------------------------------------------------------
bool SystemTask::start( Platform::AbstractThread *pThread ) {
	// ensure task is not running
    if( nullptr != m_pTaskThread ) {
		if ( Platform::AbstractThread::Running == m_pTaskThread->getCurrentState() ) {
			ce_debug( "Task " + getName() + " is already running." );
			return false;
		}
	}

	// setup the thread context
	m_pAsyncQueue = new Threading::TAsyncQueue<const TaskJob*>( Platform::AbstractThreadFactory::getInstance() );
	if ( !pThread ) {
        m_pTaskThread = new SystemTaskThread( getName() + ".thread", m_pAsyncQueue );
	} else {
        m_pTaskThread = reinterpret_cast<SystemTaskThread*>( pThread );
	}

	// start the system task
    return ( m_pTaskThread->start( nullptr ) );
}

//-------------------------------------------------------------------------------------------------
bool SystemTask::stop() {
	// Ensure task is still running
	if ( Platform::AbstractThread::Running != m_pTaskThread->getCurrentState() ) {
		ce_debug( "Task " + getName() + " is not running." );
		return false;
	}

	m_pTaskThread->stop();
	delete m_pTaskThread;
    m_pTaskThread = nullptr;

	return true;
}

//-------------------------------------------------------------------------------------------------
bool SystemTask::isRunning() const {
    if( nullptr != m_pTaskThread )
		return ( Platform::AbstractThread::Running == m_pTaskThread->getCurrentState() );

	return false;
}

//-------------------------------------------------------------------------------------------------
bool SystemTask::execute() {
	return true;
}

//-------------------------------------------------------------------------------------------------
void SystemTask::setThreadInstance( Platform::AbstractThread *pThreadInstance ) {
    if( Platform::AbstractThread::Running == m_pTaskThread->getCurrentState() ) {
        m_pTaskThread->stop();
    }

	m_pTaskThread = reinterpret_cast<SystemTaskThread*>( pThreadInstance );
}

//-------------------------------------------------------------------------------------------------
void SystemTask::attachEventHandler( Common::AbstractEventHandler *pEventHandler ) {
	ce_assert( nullptr != m_pTaskThread );

	m_pTaskThread->setEventHandler( pEventHandler );
}

//-------------------------------------------------------------------------------------------------
void SystemTask::detachEventHandler() {
    ce_assert( nullptr != m_pTaskThread );

	Common::AbstractEventHandler *pEH = m_pTaskThread->getEventHandler();
	if( pEH  ) {
		m_pTaskThread->setEventHandler( NULL );
	}
}

//-------------------------------------------------------------------------------------------------
bool SystemTask::sendEvent( const Common::Event *pEvent, const Common::EventData *pEventData ) {
    ce_assert( nullptr != m_pAsyncQueue );
    ce_assert( nullptr != pEvent );

	TaskJob *pTaskJob = new TaskJob( pEvent, pEventData );
	m_pAsyncQueue->enqueue( pTaskJob );

	return true;
}

//-------------------------------------------------------------------------------------------------
ui32 SystemTask::getEvetQueueSize() const {
	ce_assert( NULL != m_pAsyncQueue );

	return m_pAsyncQueue->size();
}

//-------------------------------------------------------------------------------------------------
void SystemTask::onUpdate() {
	ce_assert( NULL != m_pTaskThread );

	if ( !m_pTaskThread )
		return;

    if ( !m_pAsyncQueue ) {
        m_pAsyncQueue = m_pTaskThread->getActiveJobQueue();
    }
}

//-------------------------------------------------------------------------------------------------
void SystemTask::await() {
	if ( m_pTaskThread ) {
		Platform::AbstractThreadEvent *pThreadEvent = m_pTaskThread->getUpdateEvent();
		if ( pThreadEvent ) {
			pThreadEvent->wait();
		}
	}
}

//-------------------------------------------------------------------------------------------------
SystemTask *SystemTask::create( const ce_string &rTaskName ) {
	return new SystemTask( rTaskName );
}

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2
