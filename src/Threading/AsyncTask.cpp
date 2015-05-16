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
#include <osre/Threading/AsyncTask.h>
#include <osre/Platform/AbstractThread.h>
#include <osre/Platform/AbstractThreadEvent.h>

#include "WorkerThread.h"

namespace OSRE {
namespace Threading {

using namespace OSRE::Platform;

//-------------------------------------------------------------------------------------------------
AsyncTask::AsyncTask( const String &name )
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
        assert( nullptr != m_pTaskQueue );
        assert( nullptr != m_pFinishedEvent );
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
AsyncTask *AsyncTask::create( const String &taskName ) {
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
} // Namespace OSRE

