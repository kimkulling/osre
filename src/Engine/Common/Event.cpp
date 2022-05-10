/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/Event.h>

namespace OSRE {
namespace Common {

Event::Event(const c8 *id) :
        m_numRefs(1),
        m_hash(StringUtils::hashName(id)),
        mId(id),
        m_eventData(nullptr) {
    // empty
}

Event::~Event() {
    // empty
}

void Event::setEventData(const EventData *pData) {
    m_eventData = pData;
}

const EventData *Event::getEventData() const {
    return m_eventData;
}

ui32 Event::getHash() const {
    return m_hash;
}

const String Event::getId() const {
    String tmp(mId);
    return tmp;
}

void Event::get() {
    ++m_numRefs;
}

void Event::release() {
    --m_numRefs;
    if (0 == m_numRefs) {
        delete this;
    }
}

bool Event::operator == (const Event &rhs) const {
    return (m_hash == rhs.m_hash);
}

EventData::EventData(const Event &e, EventTriggerer *c) :
        m_Event(e),
        m_Source(c),
        m_timestamp(0.0),
        m_numRefs(1) {
    // empty
}

EventData::~EventData() {
    // empty
}

const Event &EventData::getEvent() const {
    return m_Event;
}

EventTriggerer *EventData::getEventSender() const {
    return m_Source;
}

void EventData::get() {
    ++m_numRefs;
}

void EventData::release() {
    if (m_numRefs) {
        --m_numRefs;
        if (0 == m_numRefs) {
            delete this;
        }
    }
}

bool EventData::operator==(const EventData &other) const {
    return (m_Event == other.m_Event && m_Source == other.m_Source);
}

} // Namespace Common
} // Namespace OSRE
