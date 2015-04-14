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
#ifndef ZFXCE2_INFRASTRUCTURE_PLATFORM_WIN32THREAD_H_INC
#define ZFXCE2_INFRASTRUCTURE_PLATFORM_WIN32THREAD_H_INC

#include <osre2/Infrastructure/Platform/AbstractThread.h>

#include <Windows.h>

namespace ZFXCE2 {
namespace Platform {

class Win32ThreadEvent;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Threading::Win32Thread
///	@ingroup	Infrastructure
///
///	@brief	This class implements a thread which uses the Win32-API.
//-------------------------------------------------------------------------------------------------
class DLL_EXPORT Win32Thread : public AbstractThread {
public:
	///	The class constructor.
	Win32Thread( const ce_string &threadName, ui32 stacksize );
	///	The class destructor.
	virtual ~Win32Thread();
	///	Starts the thread.
	bool start(  void *pData );
	///	Stops the thread, must be started before.
	bool stop();
	///	Returns the current state of the thread.
	ThreadState getCurrentState() const;
	///	The running thread will be suspended.
	bool suspend();
	///	The suspended thread will be resumed.
	bool resume();
	///	Assigns a new name to the thread.
	void setName( const ce_string &rName );
	///	Returns the thread name.
	const ce_string &getName() const;
	///	Sets a new stack size.
	void setStackSize( ui32 stacksize );
	///	Returns the current thread size.
	ui32 getStackSize() const;
	///	Awaits a timeout.
	void waitForTimeout( ui32 ms );
	///Awaits a signal.
	void wait();
	///	Returns a pointer showing to the assigned thread event.
	AbstractThreadEvent *getThreadEvent() const;
	///	Assigns a new thread priority to the thread.
	void setPriority( Priority prio );
	///	Returns the current thread priority.
	Priority getPriority() const;
	///	Returns the name of the thread.
	const ce_string &getThreadName() const;

protected:
	///	Override this for your own thread function.
	virtual i32 run();
	///	Set the new state of the thread.
	void setState( ThreadState newState );
	///	The Win32-specific thread function.
	static ui32 WINAPI ThreadFunc( LPVOID data );
	///	Will assign a thread name.
	static void setThreadName( const c8 *pName );

private:
	HANDLE m_ThreadHandle;
	Win32ThreadEvent *m_pThreadSignal;
	Priority m_Prio;
	ThreadState m_ThreadState;
	ce_string m_ThreadName;
	ui32 m_StackSize;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2

#endif // ZFXCE2_INFRASTRUCTURE_PLATFORM_WIN32THREAD_H_INC
