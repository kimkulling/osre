/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/Logger.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/Platform/Threading.h>

#include <cppcore/Container/TQueue.h>

#include <iostream>
#include <sstream>

namespace OSRE {
namespace Threading {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Infrastructure
///
///	@brief	This template class implements a thread-save queue.
//-------------------------------------------------------------------------------------------------
template <class T>
class TAsyncQueue {
public:
    ///	@brief	The constructor with the thread factory.
    TAsyncQueue();

    ///	@brief	The destructor, not virtual.
    ~TAsyncQueue();

    ///	@brief	A new item will be enqueued.
    ///	@param	rItem	The item to enqueue.
    void enqueue(const T &rItem);

    ///	@brief	The new item in the queue will be returned and removed from the list.
    ///	@return	The next item in the queue.
    T dequeue();

    ///	@brief	All enqueued items will be dequeued and stored in the list. The order of the items in
    ///			the queue will be not reordered.
    void dequeueAll(cppcore::TList<T> &rData);

    ///	@brief	The queue event will be signaled.
    void signalEnqueuedItem();

    ///	@brief	The queue awaits a signal.
    void awaitEnqueuedItem();

    ///	@brief	The number of stored items in the list will be returned.
    ///	@return	The number of enqueued items in the queue.
    size_t size();

    ///	@brief	Returns true, if the queue is empty.
    ///	@return	true, if no item was enqueued.
    bool isEmpty();

    ///	@brief	The queue will be cleared.
    void clear();

    /// Copying is not allowed.
    TAsyncQueue(const TAsyncQueue<T> &) = delete;
    TAsyncQueue &operator=(const TAsyncQueue<T> &) = delete;

private:
    Platform::CriticalSection *m_criticalSection;
    Platform::ThreadEvent *m_enqueueEvent;
    cppcore::TQueue<T> m_ItemQueue;
};

template <class T>
inline TAsyncQueue<T>::TAsyncQueue() :
        m_criticalSection(nullptr),
        m_enqueueEvent(nullptr),
        m_ItemQueue() {
    m_criticalSection = new Platform::CriticalSection;
    m_enqueueEvent = new Platform::ThreadEvent;
}

template <class T>
inline TAsyncQueue<T>::~TAsyncQueue() {
    cppcore::TList<T> dummy;
    dequeueAll(dummy);

    delete m_enqueueEvent;
    delete m_criticalSection;
}

template <class T>
inline void TAsyncQueue<T>::enqueue(const T &item) {
    osre_assert(nullptr != m_criticalSection);

    m_criticalSection->enter();

    m_ItemQueue.enqueue(item);

    m_criticalSection->leave();
    m_enqueueEvent->signal();
}

template <class T>
inline T TAsyncQueue<T>::dequeue() {
    osre_assert(nullptr != m_criticalSection);

    m_criticalSection->enter();

    osre_assert(!m_ItemQueue.isEmpty());
    T item;
    m_ItemQueue.dequeue(item);

    m_criticalSection->leave();

    return item;
}

template <class T>
inline void TAsyncQueue<T>::dequeueAll(cppcore::TList<T> &data) {
    osre_assert(nullptr != m_criticalSection);

    if (!data.isEmpty()) {
        data.clear();
    }

    while (!isEmpty()) {
        const T &ref(dequeue());
        m_criticalSection->enter();
        data.addBack(ref);
        m_criticalSection->leave();
    }
}

template <class T>
inline void TAsyncQueue<T>::signalEnqueuedItem() {
    osre_assert(nullptr != m_enqueueEvent);

    m_enqueueEvent->signal();
}

template <class T>
inline size_t TAsyncQueue<T>::size() {
    osre_assert(nullptr != m_criticalSection);

    m_criticalSection->enter();
    const size_t size = m_ItemQueue.size();
    m_criticalSection->leave();

    return size;
}

template <class T>
inline void TAsyncQueue<T>::awaitEnqueuedItem() {
    osre_assert(nullptr != m_enqueueEvent);

    if (m_ItemQueue.isEmpty()) {
        m_enqueueEvent->waitForOne();
    }
}

template <class T>
inline bool TAsyncQueue<T>::isEmpty() {
    osre_assert(nullptr != m_criticalSection);
    if (nullptr == m_criticalSection) {
        return false;
    }
    m_criticalSection->enter();
    bool res = m_ItemQueue.isEmpty();
    m_criticalSection->leave();

    return res;
}

template <class T>
inline void TAsyncQueue<T>::clear() {
    osre_assert(nullptr != m_criticalSection);

    m_criticalSection->enter();

    m_ItemQueue.clear();

    m_criticalSection->leave();
}

} // Namespace Threading
} // Namespace OSRE
