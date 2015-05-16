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
} // Namespace OSRE
