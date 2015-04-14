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
#ifndef ZFXCE2_INFRASTRUCTURE_Common_ITASKMANAGER_H_INC
#define ZFXCE2_INFRASTRUCTURE_Common_ITASKMANAGER_H_INC

#include <osre2/Infrastructure/Common/Object.h>
#include <cppCommon/Container/TQueue.h>

namespace ZFXCE2 {
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
class DLL_EXPORT TaskScheduler : public Common::Object {
public:
	///	@brief	The class destructor, virtual.
	virtual ~TaskScheduler();

	///	@brief	A new async task instance will be created.
	///	@param	rTaskName	[in] The name for the new async task instance.
	///	@return	A pointer showing to the async task instance will be returned.
	virtual AsyncTask *createTask( const ce_string &rTaskName );

	///	@brief	A given async task instance will be released.
	///	@param	pTask	[in] A pointer showing to the async task instance to be released.
	///	@return	true, if the release was successful, false if not.
	///	@note	The pointer showing to the async task instance must me not NULL.
	virtual bool releaseTask( AsyncTask *pTask );

	///	@brief	Lookup method for a async task instance.
	///	@param	rTaskName	[in] The name of the task to look for.
	///	@return	Returns a pointer showing to the task instance, if it was found, else a 
	///	NULL-pointer will be returned.
	virtual AsyncTask *getTask( const ce_string &rTaskName ) const;
	
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
	typedef CPPCommon::TQueue<AsyncTask*> AsyncTaskQueue;
	AsyncTaskQueue m_AsyncTaskQueue;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2

#endif // ZFXCE2_INFRASTRUCTURE_Common_ITASKMANAGER_H_INC
