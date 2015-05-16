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
} // Namespace OSRE
