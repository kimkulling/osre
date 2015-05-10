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
#pragma once

#include <osre/Common/Types.h>
#include <osre/Threading/TAsyncQueue.h>
#include <osre/Common/Object.h>
#include <osre/Platform/AbstractThread.h>

namespace OSRE {

namespace Platform {
    class AbstractThread;
    class AbstractThreadEvent;
    class AbstractThread;
}

namespace Threading {

class AsyncTask;

//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Threading::WorkerThread
///	@ingroup	Infrastructure
///
///	@brief  This class implements a worker thread.
//-------------------------------------------------------------------------------------------------
class WorkerThread {
public:
    ///	The class constructor with the name and the stack size.
    WorkerThread(  const String &name, ui32 stacksize );
    /// The class destructor.
    ~WorkerThread();
    ///	returns the finished event.
    Platform::AbstractThreadEvent *getFinishEvent( ) const;
    ///	Returns the task queue.
    Threading::TAsyncQueue<AsyncTask*> *getTaskQueue() const;
    /// Starts the thread execution.
    bool start( void *pData );
    /// Stops the thread execution.
    bool stop();
    /// Returns the current state of the thread.
    Platform::AbstractThread::ThreadState getCurrentState() const;
    /// The thread will be suspended.
    void suspend();
    /// The name of the thread will be returned.
    const String &getName() const;

protected:
    i32 run();

private:
    Platform::AbstractThread *m_pImpl;	
    Platform::AbstractThreadEvent *m_pFinishEvent;
    Threading::TAsyncQueue<AsyncTask*> *m_pTaskQueue;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2
