/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Common/AbstractEventHandler.h"
#include "Common/Event.h"
#include "Debugging/osre_debugging.h"
#include "Platform/Threading.h"
#include "Threading/SystemTask.h"
#include "Threading/TAsyncQueue.h"
#include "Threading/TaskJob.h"

#include <sstream>

namespace OSRE {
namespace Threading {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;

TaskJob *TaskJobAlloc(const Event *ev, const EventData *eventData) {
    return new TaskJob(ev, eventData);
}

DECL_EVENT(OnStopSystemTaskEvent);

struct OSRE_EXPORT StopSystemTaskEventData : public Common::EventData {
public:
    StopSystemTaskEventData() :
            EventData(OnStopSystemTaskEvent, nullptr) {
        // empty
    }
};

static const c8 *Tag = "SystemTaskThread";

static bool DebugQueueSize = false;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
//#ifdef OSRE_WINDOWS
class SystemTaskThread : public Thread {
public:
    enum {
        StackSize = 4096
    };

    SystemTaskThread(const String &threadName, TAsyncQueue<const TaskJob *> *jobQueue) :
            Thread(threadName, StackSize),
            m_updateEvent(nullptr),
            m_stopEvent(nullptr),
            m_activeJobQueue(jobQueue),
            m_eventHandler(nullptr) {
        osre_assert(nullptr != jobQueue);

        m_updateEvent = new ThreadEvent();
        m_stopEvent = new ThreadEvent();
    }

    ~SystemTaskThread() {
        if (m_eventHandler) {
            m_eventHandler->detach(nullptr);
        }

        delete m_updateEvent;
        m_updateEvent = nullptr;

        delete m_stopEvent;
        m_stopEvent = nullptr;
    }

    void setEventHandler(AbstractEventHandler *eventHandler) {
        m_eventHandler = eventHandler;
        if (nullptr != m_eventHandler) {
            m_eventHandler->attach(nullptr);
        }

        if (nullptr == eventHandler && nullptr != m_eventHandler) {
            m_eventHandler->detach(nullptr);
        }
    }

    virtual bool stop() {
        Thread::setState(ThreadState::Terminated);

        return true;
    }

    Common::AbstractEventHandler *getEventHandler() const {
        return m_eventHandler;
    }

    void setActiveJobQueue(Threading::TAsyncQueue<const TaskJob *> *pJobQueue) {
        m_activeJobQueue = pJobQueue;
    }

    Threading::TAsyncQueue<const TaskJob *> *getActiveJobQueue() const {
        return m_activeJobQueue;
    }

    Platform::ThreadEvent *getUpdateEvent() const {
        return m_updateEvent;
    }

    Platform::ThreadEvent *getStopEvent() const {
        return m_stopEvent;
    }

protected:
    i32 run() override {
        osre_assert(nullptr != m_activeJobQueue);

        osre_debug(Tag, "SystemThread::run");
        bool running = true;
        while (running) {
            m_activeJobQueue->awaitEnqueuedItem();
            while (!m_activeJobQueue->isEmpty()) {
                // for debugging
                if (DebugQueueSize) {
                    size_t size = m_activeJobQueue->size();
                    std::stringstream stream;
                    stream << "queue size = " << size << std::endl;
                    osre_debug(Tag, stream.str());
                }

                const TaskJob *job = m_activeJobQueue->dequeue();
                const Common::Event *ev = job->getEvent();
                if (nullptr == ev) {
                    running = false;
                    osre_assert(nullptr != ev);
                    continue;
                }

                if (OnStopSystemTaskEvent == *ev) {
                    osre_debug(Tag, "stop requested.");
                    running = false;
                }

                if (m_eventHandler) {
                    m_eventHandler->onEvent(*ev, job->getEventData());
                }
            }

            if (m_updateEvent) {
                m_updateEvent->signal();
            }
        }

        if (m_stopEvent) {
            m_stopEvent->signal();
        }

        return 0;
    }

private:
    Platform::ThreadEvent *m_updateEvent;
    Platform::ThreadEvent *m_stopEvent;
    Threading::TAsyncQueue<const TaskJob *> *m_activeJobQueue;
    Common::AbstractEventHandler *m_eventHandler;
};

SystemTask::SystemTask(const String &taskName) :
        AbstractTask(taskName),
        m_workingMode(Async),
        m_buffermode(SingleBuffer),
        m_taskThread(nullptr),
        m_asyncQueue(nullptr) {
    // empty
}

SystemTask::~SystemTask() {
    osre_assert(!isRunning());
}

void SystemTask::setWorkingMode(AbstractTask::WorkingMode mode) {
    if (isRunning()) {
        osre_error(Tag, "The working mode cannot be chanced in a running task.");
        return;
    }

    m_workingMode = mode;
}

AbstractTask::WorkingMode SystemTask::getWorkingMode() const {
    return m_workingMode;
}

void SystemTask::setBufferMode(BufferMode buffermode) {
    m_buffermode = buffermode;
}

SystemTask::BufferMode SystemTask::getBufferMode() const {
    return m_buffermode;
}

bool SystemTask::start(Thread *pThread) {
    // ensure task is not running
    if (nullptr != m_taskThread) {
        if (Thread::ThreadState::Running == m_taskThread->getCurrentState()) {
            osre_debug(Tag, "Task " + Object::getName() + " is already running.");
            return false;
        }
    }

    // setup the thread context
    m_asyncQueue = new Threading::TAsyncQueue<const TaskJob*>();
    if (!pThread) {
        m_taskThread = new SystemTaskThread(Object::getName() + ".thread", m_asyncQueue);
    } else {
        m_taskThread = reinterpret_cast<SystemTaskThread *>(pThread);
    }

    // start the system task
    return (m_taskThread->start(nullptr));
}

bool SystemTask::stop() {
    if ( Thread::ThreadState::Running != m_taskThread->getCurrentState()) {
        osre_debug(Tag, "Task " + getName() + " is not running.");
        return false;
    }

    bool res(false);
    ThreadEvent *stopEvent = m_taskThread->getStopEvent();
    sendEvent(&OnStopSystemTaskEvent, nullptr);

    if (nullptr != stopEvent) {
        stopEvent->waitForOne();
        m_taskThread->stop();
        delete m_taskThread;
        m_taskThread = nullptr;
        res = true;
    }

    return res;
}

bool SystemTask::isRunning() const {
    if (nullptr != m_taskThread) {
        return (Thread::ThreadState::Running == m_taskThread->getCurrentState());
    }

    return false;
}

bool SystemTask::execute() {
    return true;
}

void SystemTask::setThreadInstance(Thread *threadInstance) {
    if (nullptr != m_taskThread) {
        if (Platform::Thread::ThreadState::Running == m_taskThread->getCurrentState()) {
            osre_debug(Tag, "Stopping SystemTask before setting new thread.");
            SystemTask::stop();
        }
    }

    m_taskThread = reinterpret_cast<SystemTaskThread *>(threadInstance);
}

void SystemTask::attachEventHandler(AbstractEventHandler *eventHandler) {
    osre_assert(nullptr != m_taskThread);

    m_taskThread->setEventHandler(eventHandler);
}

void SystemTask::detachEventHandler() {
    osre_assert(nullptr != m_taskThread);
    if (nullptr == m_taskThread) {
        osre_debug(Tag, "TaskThread is nullptr.");
        return;
    }

    Common::AbstractEventHandler *eh = m_taskThread->getEventHandler();
    if (nullptr != eh) {
        m_taskThread->setEventHandler(nullptr);
    }
}

bool SystemTask::sendEvent(const Event *ev, const EventData *eventData) {
    osre_assert(nullptr != m_asyncQueue);
    osre_assert(nullptr != ev);

    TaskJob *taskJob = TaskJobAlloc(ev, eventData);
    m_asyncQueue->enqueue(taskJob);

    return true;
}

size_t SystemTask::getEvetQueueSize() const {
    osre_assert(nullptr != m_asyncQueue);

    return m_asyncQueue->size();
}

void SystemTask::onUpdate() {
    if (m_taskThread == nullptr) {
        return;
    }

    if (m_asyncQueue == nullptr) {
        m_asyncQueue = m_taskThread->getActiveJobQueue();
    }
}

void SystemTask::awaitUpdate() {
    osre_assert(nullptr != m_taskThread);

    if (nullptr != m_taskThread) {
        ThreadEvent *threadEvent = m_taskThread->getUpdateEvent();
        if (nullptr != threadEvent) {
            threadEvent->waitForOne();
        }
    }
}

void SystemTask::awaitStop() {
    if (nullptr != m_taskThread) {
        ThreadEvent *threadEvent = m_taskThread->getStopEvent();
        if (nullptr != threadEvent) {
            threadEvent->waitForOne();
        }
    }
}

SystemTask *SystemTask::create(const String &taskName) {
    return new SystemTask(taskName);
}

} // Namespace Threading
} // Namespace OSRE
