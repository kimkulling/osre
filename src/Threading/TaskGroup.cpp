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
} // Namespace OSRE
