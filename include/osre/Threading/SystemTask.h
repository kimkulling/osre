#pragma once

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
#ifndef ZFXCE2_INFRASTRUCTURE_THREADING_TASK_H_INC
#define ZFXCE2_INFRASTRUCTURE_THREADING_TASK_H_INC

#include <osre2/Infrastructure/Threading/AbstractTask.h>
#include <osre2/Infrastructure/Threading/TAsyncQueue.h>

namespace ZFXCE2 {

namespace Common {
    class Event;
}

namespace Threading {

class TaskManager;
class SystemTaskThread;
class TaskJob;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Threading::Task
///	@ingroup	Infrastructure
///
///	@brief	This class implements a system task. 
///
/// You can use a system task for a running service like an IO-task waiting in the background for any 
/// loader jobs. The system-task will run all the time after starting it and awaits jobs.
//-------------------------------------------------------------------------------------------------
class DLL_EXPORT SystemTask : public AbstractTask {
    friend class TaskManager;

public:
    ///	@brief	Overwritten, @see AbstractTask for more info's.
    virtual void setWorkingMode( WorkingMode mode );

    ///	@brief	Overwritten, @see AbstractTask.
    virtual WorkingMode getWorkingMode() const;

    virtual void setBufferMode( BufferMode buffermode );
    virtual BufferMode getBufferMode() const;

    ///	@brief	Overwritten, @see AbstractTask.
    virtual bool start( Platform::AbstractThread *pThread );

    ///	@brief	Overwritten, @see AbstractTask.
    virtual bool stop();

    ///	@brief	Overwritten, @see AbstractTask.
    virtual bool isRunning() const;

    ///	@brief	Overwritten, @see AbstractTask.
    virtual bool execute();

    ///	@brief	Overwritten, @see AbstractTask.
    virtual void setThreadInstance( Platform::AbstractThread *pThreadInstance );
    
    ///	@brief	Overwritten, @see AbstractTask.
    virtual void onUpdate();
    
    ///	@brief	Overwritten, @see AbstractTask.
    virtual void await();

    ///	@brief	An event handler will be attached.
    ///	@param	pEventHandler	[in] A pointer showing to the event handler to attach.
    virtual void attachEventHandler( Common::AbstractEventHandler *pEventHandler );
    
    ///	@brief	An event handler will be detached.
    virtual void detachEventHandler();
    
    ///	@brief	A new task job will be enqueued.
    ///	@param	pEvent		[in] A pointer showing to the event, which describes the kind of job.
    ///	@param	pEventData	[in] A pointer showing to the event data.
    ///	@return	true, if the enqueue operation was successful, false if not.
    virtual bool sendEvent( const Common::Event *pEvent, const Common::EventData *pEventData );
    
    ///	@brief	Returns the number of enqueued jobs.
    ///	@return	The number of attached jobs.
    virtual ui32 getEvetQueueSize() const;
    
    ///	@brief	The factory method, creates a new instance of the system task.
    static SystemTask *create( const ce_string &rTaskName );

protected:
    ///	@brief	The class constructor.
    ///	@param	taskName	[in] The class instance name.
    SystemTask( const ce_string &taskName );

    ///	@brief	The class destructor, virtual.
    virtual ~SystemTask();
    
private:
    WorkingMode m_WorkingMode;
    BufferMode m_buffermode;
    SystemTaskThread *m_pTaskThread;
    typedef Threading::TAsyncQueue<const TaskJob*> TaskQueue;
    TaskQueue *m_pAsyncQueue;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2

#endif // ZFXCE2_INFRASTRUCTURE_THREADING_TASK_H_INC
