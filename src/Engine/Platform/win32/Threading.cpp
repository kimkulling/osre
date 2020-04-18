#include <osre/Debugging/osre_debugging.h>
#include <osre/Platform/SystemInfo.h>
#include <osre/Platform/Threading.h>

#include <process.h>

namespace OSRE {
namespace Platform {

static const c8 *Tag = "Threading.win32";

CriticalSection::CriticalSection() {
    OSRE_VALIDATE(InitializeCriticalSectionAndSpinCount(&m_CriticalSection, 1024), "Error while InitializeCriticalSectionAndSpinCount");
}

CriticalSection::~CriticalSection() {
    DeleteCriticalSection(&m_CriticalSection);
}

void CriticalSection::enter() {
    ::EnterCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_CriticalSection));
}

bool CriticalSection::tryEnter() {
    if (0 != ::TryEnterCriticalSection(&m_CriticalSection)) {
        return true;
    }

    return false;
}

void CriticalSection::leave() {
    ::LeaveCriticalSection(const_cast<LPCRITICAL_SECTION>(&m_CriticalSection));
}

Mutex::Mutex(ui32 timeout) :
        m_handle(nullptr), m_timeout(timeout) {
    m_handle = ::CreateMutex(NULL, FALSE, NULL);
}

Mutex::~Mutex() {
    ::CloseHandle(m_handle);
    m_handle = NULL;
}

void Mutex::lock() {
    OSRE_ASSERT(nullptr != m_handle);

    ::WaitForSingleObject(m_handle, INFINITE);
}

bool Mutex::tryLock() {
    OSRE_ASSERT(nullptr != m_handle);

    bool ok(false);
    if (::WaitForSingleObject(m_handle, m_timeout)) {
        ok = true;
    }

    return ok;
}

bool Mutex::unlock() {
    OSRE_ASSERT(nullptr != m_handle);

    bool ok(false);
    HRESULT hr = ::ReleaseMutex(m_handle);
    if (NULL != hr) {
        ok = true;
    }

    return ok;
}

Thread::Thread(const String &name, ui32 stacksize) :
        m_ThreadHandle(NULL),
        m_pThreadSignal(NULL),
        m_Prio(Priority::Normal),
        m_threadState(ThreadState::New),
        m_tls(nullptr),
        m_ThreadName(name),
        m_id(),
        m_stacksize(stacksize) {
    // empty
}

Thread::~Thread() {
    if (ThreadState::Running == m_threadState) {
        osre_debug(Tag, "Thread " + getName() + " is still running.");
        Thread::stop();
    }
}

bool Thread::start(void *pData) {
    if (ThreadState::Running == m_threadState || m_ThreadHandle) {
        osre_debug(Tag, "Thread " + getName() + " is already running.");
        return false;
    }

    if (!pData) {
        pData = this;
    }

    m_pThreadSignal = new ThreadEvent;
    m_ThreadHandle = (HANDLE)_beginthreadex(NULL,
            m_stacksize,
            Thread::ThreadFunc,
            pData,
            NULL,
            NULL);

    assert(NULL != m_ThreadHandle);
    setPriority(Priority::Normal);
    resume();
    setState(ThreadState::Running);

    return true;
}

bool Thread::stop() {
    if (ThreadState::Running != m_threadState) {
        osre_debug(Tag, "Thread " + getName() + " is not running.");
        return false;
    }

    // Stop the thread
    if (m_ThreadHandle != NULL) {
        ::CloseHandle(m_ThreadHandle);
        m_ThreadHandle = NULL;
    }

    // update state and release signal
    setState(ThreadState::Terminated);
    delete m_pThreadSignal;
    m_pThreadSignal = NULL;

    SystemInfo::unregisterThreadName(m_id);

    return true;
}

bool Thread::suspend() {
    // check for a valid thread state
    if (!m_ThreadHandle || ThreadState::Running == m_threadState) {
        osre_debug(Tag, "Thread " + getName() + " is not running.");
        return false;
    }

    // suspend the thread
    DWORD retCode = ::SuspendThread(m_ThreadHandle);
    if (-1 == retCode) {
        return false;
    }

    setState(ThreadState::Suspended);

    return true;
}

bool Thread::resume() {
    // check for a valid thread state
    if (!m_ThreadHandle || ThreadState::Waiting != m_threadState || ThreadState::New != m_threadState) {
        osre_debug(Tag, "Thread " + getName() + " is not suspended.");
        return false;
    }

    // resume the thread
    DWORD retCode = ::ResumeThread(m_ThreadHandle);
    if (-1 == retCode) {
        osre_error(Tag, "Error while try to resume thread.");
        return false;
    }

    setState(ThreadState::Running);

    return true;
}

void Thread::setName(const String &rName) {
    assert(!rName.empty());

    m_ThreadName = rName;
}

const String &Thread::getName() const {
    return m_ThreadName;
}

void Thread::waitForTimeout(ui32 ms) {
    if (!m_pThreadSignal) {
        osre_debug(Tag, "Invalid pointer to thread signal.");
        return;
    }

    m_pThreadSignal->waitForTimeout(ms);
}

void Thread::wait() {
    if (!m_pThreadSignal) {
        osre_debug(Tag, "Invalid pointer to thread signal.");
        return;
    }
    m_pThreadSignal->waitForOne();
}

ThreadEvent *Thread::getThreadEvent() const {
    return m_pThreadSignal;
}

void Thread::setPriority(Priority prio) {
    m_Prio = prio;
    BOOL result(TRUE);
    switch (m_Prio) {
        case Priority::Low:
            result = ::SetThreadPriority(m_ThreadHandle, THREAD_PRIORITY_BELOW_NORMAL);
            break;

        case Priority::Normal:
            result = ::SetThreadPriority(m_ThreadHandle, THREAD_PRIORITY_NORMAL);
            break;

        case Priority::High:
            result = ::SetThreadPriority(m_ThreadHandle, THREAD_PRIORITY_ABOVE_NORMAL);
            break;

        default:
            break;
    }

    if (!result) {
        osre_error(Tag, "Error while setting thread prio.");
    }
}

Thread::Priority Thread::getPriority() const {
    return m_Prio;
}

const String &Thread::getThreadName() const {
    return m_ThreadName;
}

ThreadLocalStorage *Thread::getThreadLocalStorage() {
    return m_tls;
}

void Thread::setThreadLocalStorage(ThreadLocalStorage *tls) {
    m_tls = (ThreadLocalStorage *)tls;
}

ui32 WINAPI Thread::ThreadFunc(LPVOID data) {
    Thread *thread = (Thread *)data;
    if (NULL == thread) {
        return 1;
    }

    setThreadName(thread->getName().c_str());
    ThreadId id = thread->getThreadId();
    id.Id = ::GetCurrentThreadId();
    SystemInfo::registerThreadName(id, thread->getName());
    thread->setThreadId(id);
    const i32 retCode(thread->run());

    return retCode;
}

void Thread::setThreadId(const ThreadId &id) {
    m_id = id;
}

ThreadId Thread::getThreadId() {
    return m_id;
}

i32 Thread::run() {
    // Override me!

    return 0;
}

void Thread::setThreadName(const c8 *name) {
    if (nullptr == name) {
        return;
    }

    struct THREADNAME_INFO {
        DWORD dwType; // must be 0x1000
        LPCSTR szName; // pointer to name (in user address space)
        DWORD dwThreadID; // thread ID (-1 = caller thread)
        DWORD dwFlags; // reserved for future use, must be zero
    };

    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = name;
    info.dwThreadID = ::GetCurrentThreadId();
    info.dwFlags = 0;
    __try {
        RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (const ULONG_PTR *)&info);
    } __except (EXCEPTION_CONTINUE_EXECUTION) {
        // Nothing to do!
    }
}

ThreadEvent::ThreadEvent() :
        m_EventHandle(nullptr) {
    m_EventHandle = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    assert(nullptr != m_EventHandle);
}

ThreadEvent::~ThreadEvent() {
    if (m_EventHandle) {
        ::CloseHandle(m_EventHandle);
        m_EventHandle = nullptr;
    }
}

void ThreadEvent::signal() {
    if (!m_EventHandle) {
        osre_debug(Tag, "Signal handle is not valid.");
        return;
    }

    static_cast<void>(::SetEvent(m_EventHandle));
}

void ThreadEvent::waitForOne() {
    static_cast<void>(::WaitForSingleObject(m_EventHandle, INFINITE));
}

void ThreadEvent::waitForAll() {
    static_cast<void>(::WaitForMultipleObjects(1, &m_EventHandle, TRUE, INFINITE));
}

void ThreadEvent::waitForTimeout(ui32 ms) {
    ui32 timeout = ms;
    if (0 == ms) {
        timeout = INFINITE;
    }
    static_cast<void>(::WaitForSingleObject(m_EventHandle, timeout));
}

HANDLE ThreadEvent::getHandle() const {
    return m_EventHandle;
}

ThreadLocalStorage::ThreadLocalStorage() :
        m_index(TLS_OUT_OF_INDEXES) {
    m_index = ::TlsAlloc();
    OSRE_ASSERT(TLS_OUT_OF_INDEXES != m_index);
}

ThreadLocalStorage::~ThreadLocalStorage() {
    OSRE_ASSERT(TLS_OUT_OF_INDEXES != m_index);

    ::TlsFree(m_index);
}

void *ThreadLocalStorage::get() {
    OSRE_ASSERT(TLS_OUT_OF_INDEXES != m_index);

    return ::TlsGetValue(m_index);
}

void ThreadLocalStorage::set(void *data) {
    OSRE_ASSERT(TLS_OUT_OF_INDEXES != m_index);

    ::TlsSetValue(m_index, data);
}

ThreadFactory::ThreadFactory() {
        //AbstractThreadFactory("platform/threadfactory") {
    // empty
}

ThreadFactory::~ThreadFactory() {
    // empty
}

Thread *ThreadFactory::createThread(const String &name, ui32 stacksize) {
    return new Thread(name, stacksize);
}

CriticalSection *ThreadFactory::createCriticalSection() {
    return new CriticalSection;
}

ThreadEvent *ThreadFactory::createThreadEvent() {
    return new ThreadEvent;
}

AtomicInt *ThreadFactory::createAtomicInt(i32 val) {
    return new AtomicInt(val);
}

ThreadLocalStorage *ThreadFactory::createThreadLocalStorage() {
    return new ThreadLocalStorage;
}

} // Namespace Platform
} // Namespace OSRE
