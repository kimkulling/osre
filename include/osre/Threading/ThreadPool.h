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
#ifndef CE_INFRASTRUCTURE_THREADING_THREADPOOL_H_INC
#define CE_INFRASTRUCTURE_THREADING_THREADPOOL_H_INC

#include <osre2/Infrastructure/Common/Object.h>
#include <cppCommon/Container/TArray.h>

namespace ZFXCE2 {
namespace Threading {

class WorkerThread;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Threading::ThreadPool
///	@ingroup	Infrastructure
///
///	@brief	This class stores a bundle of worker threads, which you can use to startup async tasks.
//-------------------------------------------------------------------------------------------------
class DLL_EXPORT ThreadPool : public Common::Object {
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
	typedef CPPCommon::TArray<WorkerThread*> ThreadArray;
	ThreadArray m_ThreadArray;
	ThreadArray m_SuspendedThreadArray;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2

#endif // CE_INFRASTRUCTURE_THREADING_THREADPOOL_H_INC
