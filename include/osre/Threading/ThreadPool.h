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
} // Namespace ZFXCE2
