#pragma once

#include <osre/Common/Types.h>

namespace OSRE {
namespace Threading {

class AsyncTask;
class TaskScheduler;

//-------------------------------------------------------------------------------------------------
///	@enum	TaskGroupState
///	@brief	
//-------------------------------------------------------------------------------------------------
enum TaskGroupState {
	NotComplete,	///<
	Complete,		///<
	Cancelled		///<
};

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Threading::TaskGroup
///	@ingroup	Infrastructure
///
///	@brief 
//-------------------------------------------------------------------------------------------------
class TaskGroup {
public:
	TaskGroup( const String &groupName,  AsyncTask *pTask );
	~TaskGroup();
	AsyncTask *getRoot() const;
	void run( TaskScheduler *pTaskScheduler = NULL );
	TaskGroupState wait();
	TaskGroupState cancel();
	void enqueue( AsyncTask *pTask );

private:
	TaskGroup();
	TaskGroup &operator = ( const TaskGroup & );

private:
	const String m_groupname;
	AsyncTask *m_pRoot;
	TaskScheduler *m_pScheduler;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2
