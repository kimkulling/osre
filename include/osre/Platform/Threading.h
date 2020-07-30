/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Platform/PlatformCommon.h>
#ifdef OSRE_WINDOWS
#   include <osre/Platform/Windows/MinWindows.h>
#else
#   include <SDL.h>

class ThreadEvent;
class ThreadLocalStorage;

#endif

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This abstract class declares the interface for critical sections. Override this for
/// your own implementation.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT CriticalSection {
public:
    CriticalSection();

    ///	@brief	The class destructor, virtual.
    ~CriticalSection();

    ///	@brief	The critical section will be entered.
    void enter();

    ///	@brief	Tries to enter the critical section.
    ///	@return	true, if entering was successful, false if not.
    bool tryEnter();

    ///	@brief	The critical section will be leaved.
    void leave();

private:
#ifdef OSRE_WINDOWS
    CRITICAL_SECTION m_CriticalSection;
#else
    SDL_SpinLock m_spinlock;
#endif
};

/// @brief  Manages platform-independent thread id
struct ThreadId {
    unsigned long Id;

    /// @brief  The default constructor.
    ThreadId() :
            Id(0) {
        // empty
    }

    /// @brief  The equal operator implementation.
    bool operator==(const ThreadId &rhs) const {
        return Id == rhs.Id;
    }

    bool operator!=(const ThreadId &rhs) const {
        return Id != rhs.Id;
    }
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This abstract class declares the interface for a generic thread API.
///	Threads are OS-specific.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Thread {
public:
    ///	The function pointer for a user-specific thread-function.
    typedef ui32 (*threadfunc)(void *);

    ///	@brief	This enum describes the priority of the thread.
    enum class Priority {
        Low, ///< Low prio thread.
        Normal, ///< Normal prio thread.
        High ///< High prio thread.
    };

    ///	@enum	ThreadState
    ///	@brief	Describes the current state of the thread.
    enum class ThreadState {
        New, ///< In new state, just created
        Running, ///< thread is currently running
        Waiting, ///< Awaits a signal
        Suspended, ///< Is suspended
        Terminated ///< Thread is terminated, will be destroyed immediately
    };

    /// @brief  The class constructor.
    /// @param  name        [in] The thread-name.
    /// @param  stacksize   [in] The stack-size.
    Thread(const String &name, ui32 stacksize);

    ///	@brief	The class destructor.
    virtual ~Thread();

    ///	@brief	The thread will be started.
    ///	@param	pData	[in] The data pointer.
    ///	@return	true, if the startup was successful, false if not.
    bool start(void *pData);

    ///	@brief	The thread will be stopped. The thread must be in running mode before.
    ///	@return	true, if the thread was stopped, false if not.
    bool stop();

    ///	@brief	Returns the state of the thread.
    ///	@return	The current thread state.
    ThreadState getCurrentState() const;

    ///	@brief	The thread will be suspended.
    ///	@return	true, if the suspend was successful, false if not.
    bool suspend();

    ///	@brief	Resumes the thread.
    ///	@return	true, if resuming was successful, false if not.
    bool resume();

    ///	@brief	Sets the name of the thread.
    ///	@param	name	[in] The new name for the thread.
    void setName(const String &name);

    ///	@brief	Returns the current name of the thread.
    ///	@return	The current name of the thread.
    const String &getName() const;

    ///	@brief	Waits until the thread is signaled by an event.
    ///	@param	ms		[in] The timeout, set to 0 for infinite.
    void waitForTimeout(ui32 ms);

    ///	@brief	Waits for infinity for a signal.
    void wait();

    ///	@brief	Returns a pointer showing to the thread event of this thread.
    ///	@return	A pointer showing to the thread event, equal NULL if nothing.
    ThreadEvent *getThreadEvent() const;

    ///	@brief	Assigns a new thread priority to the thread.
    ///	@param	prio	[in] The new thread prio.
    void setPriority(Priority prio);

    ///	@brief	Returns the current thread prio.
    ///	@return	The current thread prio.
    Priority getPriority() const;

    ///	@brief	The assigned name of the thread will be returned.
    ///	@return	The assigned name of the thread.
    const String &getThreadName() const;

    /// @brief  Returns the thread storage instance.
    /// @param  A pointer showing to the thread local storage instance.
    ThreadLocalStorage *getThreadLocalStorage();

    /// @brief  Will set the local thread storage instance.
    /// @param  tls     [in] The new lts instance.
    void setThreadLocalStorage(ThreadLocalStorage *tls);

    /// @brief  Will set the thread id.
    /// @param  id       [in] The new thread id.
    void setThreadId(const ThreadId &id);

    /// @brief  Returns the thread id.
    /// @return The thread id.
    ThreadId getThreadId();

protected:
    /// @brief  Hook to react on thread state changes.
    virtual void onNewState();

    ///	@brief	Overwrite this for your own thread running method.
    virtual i32 run();

    ///	@brief	The new state will be set.
    ///	@param	newState	[in] The new state.
    virtual void setState(ThreadState newState);

#ifdef OSRE_WINDOWS
    static ui32 WINAPI ThreadFunc(LPVOID data);
#else
    static int sdl2threadfunc( void *data );
#endif

    ///	Will assign a thread name.
    static void setThreadName(const c8 *pName);

private:
    ThreadState m_threadState;
    ThreadId m_id;
    ui32 m_stacksize;
    String mThreadName;
    Priority m_Prio;

#ifdef OSRE_WINDOWS
    HANDLE m_ThreadHandle;
    ThreadEvent *m_pThreadSignal;
#else
    SDL_Thread *m_thread;
    ThreadEvent *m_threadSignal;
#endif
    ThreadLocalStorage *mTls;
};

inline void Thread::setName( const String &name ) {
    mThreadName = name;
}

inline const String &Thread::getName() const {
    return mThreadName;
}

inline void Thread::onNewState() {
    // override me!
}

inline Thread::ThreadState Thread::getCurrentState() const {
    return m_threadState;
}

inline void Thread::setState(ThreadState newState) {
    if (m_threadState == newState) {
        return;
    }

    m_threadState = newState;
    onNewState();
}

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Platform::Win32ThreadLocalStorage
///	@ingroup	Engine
///
///	@brief This class implements the Win32-based Thread Local Storage API.
//-------------------------------------------------------------------------------------------------
class ThreadLocalStorage {
public:
    ThreadLocalStorage();
    ~ThreadLocalStorage();
    void *get();
    void set(void *data);

private:
#ifdef OSRE_WINDOWS
    DWORD m_index;
#else
    SDL_TLSID m_index;
#endif
};

class OSRE_EXPORT Mutex {
public:
    Mutex(ui32 timeout);
    ~Mutex();
    void lock();
    bool tryLock();
    bool unlock();

private:
#ifdef OSRE_WINDOWS
    HANDLE m_handle;
#else
    SDL_mutex *m_mutex;
#endif // _WIN32
    ui32 m_timeout;
};

class ThreadEvent {
public:
    ///	The constructor.
    ThreadEvent();
    ///	The destructor.
    ~ThreadEvent();
    ///	The event will be signaled.
    void signal();
    ///	Wait until the event is signaled.
    void waitForOne();
    ///	Waits for all.
    void waitForAll();
    ///	Wait until the event is signaled until a given timeout.
    void waitForTimeout(ui32 ms);
#ifdef OSRE_WINDOWS
    ///	The handle of the event will be returned.
    HANDLE getHandle() const;
#endif // _WIN32

private:
#ifdef OSRE_WINDOWS
    HANDLE m_EventHandle;
#else
    i32 m_bool;
    SDL_mutex *m_lock;
    SDL_cond *m_event;
#endif // _WIN32
};

#ifdef OSRE_WINDOWS
#include <intrin.h>

extern "C" long __cdecl _InterlockedCompareExchange(long volatile *a_vpDest, long a_Exchange, long a_Comp);
extern "C" long __cdecl _InterlockedExchange(long volatile *a_vpTarget, long a_Value);
extern "C" long __cdecl _InterlockedExchangeAdd(long volatile *a_vpAddend, long a_Value);
extern "C" long __cdecl _InterlockedIncrement(long volatile *a_vpAddend);
extern "C" long __cdecl _InterlockedDecrement(long volatile *a_vpAddend);

#pragma intrinsic(_InterlockedCompareExchange)
#pragma intrinsic(_InterlockedExchange)
#pragma intrinsic(_InterlockedExchangeAdd)
#pragma intrinsic(_InterlockedIncrement)
#pragma intrinsic(_InterlockedDecrement)

#endif

class OSRE_EXPORT AtomicInt {
public:
    AtomicInt(i32 val);

    /// @brief  The class destructor, virtual.
    ~AtomicInt();

    /// @brief  Increments the int.
    /// @param  value   [in] Increment value.
    void incValue(i32 value);

    /// @brief  Decrements the int.
    /// @param  value   [in] Decrement value.
    void decValue(i32 value);

    /// @brief  Will return the atomic in value.
    /// @return The value.
    i32 getValue();

    /// @brief  Increment by one.
    /// @return The new value
    i32 inc();

    /// @brief  Decrement by one.
    /// @return The new value
    i32 dec();

private:
#ifdef OSRE_WINDOWS
    mutable long m_value;
#else
    SDL_atomic_t m_value;
#endif
};

#ifdef OSRE_WINDOWS

inline AtomicInt::AtomicInt(i32 val) :
        m_value(val) {
    // empty
}

inline AtomicInt::~AtomicInt() {
    // empty
}

inline void AtomicInt::incValue(i32 value) {
    static_cast<void>(_InterlockedExchangeAdd(&m_value, value));
}

inline void AtomicInt::decValue(i32 value) {
    static_cast<void>(_InterlockedExchangeAdd(&m_value, -1 * value));
}

inline i32 AtomicInt::getValue() {
    return _InterlockedExchangeAdd(&m_value, 0);
}

inline i32 AtomicInt::inc() {
    return _InterlockedIncrement(&m_value);
}

inline i32 AtomicInt::dec() {
    return _InterlockedDecrement(&m_value);
}

#else

inline AtomicInt::AtomicInt( i32 value ) {
    SDL_AtomicSet( &m_value, value );
}

inline AtomicInt::~AtomicInt( ) {
    // empty
}

inline void AtomicInt::incValue( i32 value ) {
    SDL_AtomicAdd( &m_value, value );
}

inline void AtomicInt::decValue( i32 value ) {
    SDL_AtomicAdd( &m_value, -value );
}

inline i32 AtomicInt::getValue( ) {
    return ( SDL_AtomicGet( &m_value ) );
}

inline i32 AtomicInt::inc( ) {
    SDL_AtomicIncRef( &m_value );
    return getValue();
}

inline i32 AtomicInt::dec( ) {
    SDL_AtomicDecRef( &m_value );
    return getValue();
}

#endif

class ThreadFactory {
public:
    ThreadFactory();
    ~ThreadFactory();
    Thread *createThread(const String &name, ui32 stacksize);
    CriticalSection *createCriticalSection();
    ThreadEvent *createThreadEvent();
    AtomicInt *createAtomicInt(i32 val);
    ThreadLocalStorage *createThreadLocalStorage();
};

} // Namespace Platform
} // Namespace OSRE
