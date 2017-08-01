/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Threading/SystemTask.h>
#include <osre/Threading/TAsyncQueue.h>
#include <osre/Threading/TaskJob.h>
#include <osre/Platform/AtomicInt.h>
#include <osre/Common/AbstractEventHandler.h>
#include <osre/Common/Event.h>
#include <osre/Debugging/osre_debugging.h>

#ifdef OSRE_WINDOWS
#   include <src/Engine/Platform/win32/Win32Thread.h>
#else
#   include <src/Engine/Platform/sdl2/SDL2Thread.h>
#endif

#include <sstream>

namespace OSRE {
namespace Threading {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;

TaskJob *TaskJobAlloc( const Event *ev, const EventData *eventData ) {
    return new TaskJob( ev, eventData );
}

DECL_EVENT( OnStopSystemTaskEvent );

struct OSRE_EXPORT StopSystemTaskEventData : public Common::EventData {
public:
    StopSystemTaskEventData()
    : EventData( OnStopSystemTaskEvent, nullptr ) {
        // empty
    }
};

static const String Tag = "SystemTaskThread";

static bool DebugQueueSize = false;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
#ifdef OSRE_WINDOWS
class SystemTaskThread : public Win32Thread {
#else
class SystemTaskThread : public SDL2Thread {
#endif

public:
    enum {
        StackSize = 4096
    };

public:
#ifdef OSRE_WINDOWS
    SystemTaskThread( const String &threadName, TAsyncQueue<const TaskJob*> *jobQueue )
    : Win32Thread( threadName, StackSize )
#else
    SystemTaskThread( const String &threadName, TAsyncQueue<const TaskJob*> *jobQueue )
    : SDL2Thread( threadName, StackSize )
#endif
    , m_updateEvent( nullptr )
    , m_stopEvent( nullptr )
    , m_activeJobQueue( jobQueue )
    , m_eventHandler( nullptr ) {
        OSRE_ASSERT(nullptr != jobQueue);

        AbstractThreadFactory *threadFactory( AbstractThreadFactory::getInstance() );
        if ( threadFactory ) {
            m_updateEvent = threadFactory->createThreadEvent();
            m_stopEvent = threadFactory->createThreadEvent();
        } else {
            osre_error( Tag, "Invalid pointer to thread factory." );
        }
    }

    ~SystemTaskThread()	{
        if ( m_eventHandler ) {
            m_eventHandler->detach( nullptr );
        }

        delete m_updateEvent;
        m_updateEvent = nullptr;

        delete m_stopEvent;
        m_stopEvent = nullptr;
    }

    void setEventHandler( AbstractEventHandler *eventHandler ) {
        m_eventHandler = eventHandler;
        if ( m_eventHandler ) {
            m_eventHandler->attach( nullptr );
        } 
    }

    virtual bool stop() {
        AbstractThread::setState( ThreadState::Terminated );

        return true;
    }

    Common::AbstractEventHandler *getEventHandler() const {
        return m_eventHandler;
    }

    void setActiveJobQueue( Threading::TAsyncQueue<const TaskJob*> *pJobQueue ) {
        m_activeJobQueue = pJobQueue;
    }

    Threading::TAsyncQueue<const TaskJob*> *getActiveJobQueue() const {
        return m_activeJobQueue;
    }

    Platform::AbstractThreadEvent *getUpdateEvent() const {
        return m_updateEvent;
    }

    Platform::AbstractThreadEvent *getStopEvent() const {
        return m_stopEvent;
    }

protected:
    i32 run() {
        OSRE_ASSERT( nullptr != m_activeJobQueue );

        osre_debug( Tag, "SystemThread::run" );
        bool running = true;
        while ( running ) {
            m_activeJobQueue->awaitEnqueuedItem();
            while ( !m_activeJobQueue->isEmpty() )	{
                // for debugging
				if (DebugQueueSize) {
					ui32 size = m_activeJobQueue->size();
					std::stringstream stream;
					stream << "queue size = " << size << std::endl;
					osre_debug(Tag, stream.str());
				}

                const TaskJob *job = m_activeJobQueue->dequeue();
                const Common::Event *ev = job->getEvent();
                if ( nullptr == ev ) {
                    running = false;
                    OSRE_ASSERT(nullptr != ev);
                    continue;
                }

                if ( OnStopSystemTaskEvent == *ev ) {
                    osre_debug( Tag, "stop requested." );
                    running = false;
                }

                if ( m_eventHandler ) {
                    m_eventHandler->onEvent( *ev, job->getEventData() );
                }
            }

            if ( m_updateEvent ) {
                m_updateEvent->signal();
            }
        }

        if (m_stopEvent) {
            m_stopEvent->signal();
        }

        return 0;
    }

private:
    Platform::AbstractThreadEvent *m_updateEvent;
    Platform::AbstractThreadEvent *m_stopEvent;
    Threading::TAsyncQueue<const TaskJob*> *m_activeJobQueue;
    Common::AbstractEventHandler *m_eventHandler;
};

SystemTask::SystemTask( const String &taskName )
: AbstractTask( taskName )
, m_workingMode( Async )
, m_buffermode( SingleBuffer )
, m_taskThread( nullptr )
, m_asyncQueue( nullptr ) {
    // empty
}

SystemTask::~SystemTask() {
    OSRE_ASSERT( !isRunning() );
}

void SystemTask::setWorkingMode( AbstractTask::WorkingMode mode ) {
    if ( isRunning() ) {
        osre_error( Tag, "The working mode cannot be chanced in a running task." );
        return;
    }

    m_workingMode = mode;
}

AbstractTask::WorkingMode SystemTask::getWorkingMode() const {
    return m_workingMode;
}

void SystemTask::setBufferMode( BufferMode buffermode ) {
    m_buffermode = buffermode;
}

SystemTask::BufferMode SystemTask::getBufferMode() const {
    return m_buffermode;
}

bool SystemTask::start( AbstractThread *pThread ) {
    // ensure task is not running
    if( nullptr != m_taskThread ) {
        if ( AbstractThread::ThreadState::Running == m_taskThread->getCurrentState() ) {
            osre_debug( Tag, "Task " + Object::getName() + " is already running." );
            return false;
        }
    }

    // setup the thread context
    m_asyncQueue = new Threading::TAsyncQueue<const TaskJob*>( Platform::AbstractThreadFactory::getInstance() );
    if ( !pThread ) {
        m_taskThread = new SystemTaskThread( Object::getName() + ".thread", m_asyncQueue );
    } else {
        m_taskThread = reinterpret_cast<SystemTaskThread*>( pThread );
    }

    // start the system task
    return ( m_taskThread->start( nullptr ) );
}

bool SystemTask::stop() {
    if ( AbstractThread::ThreadState::Running != m_taskThread->getCurrentState() ) {
        osre_debug( Tag, "Task " + getName() + " is not running." );
        return false;
    }

    bool res( false );
    AbstractThreadEvent *stopEvent = m_taskThread->getStopEvent();
    sendEvent( &OnStopSystemTaskEvent, nullptr );
    
    if ( nullptr != stopEvent ) {
        stopEvent->waitForOne();
        m_taskThread->stop();
        delete m_taskThread;
        m_taskThread = nullptr;
        res = true;
    }

    return res;
}

bool SystemTask::isRunning() const {
    if ( nullptr != m_taskThread ) {
        return ( AbstractThread::ThreadState::Running == m_taskThread->getCurrentState() );
    }

    return false;
}

bool SystemTask::execute() {
    return true;
}

void SystemTask::setThreadInstance( AbstractThread *threadInstance ) {
    if ( nullptr != m_taskThread ) {
        if( Platform::AbstractThread::ThreadState::Running == m_taskThread->getCurrentState() ) {
            osre_debug( Tag, "Stopping SystemTask before setting new thread." );
            SystemTask::stop();
        }
    }

    m_taskThread = reinterpret_cast<SystemTaskThread*>( threadInstance );
}

void SystemTask::attachEventHandler( AbstractEventHandler *eventHandler ) {
    OSRE_ASSERT( nullptr != m_taskThread );

    m_taskThread->setEventHandler( eventHandler );
}

void SystemTask::detachEventHandler() {
    OSRE_ASSERT( nullptr != m_taskThread );
    if ( nullptr == m_taskThread ) {
        osre_debug( Tag, "TaskThread is nullptr." );
        return;
    }

    Common::AbstractEventHandler *eh = m_taskThread->getEventHandler();
    if( eh ) {
        m_taskThread->setEventHandler( NULL );
    }
}

bool SystemTask::sendEvent( const Event *ev, const EventData *eventData ) {
    OSRE_ASSERT( nullptr != m_asyncQueue );
    OSRE_ASSERT( nullptr != ev );

    TaskJob *taskJob = TaskJobAlloc( ev, eventData );
    m_asyncQueue->enqueue( taskJob );

    return true;
}

ui32 SystemTask::getEvetQueueSize() const {
    OSRE_ASSERT( nullptr != m_asyncQueue);

    return m_asyncQueue->size();
}

void SystemTask::onUpdate() {
    OSRE_ASSERT( nullptr != m_taskThread);

    if ( nullptr == m_asyncQueue ) {
        m_asyncQueue = m_taskThread->getActiveJobQueue();
    }
}

void SystemTask::awaitUpdate() {
    OSRE_ASSERT( nullptr != m_taskThread );

    if ( nullptr != m_taskThread ) {
        AbstractThreadEvent *threadEvent = m_taskThread->getUpdateEvent();
        if ( nullptr != threadEvent ) {
            threadEvent->waitForOne();
        }
    }
}

void SystemTask::awaitStop() {
    if ( nullptr != m_taskThread ) {
        AbstractThreadEvent *threadEvent = m_taskThread->getStopEvent();
        if ( nullptr != threadEvent ) {
            threadEvent->waitForOne();
        }
    }
}

SystemTask *SystemTask::create( const String &taskName ) {
    return new SystemTask( taskName );
}

} // Namespace Threading
} // Namespace OSRE
