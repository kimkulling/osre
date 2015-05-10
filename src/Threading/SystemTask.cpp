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
#include <osre/Threading/SystemTask.h>
#include <osre/Threading/TAsyncQueue.h>
#include <osre/Threading/TaskJob.h>
#include <osre/Common/AbstractEventHandler.h>
#include <osre/Common/Event.h>
#include <cassert>

#ifdef _WIN32
#   include <src/Platform/win32/Win32Thread.h>
#else
#   include <src/Platform/sdl2/SDL2Thread.h>
#endif

#include <sstream>

namespace OSRE {
namespace Threading {

using namespace ::OSRE::Common;

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Threading::TaskThread
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
    SystemTaskThread( const String &rThreadName, TAsyncQueue<const TaskJob*> *pJobQueue )
    : Win32Thread( rThreadName, StackSize )
#else
    SystemTaskThread( const String &rThreadName, TAsyncQueue<const TaskJob*> *pJobQueue )
    : SDL2Thread( rThreadName, StackSize )
#endif
    , m_pUpdateEvent( nullptr )
    , m_pActiveJobQueue( pJobQueue )
    , m_pEventHandler( nullptr ) {
        assert( nullptr != pJobQueue );

        Platform::AbstractThreadFactory *pThreadFactory( Platform::AbstractThreadFactory::getInstance() );
        if ( pThreadFactory ) {
            m_pUpdateEvent = pThreadFactory->createThreadEvent();
        } else {
            osre_error( "Invalid pointer to thread factory." );
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
        osre_debug( "SystemThread::run" );
        bool running = true;
        while ( running ) {
            m_pActiveJobQueue->awaitEnqueuedItem();
            while ( !m_pActiveJobQueue->isEmpty() )	{
                // for debugging
                //ui32 size = m_pActiveJobQueue->size();
                //std::stringstream stream;
                //stream << "queue size = " << size << std::endl;
                //osre_debug( stream.str() );

                const TaskJob *pJob = m_pActiveJobQueue->dequeue();
                const Common::Event *pEvent = pJob->getEvent();
                if ( !pEvent ) {
                    running = false;
                    assert( nullptr != pEvent );
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
SystemTask::SystemTask( const String &taskName )
: AbstractTask( taskName )
, m_WorkingMode( Async )
, m_buffermode( SingleBuffer )
, m_pTaskThread( nullptr )
, m_pAsyncQueue( nullptr ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
SystemTask::~SystemTask() {
    assert( !isRunning() );
}

//-------------------------------------------------------------------------------------------------
void SystemTask::setWorkingMode( AbstractTask::WorkingMode mode ) {
    if ( isRunning() ) {
        osre_error( "The working mode cannot be chanced in a running task." );
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
            osre_debug( "Task " + getName() + " is already running." );
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
        osre_debug( "Task " + getName() + " is not running." );
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
    assert( nullptr != m_pTaskThread );

    m_pTaskThread->setEventHandler( pEventHandler );
}

//-------------------------------------------------------------------------------------------------
void SystemTask::detachEventHandler() {
    assert( nullptr != m_pTaskThread );

    Common::AbstractEventHandler *pEH = m_pTaskThread->getEventHandler();
    if( pEH  ) {
        m_pTaskThread->setEventHandler( NULL );
    }
}

//-------------------------------------------------------------------------------------------------
bool SystemTask::sendEvent( const Common::Event *pEvent, const Common::EventData *pEventData ) {
    assert( nullptr != m_pAsyncQueue );
    assert( nullptr != pEvent );

    TaskJob *pTaskJob = new TaskJob( pEvent, pEventData );
    m_pAsyncQueue->enqueue( pTaskJob );

    return true;
}

//-------------------------------------------------------------------------------------------------
ui32 SystemTask::getEvetQueueSize() const {
    assert( NULL != m_pAsyncQueue );

    return m_pAsyncQueue->size();
}

//-------------------------------------------------------------------------------------------------
void SystemTask::onUpdate() {
    assert( NULL != m_pTaskThread );

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
SystemTask *SystemTask::create( const String &rTaskName ) {
    return new SystemTask( rTaskName );
}

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2
