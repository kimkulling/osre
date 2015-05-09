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

#include <osre/Platform/AbstractCriticalSection.h>
#include <osre/Platform/AbstractThreadEvent.h>
#include <osre/Platform/AbstractThreadFactory.h>
#include <osre/Common/Logger.h>

#include <cppcore/Container/TQueue.h>

#include <iostream>
#include <sstream>

namespace OSRE {
namespace Threading {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Threading::TAsyncQueue
///	@ingroup	Infrastructure
///
///	@brief	This template class implements a thread-save queue.
//-------------------------------------------------------------------------------------------------
template<class T>
class TAsyncQueue {
public:
    ///	@brief	The constructor with the thread factory.
    ///	@param	pThreadFactory	The thread factory.
    TAsyncQueue( Platform::AbstractThreadFactory *pThreadFactory );

    ///	@brief	The destructor, not virtual.
    ~TAsyncQueue();

    ///	@brief	A new item will be enqueued.
    ///	@param	rItem	The item to enqueue.
    void enqueue( const T &rItem );

    ///	@brief	The new item in the queue will be returned and removed from the list.
    ///	@return	The next item in the queue.
    T dequeue();
    
    ///	@brief	All enqueued items will be dequeued and stored in the list. The order of the items in
    ///			the queue will be not reordered.
    void dequeueAll( CPPCore::TList<T> &rData );
    
    ///	@brief	The queue event will be signaled.
    void signalEnqueuedItem();

    ///	@brief	The queue awaits a signal.
    void awaitEnqueuedItem();

    ///	@brief	The number of stored items in the list will be returned.
    ///	@return	The number of enqueued items in the queue.
    ui32 size();

    ///	@brief	Returns true, if the queue is empty.
    ///	@return	true, if no item was enqueued.
    bool isEmpty();

    ///	@brief	The queue will be cleared.
    void clear();

private:
    /// Copying is not allowed.
    TAsyncQueue( const TAsyncQueue<T> & );
    TAsyncQueue &operator = ( const TAsyncQueue<T> & );

private:
    Platform::AbstractCriticalSection *m_pCriticalSection;
    Platform::AbstractThreadEvent *m_pEnqueueEvent;
    CPPCore::TQueue<T> m_ItemQueue;
};

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TAsyncQueue<T>::TAsyncQueue( Platform::AbstractThreadFactory *pThreadFactory )
: m_pCriticalSection( nullptr )
, m_pEnqueueEvent( nullptr )
, m_ItemQueue() {
    assert( nullptr != pThreadFactory );
  
    m_pCriticalSection = pThreadFactory->createCriticalSection();
    m_pEnqueueEvent = pThreadFactory->createThreadEvent();
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TAsyncQueue<T>::~TAsyncQueue() {
    CPPCore::TList<T> dummy;
    dequeueAll( dummy );
    
    delete m_pEnqueueEvent;
    m_pEnqueueEvent = nullptr;

    delete m_pCriticalSection;
    m_pCriticalSection = nullptr;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
void TAsyncQueue<T>::enqueue( const T &item ) {
    assert( nullptr != m_pCriticalSection );

    m_pCriticalSection->enter();
    
    m_ItemQueue.enqueue( item );
    
    m_pCriticalSection->leave();
    m_pEnqueueEvent->signal();
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TAsyncQueue<T>::dequeue() {
    assert( nullptr != m_pCriticalSection );
    
    m_pCriticalSection->enter();

    assert( !m_ItemQueue.isEmpty() );
    T item;	
    m_ItemQueue.dequeue( item );
    
    m_pCriticalSection->leave();

    return item;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
void TAsyncQueue<T>::dequeueAll( CPPCore::TList<T> &data ) {
    assert( nullptr != m_pCriticalSection );

    if( !data.isEmpty( ) ) {
        data.clear( );
    }

    while ( !isEmpty() ) {
        const T &ref( dequeue() );
        m_pCriticalSection->enter();
        data.addBack( ref );
        m_pCriticalSection->leave();
    }
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
void TAsyncQueue<T>::signalEnqueuedItem() {
    assert( nullptr != m_pEnqueueEvent );

    m_pEnqueueEvent->signal();
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
ui32 TAsyncQueue<T>::size() {
    assert( nullptr != m_pCriticalSection );

    m_pCriticalSection->enter( );
    const ui32 size = m_ItemQueue.size();
    m_pCriticalSection->leave();

    return size;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
void TAsyncQueue<T>::awaitEnqueuedItem() {
    assert( nullptr != m_pEnqueueEvent );

    if ( m_ItemQueue.isEmpty() ) {
        m_pEnqueueEvent->wait();
    }
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
bool TAsyncQueue<T>::isEmpty() {
    assert( nullptr != m_pCriticalSection );

    m_pCriticalSection->enter();
    bool res = m_ItemQueue.isEmpty();
    m_pCriticalSection->leave();

    return res;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
void TAsyncQueue<T>::clear() {
    assert( nullptr != m_pCriticalSection );

    m_pCriticalSection->enter();
    
    m_ItemQueue.clear();
    
    m_pCriticalSection->leave();
}

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2
