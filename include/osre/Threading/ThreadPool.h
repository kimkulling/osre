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
#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace Threading {

class WorkerThread;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Threading::ThreadPool
///	@ingroup	Infrastructure
///
///	@brief	This class stores a bundle of worker threads, which you can use to startup async tasks.
//-------------------------------------------------------------------------------------------------
class ThreadPool : public Common::Object {
public:
	///	@brief	The class constructor.
	///	@param	numThreads	[in] The number of threads to create.
	ThreadPool( ui32 numThreads );

	///	@brief	The class destructor.
	~ThreadPool();
	
	///	@brief	The number of suspended threads will be returned.
	///	@return	The number of suspended threads.
	ui32 getNumSuspendedThreads() const;
	
	///	@brief	The whole pool size will be returned. This value corresponds to the number of
	///	pooled threads.
	///	@return	The pool size
	ui32 getPoolSize() const;
	
	///	@brief	A suspended thread will be returned. 
	///	@return	A pointer showing to the thread. Is NULL if no suspended thread is free.
	///	@remark	After returning a free thread instance the number of suspended threads will be 
	///	decremented by one.
	WorkerThread *getThread();
	
	///	@brief	A thread instance will be places at the list of suspended threads. So you can 
	///	reuse this instance for other jobs.
	///	@param	pThread		[in] A pointer showing to the thread instance.
	void releaseThread( WorkerThread *pThread );

private:
	typedef CPPCore::TArray<WorkerThread*> ThreadArray;
	ThreadArray m_ThreadArray;
	ThreadArray m_SuspendedThreadArray;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace OSRE
