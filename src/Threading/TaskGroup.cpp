#include <osre/Threading/TaskGroup.h>
#include <osre/Threading/AsyncTask.h>
#include <osre/Threading/TaskScheduler.h>

namespace OSRE {
namespace Threading {

TaskGroup::TaskGroup( const String &groupName,  AsyncTask *pTask  )  :
	m_groupname( groupName ),
	m_pRoot( nullptr ),
	m_pScheduler( nullptr )
{
	if ( pTask ) {
		m_pRoot = pTask;
	} else {
		m_pRoot = EmptyTask::create();
	}
}

TaskGroup::~TaskGroup() {
	m_pRoot->release();
}

AsyncTask *TaskGroup::getRoot() const {
	return m_pRoot;
}

void TaskGroup::run( TaskScheduler *pTaskScheduler ) {
	if ( !pTaskScheduler ) {
		return;
	}

	pTaskScheduler->dispatch( m_pRoot );
	const ui32 numChilds = m_pRoot->getNumChildTasks();
	if ( numChilds > 0 ) {
		for ( ui32 idx=0; idx<numChilds; ++idx ) {
			AsyncTask *pChildTask = (AsyncTask*) m_pRoot->getChildTask( idx );
			if ( pChildTask ) {
				pTaskScheduler->dispatch( pChildTask );
			}
		}
	}
}

TaskGroupState TaskGroup::wait() {
	if ( m_pRoot ) {
		for ( ;; ) {
			m_pRoot->await();
			if ( m_pRoot->getNumRefs() == 0 ) {
				break;
			}
		}
		return Complete;
	} else {
		return NotComplete;
	}
}

TaskGroupState TaskGroup::cancel() {
	return Cancelled;
}

void TaskGroup::enqueue( AsyncTask *pTask ) {
	if ( m_pRoot ) {
		m_pRoot->enqueue( pTask );
	}
}

} // Namespace Threading
} // Namespace ZFXCE2
