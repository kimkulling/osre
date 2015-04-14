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
#ifndef CE_INFRASTRUCTURE_THREADING_TASKGROUP_H_INC
#define CE_INFRASTRUCTURE_THREADING_TASKGROUP_H_INC

#include <osre2/Types.h>

namespace ZFXCE2 {
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
class DLL_EXPORT TaskGroup {
public:
	TaskGroup( const ce_string &groupName,  AsyncTask *pTask );
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
	const ce_string m_groupname;
	AsyncTask *m_pRoot;
	TaskScheduler *m_pScheduler;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2

#endif // CE_INFRASTRUCTURE_THREADING_TASKGROUP_H_INC
