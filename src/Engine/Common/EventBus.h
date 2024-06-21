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

#include "Common/osre_common.h"

#include <cppcore/Container/TQueue.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {
namespace Common {

class AbstractEventHandler;
class EventTriggerer;
struct Event;
struct EventData;

struct Action {
    const Event     *mEvent;
    const EventData *mEventData;
    i32              mRetCode;

    Action() : mEvent(nullptr), mEventData(nullptr) {}
    virtual ~Action() = default;
    i32 getReturnCode() const {return mRetCode;}
};

struct QueueEntry {
    Action mAction;

    QueueEntry() = default;
    ~QueueEntry() = default;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief	This class is used to provide global published events to other subscriptions.
/// A publish suscripe model is in use to implement this. It works in the following way:
/// 
/// Sender -> broadcasring an event -> suscribed EventHandler 1
///                                 -> suscribed EventHandler 2
/// All event handlers, which has registered a suscribtion will get notified.
/// If no event handler has registered before the event will get lost.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT EventBus {
public:
    /// @brief  The class constructor.
    EventBus();

    ///	@brief  The class destructor.
    ~EventBus() = default;

    ///	@brief  Will create the event bus, allocator will get prepared.
    /// @return true if successful, false in case of an error.
    bool create();

    /// @brief  Returns the creation state of the event bus.
    /// @return true for created, false for not.
    bool isCreated() const;

    ///	@brief  Will destroy the event bus, all events will be releases.
    /// @return true if successful, false if not.
    bool destroy();

    /// @brief  Performs an update, all event-subscribers will get their callback.
    void update();
    
    ///	@brief  Will subscribe an event-handler to a given event type.
    /// @param[in] handler  The handler to subscribe.
    /// @param[in] ev       The event type.
    void subscribeEventHandler(AbstractEventHandler *handler, const Event &ev);
    
    /// @brief  Will unsubscribe an event-handler from a given event type.
    /// @param[in] handler  The handler to subscribe.
    /// @param[in] ev       The event type.
    void unsubscribeEventHandler(AbstractEventHandler *handler, const Event &ev);
    
    ///	@brief  Will publish an event with its data, all subscribers will get notified.
    /// @param[in] ev           The event type
    /// @param[in] eventData    The event data
    void publish(const Event &ev, const EventData *eventData);

    void registerAction();

    // No copying.
    EventBus &operator = (const EventBus &) = delete;
    EventBus(const EventBus &) = delete;

private:
    using EventHandlerArray = cppcore::TArray<AbstractEventHandler*>;
    using SuscribedHandler = cppcore::THashMap<HashId, EventHandlerArray*>;
    SuscribedHandler mSuscribedHandler;
    using QueueEntryArray = cppcore::TArray<QueueEntry*>;
    QueueEntryArray mQueueEntryArray;
    bool mCreated;
};

} // namespace Common
} // namespace OSRE
