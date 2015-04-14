/*
ZFX Community Engine 2  (ZFXCE2)
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
---------------------------------------------------------------------------------------------------
*/
#include <osre2/Infrastructure/Threading/TaskGroup.h>
#include <osre2/Infrastructure/Threading/AsyncTask.h>
#include <osre2/Infrastructure/Threading/TaskScheduler.h>

namespace ZFXCE2 {
namespace Threading {

TaskGroup::TaskGroup( const ce_string &groupName,  AsyncTask *pTask  )  :
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
