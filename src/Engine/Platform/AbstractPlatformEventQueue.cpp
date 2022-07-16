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

#include <osre/Platform/AbstractPlatformEventQueue.h>
#include <osre/Common/EventBus.h>

namespace OSRE {
namespace Platform {

using namespace OSRE::Common;

AbstractPlatformEventQueue::AbstractPlatformEventQueue() :
        Object("Platform/AbstractPlatformEventQueue"),
        mActiveList(0),
        mRenderBackendSrv(nullptr),
        mEventBus(nullptr) {
    mEventBus = new Common::EventBus;
    mEventBus->create();
}

AbstractPlatformEventQueue::~AbstractPlatformEventQueue() {
    mEventBus->destroy();
    delete mEventBus;
}

void AbstractPlatformEventQueue::processEvents(Common::EventTriggerer *triggerer) {
    Common::EventDataList *theList = getActiveEventDataList();
    if (nullptr == theList) {
        return;
    }

    EventBus *eventBus = getEventBus();
    osre_assert(nullptr != eventBus);
    while (!theList->isEmpty()) {
        Common::EventData *eventData = theList->front();
        if (nullptr != eventData) {
            eventBus->publish(eventData->getEvent(), eventData);
            triggerer->triggerEvent(eventData->getEvent(), eventData);
            theList->removeFront();
            eventData->release();
        }
    }
    switchEventDataList();
    eventBus->update();
}

Common::EventDataList *AbstractPlatformEventQueue::getActiveEventDataList() {
    Common::EventDataList *activeEventQueue(&m_eventQueues[mActiveList]);
    return activeEventQueue;
}

Common::EventDataList *AbstractPlatformEventQueue::getPendingEventDataList() {
    ui32 queueToProcess = (mActiveList + 1) % numEventQueues;
    m_eventQueues[queueToProcess].clear();
    Common::EventDataList *pendingEventQueue(&m_eventQueues[queueToProcess]);

    return pendingEventQueue;
}

void AbstractPlatformEventQueue::switchEventDataList() {
    mActiveList = (mActiveList + 1) % numEventQueues;
}

void AbstractPlatformEventQueue::setRenderBackendService(RenderBackend::RenderBackendService *rbSrv) {
    mRenderBackendSrv = rbSrv;
}

RenderBackend::RenderBackendService *AbstractPlatformEventQueue::getRenderBackendService() const {
    return mRenderBackendSrv;
}

void AbstractPlatformEventQueue::enqueueEvent(const Common::Event &ev, Common::EventData *data) {
    osre_assert(nullptr != data);

    if (nullptr != data) {
        if (ev == data->getEvent()) {
            getActiveEventDataList()->addBack(data);
        }
    }
}

} // namespace Platform
} // namespace OSRE
