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
#include <osre/Common/EventBus.h>
#include <osre/Common/Event.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/Common/AbstractEventHandler.h>
#include <cppcore/Memory/TStackAllocator.h>

namespace OSRE {
namespace Common {

CPPCore::TStackAllocator<QueueEntry> QueueEntryAlloctor;

static constexpr c8 Tag[] = "EventBus";

EventBus::EventBus() :
        mSuscribedHandler(), mQueueEntryArray(), mCreated(false) {
    // empty
}

EventBus::~EventBus() {
    // empty
}

bool EventBus::create() {
    if (mCreated) {
        return false;
    }

    mCreated = true;
    QueueEntryAlloctor.reserve(1000);
    
    return mCreated;
}

bool EventBus::isCreated() const {
    return mCreated;
}

bool EventBus::destroy() {
    if (!mCreated) {
        return false;
    }
    
    QueueEntryAlloctor.clear();
    mCreated = false;

    return true;
}

void EventBus::update() {
    osre_assert(mCreated);
    if (mQueueEntryArray.isEmpty()) {
        return;
    }

    for (ui32 i = 0; i < mQueueEntryArray.size(); ++i) {
        const QueueEntry *entry = mQueueEntryArray[i];
        if (nullptr == entry) {
            continue;
        }

        const ui32 id = entry->mEvent->getHash();
        if (mSuscribedHandler.hasKey(id)) {
            EventHandlerArray *ehArray = nullptr;
            mSuscribedHandler.getValue(id, ehArray);
            for (ui32 j = 0; j < ehArray->size(); ++j) {
                AbstractEventHandler *eh = (*ehArray)[j];
                eh->onEvent(*entry->mEvent, entry->mEventData);
            }
        }
    }
    mQueueEntryArray.resize(0);
    QueueEntryAlloctor.reset();
}

void EventBus::subscribeEventHandler(AbstractEventHandler *handler, const Event &ev) {
    osre_assert(mCreated);
    if (nullptr == handler) {
        return;
    }

    const ui32 id = ev.getHash();
    EventHandlerArray *ehArray = nullptr;
    if (mSuscribedHandler.hasKey(id)) {
        mSuscribedHandler.getValue(id, ehArray);            
    } else {
        ehArray = new EventHandlerArray;
        mSuscribedHandler.insert(id, ehArray);
    }
    osre_assert(nullptr != ehArray);
    ehArray->add(handler);
}

void EventBus::unsubscribeEventHandler(AbstractEventHandler *handler, const Event &ev) {
    osre_assert(mCreated);

    if (nullptr == handler) {
        return;
    }
    const ui32 id = ev.getHash();
    if (mSuscribedHandler.hasKey(id)) {
        EventHandlerArray *ehArray = nullptr;
        mSuscribedHandler.getValue(id, ehArray);
        for (ui32 i = 0; i < ehArray->size(); ++i) {
            if ((*ehArray)[i] == handler) {
                ehArray->remove(i);
            }
        }
    }
}

void EventBus::publish( const Event &ev, const EventData *eventData ) {
    osre_assert(mCreated);

    QueueEntry *entry = QueueEntryAlloctor.alloc(1);
    osre_assert(entry != nullptr);
    if (entry == nullptr) {
        osre_fatal(Tag, "CAnnot allocate any new Queue event entried.");
        return;
    }

    entry->mEvent = &ev;
    entry->mEventData = eventData;
    mQueueEntryArray.add(entry);
}

} // namespace Common
} // namespace OSRE
