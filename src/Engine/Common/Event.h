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

#include "Common/StringUtils.h"
#include "Common/TFunctor.h"

// Container
#include <cppcore/Container/TArray.h>
#include <cppcore/Container/TList.h>

namespace OSRE {
namespace Common {

// Forward declarations ---------------------------------------------------------------------------
class EventTriggerer;

struct Event;
struct EventData;

//-------------------------------------------------------------------------------------------------
///	@brief	Event type declaration helper macro. 
/// 
/// This is a shortcut to define global events like Windows messages.
//-------------------------------------------------------------------------------------------------
#define DECL_EVENT(NAME) const Common::Event NAME(#NAME)

//-------------------------------------------------------------------------------------------------
///	@brief	The base class of osre-events.
/// 
/// Describes the type of an event. To create your events it is possible to build derives 
/// from the base class.
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT Event {
    friend class EventTriggerer;

    ///	@brief	The class constructor with description id.
    ///	@param	id	Id description, will be used to describe the event type.
    Event(const c8 *id);

    ///	@brief The default class destructor, virtual.
    virtual ~Event() = default;

    ///	@brief	Assigns a new event data instance to the event instance.
    ///	@param	pData	Pointer to event data instance.
    void setEventData(const EventData *pData);

    ///	@brief	Event data getter.
    ///	@return	Pointer to assigned event data instance.
    const EventData *getEventData() const;

    /// @brief  Returns the hash id of the event id.
    /// @return The hash id.
    HashId getHash() const;

    const String getId() const;

    ///	@brief	A reference ownership will be marked.
    void get();

    ///	@brief	An owned reference will be released.
    void release();

    ///	@brief	Implementation of == operator.
    ///	@param	other	Instance to compare.
    ///	@return	true, if both instances are equal, else false.
    bool operator==(const Event &other) const;

    // No copying
    Event(const Event &) = delete;
    Event &operator=(const Event &) = delete;

    ui32 mNumRefs;
    HashId mHash;
    const c8 *mId;
    const EventData *mEventData;
};

//--------------------------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	The base class for event specific data. For instance if you get a mouse event from your
///	input device the assigned event-data-instance will hold the assigned data like mouse button
///	clicked or the current position of the mouse cursor. Each event data instance holds a reference
///	to its assigned event-instance.
//--------------------------------------------------------------------------------------------------------------------
struct OSRE_EXPORT EventData {
    ///	@brief	The class constructor with event and trigger.
    ///	@param	ev		[in] Owning event.
    ///	@param	sender	[in] Event-triggering instance.
    EventData(const Event &ev, EventTriggerer *sender);

    ///	@brief	The class copy constructor.
    ///	@param	other	Instance to copy from.

    ///	@brief The default class destructor, virtual.
    virtual ~EventData() = default;

    ///	@brief	Returns assigned event.
    ///	@return	Assigned event instance.
    const Event &getEvent() const;

    ///	@brief	Returns event-trigger instance.
    ///	@return	Pointer to assigned event instance.
    EventTriggerer *getEventSender() const;

    ///	@brief  Adds another reference ownership.
    void get();

    ///	@brief  Releases reference ownership, if no owners are there data will be deleted..
    void release();

    ///	@brief	Equal operator implementation.
    ///	@param	rhs	Instance to compare.
    ///	@return	true, if both instances are equal.
    bool operator == (const EventData &rhs) const;

    // removed functions
    EventData() = delete;
    EventData(const EventData &) = delete;
    EventData &operator=(const EventData &) = delete;

private:
    const Event &mEvent;
    EventTriggerer *mSource;
    d32 mTimestamp;
    ui32 mNumRefs;
    void *mPayload;
};

///	Base event functor.
using EventFunctor = Functor<void, const Event &, const EventData *>;

using EventDataList = cppcore::TList<Common::EventData *>;
using EventPtrArray = cppcore::TArray<const Common::Event *>;

} // Namespace Common
} // Namespace OSRE
