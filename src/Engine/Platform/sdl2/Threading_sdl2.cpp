#include <osre/Platform/Threading.h>
#include <osre/Platform/SystemInfo.h>
#include <osre/Debugging/osre_debugging.h>

#include <SDL.h>
#include <SDL_atomic.h>

namespace OSRE {
namespace Platform {

namespace {

static const c8 *Tag = "Threading_sdl2";

}

CriticalSection::CriticalSection() :
        m_spinlock( 0 ) {
    // empty
}

CriticalSection::~CriticalSection() {
    SDL_AtomicUnlock( &m_spinlock );
}

void CriticalSection::enter() {
    SDL_AtomicLock( &m_spinlock );
}

bool CriticalSection::tryEnter() {
    if( SDL_TRUE == SDL_AtomicTryLock( &m_spinlock ) ) {
        return true;
    }

    return false;
}

void CriticalSection::leave() {
    SDL_AtomicUnlock( &m_spinlock );
}

ThreadLocalStorage::ThreadLocalStorage() :
        m_index( 99999999 ) {
    // empty
}

ThreadLocalStorage::~ThreadLocalStorage() {
    // empty
}

void *ThreadLocalStorage::get() {
    OSRE_ASSERT( 99999999 != m_index );

    return ::SDL_TLSGet( m_index );
}

void ThreadLocalStorage::set( void *data ) {
    OSRE_ASSERT( 99999999 != m_index );

    ::SDL_TLSSet( m_index, data, 0 );
}

Thread::Thread( const String &name, ui32 stacksize ) :
        m_threadState( ThreadState::New ),
        m_id(),
        m_stacksize( stacksize ),
        mThreadName( name ),
        m_Prio( Priority::Normal ),
        m_thread( nullptr ),
        m_threadSignal( nullptr ),
        mTls( nullptr ) {
    // empty
}

Thread::~Thread( ) {
    if ( ThreadState::Running == Thread::getCurrentState() ) {
        osre_debug( Tag, "Thread " + getName() + " is still running." );
        Thread::stop( );
    }
}

bool Thread::start( void *data ) {
    if ( ThreadState::Running == Thread::getCurrentState() ) {
        osre_debug( Tag, "Thread " + getName() + " is already running." );
        return false;
    }
    if ( nullptr == data ) {
        data = this;
    }

    bool result( true );
    m_thread = SDL_CreateThread( Thread::sdl2threadfunc, getName( ).c_str( ), data );
    if( m_thread ) {
        setState( ThreadState::Running );
    } else {
        // cannot start thread, so it's terminated
        setState( ThreadState::Terminated );
        result = false;
    }

    return result;
}

bool Thread::stop( ) {
    if ( ThreadState::Running != Thread::getCurrentState()) {
        osre_debug( Tag, "Thread " + getName() + " is not running." );
        return false;
    }

    SDL_DetachThread( m_thread );
    m_thread = nullptr;

    // update state and release signal
    setState( ThreadState::Terminated );
    delete m_threadSignal;
    m_threadSignal = nullptr;

    SystemInfo::unregisterThreadName( getThreadId() );

    return true;
}

bool Thread::suspend( ) {
    // check for a valid thread state
    if ( ThreadState::Running == Thread::getCurrentState() ) {
        osre_debug( Tag, "Thread " + getName() + " is not running." );
        return false;
    }

    // suspend the thread
    setState( ThreadState::Suspended );

    return true;
}

bool Thread::resume( ) {
    // check for a valid thread state
    if ( ThreadState::Waiting != Thread::getCurrentState()) {
        osre_debug( Tag, "Thread " + getName() + " is not suspended." );
        return false;
    }

    // resume the thread
    setState( ThreadState::Running );

    return true;
}

void Thread::waitForTimeout( ui32 ms ) {
    if ( !m_threadSignal ) {
        osre_debug( Tag, "Invalid pointer to thread signal." );
        return;
    } else {
        m_threadSignal->waitForTimeout( ms );
    }
}

void Thread::wait( ) {
    if ( !m_threadSignal ) {
        osre_debug( Tag, "Invalid pointer to thread signal." );
        return;
    }

    m_threadSignal->waitForOne();
}

ThreadEvent *Thread::getThreadEvent( ) const {
    return m_threadSignal;
}

void Thread::setPriority( Priority prio ) {
    m_Prio = prio;
}

Thread::Priority Thread::getPriority( ) const {
    return m_Prio;
}

ThreadLocalStorage *Thread::getThreadLocalStorage() {
    return mTls;
}

void Thread::setThreadLocalStorage( ThreadLocalStorage *tls ) {
    mTls = ( ThreadLocalStorage* ) tls;
}

void Thread::setThreadId(const ThreadId &id) {
    m_id = id;
}

ThreadId Thread::getThreadId() {
    return m_id;
}

i32 Thread::sdl2threadfunc( void *data ) {
    i32 retCode( 0 );
    if( nullptr != data ) {
        Thread *instance = ( Thread* ) data;
        switch( instance->getPriority() ) {
            case Priority::Low:
                retCode = SDL_SetThreadPriority( SDL_THREAD_PRIORITY_LOW );
                break;
            case Priority::Normal:
                retCode = SDL_SetThreadPriority( SDL_THREAD_PRIORITY_NORMAL );
                break;
            case Priority::High:
                retCode = SDL_SetThreadPriority( SDL_THREAD_PRIORITY_HIGH );
                break;
            default:
                retCode = 1;
                break;
        }
        if( 0 == retCode ) {
            ThreadId id;
            id.Id = ( unsigned long ) SDL_ThreadID();
            instance->setThreadId( id );
            SystemInfo::registerThreadName( id, instance->getName() );
            retCode = instance->run();
        }
    } else {
        osre_error( Tag, "Invalid thread data." );
        retCode = 1;
    }

    return retCode;
}

i32 Thread::run( ) {
    // Override me!

    return 0;
}

ThreadEvent::ThreadEvent()
: m_bool( SDL_FALSE )
, m_lock( nullptr )
, m_event( nullptr ) {
    m_lock = SDL_CreateMutex();
    m_event = SDL_CreateCond();
}

ThreadEvent::~ThreadEvent( ) {
    SDL_DestroyCond( m_event );
    m_event = nullptr;

    SDL_DestroyMutex( m_lock );
    m_lock = nullptr;
}

void ThreadEvent::signal( ) {
    SDL_LockMutex( m_lock );
    m_bool = SDL_TRUE;
    SDL_CondSignal( m_event );
    SDL_UnlockMutex( m_lock );
}

void ThreadEvent::waitForOne( ) {
    m_bool = SDL_FALSE;
    SDL_LockMutex( m_lock );
    while( !m_bool ) {
        SDL_CondWait( m_event, m_lock );
    }
    SDL_UnlockMutex( m_lock );
}

void ThreadEvent::waitForAll() {
    m_bool = SDL_FALSE;
    SDL_LockMutex( m_lock );
    while (!m_bool) {
        SDL_CondWait( m_event, m_lock );
    }
    SDL_UnlockMutex( m_lock );
}


void ThreadEvent::waitForTimeout( ui32 ms ) {
    m_bool = SDL_FALSE;
    SDL_LockMutex( m_lock );
    while (!m_bool) {
        SDL_CondWaitTimeout( m_event, m_lock, ms );
    }
    SDL_UnlockMutex( m_lock );
}

Mutex::Mutex(ui32 timeout) :
        m_mutex( nullptr ),
        m_timeout(timeout) {
    m_mutex = SDL_CreateMutex();
}

Mutex::~Mutex() {
    SDL_DestroyMutex(m_mutex);
    m_mutex = nullptr;
}

void Mutex::lock() {
    OSRE_ASSERT(nullptr != m_mutex);

    static_cast<void>(SDL_LockMutex(m_mutex));
}

bool Mutex::tryLock() {
    OSRE_ASSERT(nullptr != m_mutex);

    bool ok = false;
    i32 status = SDL_TryLockMutex(m_mutex);
    if (status == 0) {
        ok = true;
    }

    return ok;
}

bool Mutex::unlock() {
    OSRE_ASSERT(nullptr != m_mutex);

    return 0 == SDL_UnlockMutex(m_mutex);
}

}
}
