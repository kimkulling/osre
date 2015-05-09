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
--------------------------------------------------------------------------------------------------*/
#pragma once

#include <osre/Threading/AbstractTask.h>
#include <osre/Threading/TAsyncQueue.h>

namespace OSRE {

namespace Platform {
    class AbstractThreadEvent;
}

namespace Threading {

class WorkerThread;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Threading::AsyncTask
///	@ingroup	Infrastructure
///
///	@brief  This task can be used to implement a async. handled task.
//-------------------------------------------------------------------------------------------------
class AsyncTask : public AbstractTask {
public:
    ///	@brief	The class constructor with the name of the task
    ///	@param	taskName	[in] The name for the task.
    AsyncTask( const String &taskName );

    ///	@brief	The class destructor, virtual.
    virtual ~AsyncTask();

    ///	@brief	Requests a new working mode.
    ///	@param	mode	[in] The requested working mode.
    virtual void setWorkingMode( WorkingMode mode );

    ///	@brief	The current working mode will be returned, should by async.
    ///	@return	The current working mode.
    virtual WorkingMode getWorkingMode() const;

    virtual void setBufferMode( BufferMode buffermode );
    virtual BufferMode getBufferMode() const;

    ///	@brief	Set the new thread instance.
    ///	@param	pThreadInstance		[in] A pointer showing to the thread instance.
    virtual void setThreadInstance( Platform::AbstractThread *pThreadInstance );

    ///	@brief	Callback which will be executed before the task execute method.
    ///	@return	true, if the pre-executor implementation was successful, false if not.
    virtual bool preExecute();
    
    ///	@brief	Callback which will execute the task.
    ///	@return	true, if the executor implementation was successful, false if not.
    virtual bool execute();
    
    ///	@brief	Callback which will be executed after the task execute method.
    ///	@return	true, if the post-executor implementation was successful, false if not.
    virtual bool postExecute();

    ///	@brief	The update call-back.
    virtual void onUpdate();

    ///	@brief	Awaits an incoming signal.
    virtual void await();

    ///	@brief	The creation method.
    ///	@param	taskName	[in] The name for the new created task.
    ///	@return	A pointer showing to the new created task instance.
    static AsyncTask *create( const String &taskName );

private:
    WorkingMode m_WorkingMode;
    BufferMode m_buffermode;
    bool m_IsRunning;
    WorkerThread *m_pThreadInstance;
    TAsyncQueue<AsyncTask*> *m_pTaskQueue;
    Platform::AbstractThreadEvent *m_pFinishedEvent;
};

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Threading::EmptyTask
///	@ingroup	Infrastructure
///
///	@brief	An empty task as a placeholder.
//-------------------------------------------------------------------------------------------------
class EmptyTask : public AsyncTask {
public:
    ///	The class constructor.
    EmptyTask();

    ///	The class destructor.
    ~EmptyTask();

    ///	No execution code.
    virtual bool execute(); 

    ///	Creates a new instance.
    static EmptyTask *create();
};

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2
