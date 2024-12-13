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

#include "Common/Logger.h"
#include "Platform/Threading.h"
#include <cppcore/Container/TQueue.h>

namespace OSRE {
namespace Threading {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This template class implements a thread-save queue.
//-------------------------------------------------------------------------------------------------
template <class T>
class TAsyncQueue {
public:
    ///	@brief	The constructor with the thread factory.
    TAsyncQueue() = default;

    ///	@brief	The destructor, not virtual.
    ~TAsyncQueue();

    ///	@brief	A new item will be enqueued.
    ///	@param	item	The item to enqueue.
    void enqueue(const T &item);

    ///	@brief	The new item in the queue will be returned and removed from the list.
    ///	@return	The next item in the queue.
    T dequeue();

    ///	@brief	All enqueued items will be dequeued and stored in the list. The order of the items in
    ///			the queue will be not reordered.
    /// @param[out] data    List containing all items.
    void dequeueAll(cppcore::TList<T> &data);

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
    Platform::CriticalSection mCriticalSection;
    Platform::ThreadEvent mEnqueueEvent;
    cppcore::TQueue<T> mItemQueue;
};

template <class T>
inline TAsyncQueue<T>::~TAsyncQueue() {
    cppcore::TList<T> dummy;
    dequeueAll(dummy);
}

template <class T>
inline void TAsyncQueue<T>::enqueue(const T &item) {
    mCriticalSection.enter();

    mItemQueue.enqueue(item);

    mCriticalSection.leave();
    mEnqueueEvent.signal();
}

template <class T>
inline T TAsyncQueue<T>::dequeue() {
    T item = {};
    mCriticalSection.enter();

    if (!mItemQueue.isEmpty()) {
        mItemQueue.dequeue(item);
    }

    mCriticalSection.leave();

    return item;
}

template <class T>
inline void TAsyncQueue<T>::dequeueAll(cppcore::TList<T> &data) {
    if (!data.isEmpty()) {
        data.clear();
    }

    mCriticalSection.enter();
    while (!isEmpty()) {
        if (!mItemQueue.isEmpty()) {
            T item;
            mItemQueue.dequeue(item);
            data.addBack(std::move(item));
        }
        mCriticalSection.leave();
    }
}

template <class T>
inline void TAsyncQueue<T>::signalEnqueuedItem() {
    mEnqueueEvent.signal();
}

template <class T>
inline size_t TAsyncQueue<T>::size() {
    mCriticalSection.enter();
    const size_t size = mItemQueue.size();
    mCriticalSection.leave();

    return size;
}

template <class T>
inline void TAsyncQueue<T>::awaitEnqueuedItem() {
    if (mItemQueue.isEmpty()) {
        mEnqueueEvent.waitForOne();
    }
}

template <class T>
inline bool TAsyncQueue<T>::isEmpty() {
    mCriticalSection.enter();
    bool res = mItemQueue.isEmpty();
    mCriticalSection.leave();

    return res;
}

template <class T>
inline void TAsyncQueue<T>::clear() {
    mCriticalSection.enter();

    mItemQueue.clear();

    mCriticalSection.leave();
}

} // Namespace Threading
} // Namespace OSRE
