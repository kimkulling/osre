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
#pragma once
#ifndef ZFXCE2_INFRASTRUCTURE_PLATFORM_ABSTRACTTHREAD_H_INC
#define ZFXCE2_INFRASTRUCTURE_PLATFORM_ABSTRACTTHREAD_H_INC

#include <osre/Common/Types.h>

namespace ZFXCE2 {
namespace Platform {

class AbstractThreadEvent;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::IThread
///	@ingroup	Infrastructure
///
///	@brief	This abstract class declares the interface for a thread implementation.

///	Threads are OS-specific.
//-------------------------------------------------------------------------------------------------
class DLL_EXPORT AbstractThread {
public:
    ///	The function pointer for a user-specific thread-function.
    typedef ui32 (*threadfunc) ( void * );

    ///	@enum	Priority
    ///	@brief	This enum describes the priority of the thread.
    enum Priority {
        Low,	///< Low prio thread.
        Normal,	///< Normal prio thread.
        High	///< High prio thread.
    };

    ///	@enum	ThreadState
    ///	@brief	Describes the current state of the thread.
    enum ThreadState {
        New,			///< In new state, just created
        Running,		///< thread is currently running
        Waiting,		///< Awaits a signal
        Suspended,		///< Is suspended
        Terminated		///< Thread is terminated, will be destroyed immediately
    };

public:
    ///	@brief	The destructor, virtual.
    virtual ~AbstractThread();

    ///	@brief	The thread will be started.
    ///	@param	pData	The data pointer.
    ///	@return	true, if the startup was successful, false if not.
    virtual bool start( void *pData ) = 0;

    ///	@brief	The thread will be stopped. The thread must be in running mode before.
    ///	@return	true, if the thread was stopped, false if not.
    virtual bool stop() = 0;

    ///	@brief	The running state of the thread will be returned.
    ///	@return	true, if the thread is in running-state. false if the thread is in stop-state.
    //virtual bool isRunning() = 0;

    ///	@brief	Returns the state of the thread.
    ///	@return	The current thread state.
    virtual ThreadState getCurrentState() const = 0;

    ///	@brief	The thread will be suspended.
    ///	@return	true, if the suspend was successful, false if not.
    virtual bool suspend() = 0;

    ///	@brief	Resumes the thread.
    ///	@return	true, if resuming was successful, false if not.
    virtual bool resume() = 0;

    ///	@brief	Sets the name of the thread.
    ///	@param	rName	The new name for the thread.
    virtual void setName( const ce_string &rName ) = 0;

    ///	@brief	Returns the current name of the thread.
    ///	@return	The current name of the thread.
    virtual const ce_string &getName() const = 0;

    ///	@brief	Set the new stack size.
    ///	@param	stacksize	The new stack size.
    virtual void setStackSize( ui32 stacksize ) = 0;

    ///	@brief	Returns the current stacksize.
    ///	@return	The current stacksize of the thread.
    virtual ui32 getStackSize() const = 0;

    ///	@brief	Waits until the thread is signaled by an event.
    ///	@param	ms				The timeout, set to 0 for infinite.
    virtual void waitForTimeout( ui32 ms ) = 0;

    ///	@brief	Waits for infinity for a signal.
    virtual void wait() = 0;

    ///	@brief	Returns a pointer showing to the thread event of this thread.
    ///	@return	A pointer showing to the thread event, equal NULL if nothing.
    virtual AbstractThreadEvent *getThreadEvent() const = 0;

    ///	@brief	Assigns a new thread priority to the thread.
    ///	@param	prio	The new thread prio.
    virtual void setPriority( Priority prio ) = 0;

    ///	@brief	Returns the current thread prio.
    ///	@return	The current thread prio.
    virtual Priority getPriority() const = 0;

    ///	@brief	The assigned name of the thread will be returned.
    ///	@return	The assigned name of the thread.
    virtual const ce_string &getThreadName() const = 0;

protected:
    ///	@brief	Overwrite this for your own thread running method.
    virtual i32 run() = 0;

    ///	@brief	The new state will be set.
    ///	@param	newState	[in] The new state.
    virtual void setState( ThreadState newState ) = 0;
};

//-------------------------------------------------------------------------------------------------
inline
AbstractThread::~AbstractThread() {
    // empty
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2

#endif // ZFXCE2_INFRASTRUCTURE_PLATFORM_ABSTRACTTHREAD_H_INC
