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
    WorkerThread(  const ce_string &name, ui32 stacksize );
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
    const ce_string &getName() const;

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
