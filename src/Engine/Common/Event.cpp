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

#include "Common/Event.h"

namespace OSRE {
namespace Common {

Event::Event(const c8 *id) :
        mNumRefs(1),
        mHash(StringUtils::hashName(id)),
        mId(id),
        mEventData(nullptr) {
    // empty
}

void Event::setEventData(const EventData *pData) {
    mEventData = pData;
}

const EventData *Event::getEventData() const {
    return mEventData;
}

HashId Event::getHash() const {
    return mHash;
}

const String Event::getId() const {
    String tmp(mId);
    return tmp;
}

void Event::get() {
    ++mNumRefs;
}

void Event::release() {
    --mNumRefs;
    if (0 == mNumRefs) {
        delete this;
    }
}

bool Event::operator == (const Event &rhs) const {
    return (mHash == rhs.mHash);
}

EventData::EventData(const Event &e, EventTriggerer *c) :
        mEvent(e),
        mSource(c),
        mTimestamp(0.0),
        mNumRefs(1) {
    // empty
}

const Event &EventData::getEvent() const {
    return mEvent;
}

EventTriggerer *EventData::getEventSender() const {
    return mSource;
}

void EventData::get() {
    ++mNumRefs;
}

void EventData::release() {
    if (mNumRefs) {
        --mNumRefs;
        if (mNumRefs == 0) {
            delete this;
        }
    }
}

bool EventData::operator==(const EventData &other) const {
    return (mEvent == other.mEvent && mSource == other.mSource);
}

} // Namespace Common
} // Namespace OSRE
