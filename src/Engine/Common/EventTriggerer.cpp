/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/EventTriggerer.h>
#include <osre/Debugging/osre_debugging.h>

#include <algorithm>

namespace OSRE {
namespace Common {

using namespace ::std;
using namespace ::CPPCore;

EventTriggerer::EventTriggerer() 
: m_EventList() {
    // empty
}

EventTriggerer::~EventTriggerer() {	
    clear();
}

void EventTriggerer::addEventListener( const Event& ev, const EventFunctor& func ) {
    if ( 1 == m_EventList.count(ev.getHash() ) ) {
        m_EventList[ ev.getHash() ].push_back(func);
    }
}

void EventTriggerer::addEventListener( const TArray<const Event*> &events, const EventFunctor &func ) {
    if ( events.isEmpty() ) {
        return;
    }

    for ( ui32 i=0; i<events.size(); ++i ) {
        const Event &rEvent = *events[ i ];
        addEventListener( rEvent, func );
    }
}

void EventTriggerer::removeEventListener(const Event& ev, const EventFunctor& func) {
    if ( 1 == m_EventList.count(ev.getHash() ) )	{
        const ui32 id( ev.getHash() );
        FunctorList functorlist( m_EventList[ id ] );
        FunctorList::iterator it = find( functorlist.begin(), functorlist.end(), func );
        if ( it != functorlist.end() )	{
            functorlist.erase( it );
        }
    }
}

void EventTriggerer::removeEventListener( const TArray<const Event*> &events, const EventFunctor &func ) {
    if ( events.isEmpty() ) {
        return;
    }

    for ( ui32 i=0; i<events.size(); ++i ) {
        const Event &currentEvent = *events[ i ];
        removeEventListener( currentEvent, func );
    }
}

void EventTriggerer::removeAllEventListeners(const Event& ev) {
    m_EventList[ ev.getHash() ].clear();
}

bool EventTriggerer::isEventTriggerable(const Event& ev) {
    return (1 == m_EventList.count( ev.getHash() ) );
}

void EventTriggerer::addTriggerableEvent(const Event& ev) {
    if ( m_EventList.count( ev.getHash() ) < 1 ) {
        m_EventList[ ev.getHash() ].clear();
    }
}

void EventTriggerer::triggerEvent(const Event& ev, const EventData *data) {	
    OSRE_ASSERT( 0 != m_EventList.count( ev.getHash() ) );

    if ( 1 != m_EventList.count( ev.getHash() ) ) {
        return;
    }

    const ui32 &id( ev.getHash() );
    const FunctorList &functorlist( m_EventList[ id ] );
    for ( FunctorList::const_iterator it = functorlist.begin(); it !=functorlist.end(); ++it ) {
        (*it)(ev, data);
    }
}

void EventTriggerer::clear() {
    FunctorMap::iterator it( m_EventList.begin() );
    for ( ; it != m_EventList.end(); ++it ) {
        FunctorList &currentList( it->second );
        currentList.clear();
    }
    m_EventList.clear();
}

} // Namespace Common
} // Namespace OSRE
