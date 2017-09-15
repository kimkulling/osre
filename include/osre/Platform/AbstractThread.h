/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace Platform {

class AbstractThreadEvent;
class AbstractThreadLocalStorage;

/// @brief  Manages platform-independent thread id
struct ThreadId {
    unsigned long Id;

    /// @brief  The default constructor.
    ThreadId()
    : Id( 0 ) {
        // empty
    }

    /// @brief  The equal operator implementation.
    bool operator == ( const ThreadId &rhs ) const {
        return Id == rhs.Id;
    }
    
    bool operator != ( const ThreadId &rhs ) const {
        return Id != rhs.Id;
    }
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This abstract class declares the interface for a generic thread API.
///	Threads are OS-specific.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractThread {
public:
    ///	The function pointer for a user-specific thread-function.
    typedef ui32 (*threadfunc) ( void * );

    ///	@brief	This enum describes the priority of the thread.
    enum class Priority {
        Low,	///< Low prio thread.
        Normal,	///< Normal prio thread.
        High	///< High prio thread.
    };

    ///	@enum	ThreadState
    ///	@brief	Describes the current state of the thread.
    enum class ThreadState {
        New,			///< In new state, just created
        Running,		///< thread is currently running
        Waiting,		///< Awaits a signal
        Suspended,		///< Is suspended
        Terminated		///< Thread is terminated, will be destroyed immediately
    };

    ///	@brief	The destructor, virtual.
    virtual ~AbstractThread();

    ///	@brief	The thread will be started.
    ///	@param	pData	[in] The data pointer.
    ///	@return	true, if the startup was successful, false if not.
    virtual bool start( void *pData ) = 0;

    ///	@brief	The thread will be stopped. The thread must be in running mode before.
    ///	@return	true, if the thread was stopped, false if not.
    virtual bool stop() = 0;

    ///	@brief	Returns the state of the thread.
    ///	@return	The current thread state.
    virtual ThreadState getCurrentState() const;

    ///	@brief	The thread will be suspended.
    ///	@return	true, if the suspend was successful, false if not.
    virtual bool suspend() = 0;

    ///	@brief	Resumes the thread.
    ///	@return	true, if resuming was successful, false if not.
    virtual bool resume() = 0;

    ///	@brief	Sets the name of the thread.
    ///	@param	name	[in] The new name for the thread.
    virtual void setName( const String &name ) = 0;

    ///	@brief	Returns the current name of the thread.
    ///	@return	The current name of the thread.
    virtual const String &getName() const = 0;

    ///	@brief	Waits until the thread is signaled by an event.
    ///	@param	ms		[in] The timeout, set to 0 for infinite.
    virtual void waitForTimeout( ui32 ms ) = 0;

    ///	@brief	Waits for infinity for a signal.
    virtual void wait() = 0;

    ///	@brief	Returns a pointer showing to the thread event of this thread.
    ///	@return	A pointer showing to the thread event, equal NULL if nothing.
    virtual AbstractThreadEvent *getThreadEvent() const = 0;

    ///	@brief	Assigns a new thread priority to the thread.
    ///	@param	prio	[in] The new thread prio.
    virtual void setPriority( Priority prio ) = 0;

    ///	@brief	Returns the current thread prio.
    ///	@return	The current thread prio.
    virtual Priority getPriority() const = 0;

    ///	@brief	The assigned name of the thread will be returned.
    ///	@return	The assigned name of the thread.
    virtual const String &getThreadName() const = 0;

    /// @brief  Returns the thread storage instance.
    /// @param  A pointer showing to the thread local storage instance.
    virtual AbstractThreadLocalStorage *getThreadLocalStorage() = 0;
    
    /// @brief  Will set the local thread storage instance.
    /// @param  tls     [in] The new lts instance.
    virtual void setThreadLocalStorage( AbstractThreadLocalStorage *tls ) = 0;
    
    /// @brief  Will set the thread id.
    /// @param  id       [in] The new thread id.
    virtual void setThreadId( const ThreadId &id ) = 0;

    /// @brief  Returns the thread id.
    /// @return The thread id.
    virtual ThreadId getThreadId() = 0;

protected:
    /// @brief  The default constructor.
    AbstractThread();

    /// @brief  Hook to react on thread state changes.
    virtual void onNewState();

    ///	@brief	Overwrite this for your own thread running method.
    virtual i32 run() = 0;

    ///	@brief	The new state will be set.
    ///	@param	newState	[in] The new state.
    virtual void setState( ThreadState newState );

private:
    ThreadState m_threadState;
};

inline
AbstractThread::AbstractThread()
: m_threadState( ThreadState::New ) {
    // empty
}

inline
AbstractThread::~AbstractThread() {
    // empty
}

inline
void AbstractThread::onNewState() {
    // override me!
}

inline
AbstractThread::ThreadState AbstractThread::getCurrentState() const {
    return m_threadState;
}

inline
void AbstractThread::setState(ThreadState newState) {
    if ( m_threadState == newState) {
        return;
    }

    m_threadState = newState;
    onNewState();
}

} // Namespace Platform
} // Namespace OSRE
