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

#include <osre/Common/Object.h>
#include <cppcore/Container/TQueue.h>

namespace OSRE {
namespace Threading {

class AbstractTask;
class AsyncTask;
class ThreadPool;
class WorkerThread;

//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Threading::TaskScheduler
///	@ingroup	Infrastructure
///
///	@brief	This class can be used to create and release async task instances. you can also use it 
///	to dispatch your worker task to a free worker thread.
//-------------------------------------------------------------------------------------------------
class TaskScheduler : public Common::Object {
public:
	///	@brief	The class destructor, virtual.
	virtual ~TaskScheduler();

	///	@brief	A new async task instance will be created.
	///	@param	rTaskName	[in] The name for the new async task instance.
	///	@return	A pointer showing to the async task instance will be returned.
	virtual AsyncTask *createTask( const String &rTaskName );

	///	@brief	A given async task instance will be released.
	///	@param	pTask	[in] A pointer showing to the async task instance to be released.
	///	@return	true, if the release was successful, false if not.
	///	@note	The pointer showing to the async task instance must me not NULL.
	virtual bool releaseTask( AsyncTask *pTask );

	///	@brief	Lookup method for a async task instance.
	///	@param	rTaskName	[in] The name of the task to look for.
	///	@return	Returns a pointer showing to the task instance, if it was found, else a 
	///	NULL-pointer will be returned.
	virtual AsyncTask *getTask( const String &rTaskName ) const;
	
	///	@brief	An async task will be dispatched.
	///	@param	pTask	[in] A pointer showing to the async task instance.
	///	@return	true, if the task was already started, false if it is in waiting mode.
	virtual bool dispatch( AsyncTask *pTask );

	///	@brief	The update callback, will be called one per frame.
	virtual void update();

	///	@brief	A new instance of the task scheduler will be created.
	///	@return	A pointer showing to the task scheduler instance.
	static TaskScheduler *create();
	
	///	@brief	Set the current task scheduler instance.
	///	@param	pInstance	[in] The new scheduler instance.
	static void setInstance( TaskScheduler *pInstance );

	///	@brief	Returns the current task scheduler instance. You can only use one scheduler per 
	///	application.
	///	@return	A pointer showing to the tasks scheduler instance. If no task scheduler instance 
	///	was set NULL will be returned.
	static TaskScheduler *getInstance();

	///	@brief	The task scheduler will be destroyed.
	static void destroy();

protected:
	///	@brief	The default class constructor.
	TaskScheduler();

	///	@brief	Runs an async task instance.
	///	@param	pTask	[in] A pointer showing to the async task instance.
	///	@return	true, if the task was started, false if not.
	///	@remark	A task cannot be started, if no thread is free.
	bool run( AsyncTask *pTask );

private:
	static TaskScheduler *s_pInstance;
	ThreadPool *m_pThreadPool;
	typedef CPPCore::TQueue<AsyncTask*> AsyncTaskQueue;
	AsyncTaskQueue m_AsyncTaskQueue;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace OSRE
