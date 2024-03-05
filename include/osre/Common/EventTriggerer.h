/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/Event.h>
#include <cppcore/Container/TArray.h>
#include <map>
#include <list>

namespace OSRE {
namespace Common {

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
/// @brief	Base class to trigger events.
///
///	You can register a listener, which will get update callbacks if the related event was fired. 
///	You can also register a listener to a bundle of events. The event trigger will use a simple
///	functor as a listener implementation. 
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT EventTriggerer {
public:
    ///	@brief	The default class constructor.
    EventTriggerer();

    ///	@brief	The class destructor, virtual.
    virtual ~EventTriggerer();

    ///	@brief	Add an listener for an event
    ///	@param	ev	Reference to listened event
    ///	@param	func	Reference to called functor
    void addEventListener(const Event& ev, const EventFunctor &func );

    ///	@brief	Add an listener for an event
    ///	@param	ev	    Reference to listened event-array.
    ///	@param	func	Reference to called functor
    void addEventListener(const EventPtrArray &events, const EventFunctor &func);

    ///	@brief Remove an listener for an event
    ///	@param ev	Reference to event
    ///	@param func	Reference to functor
    void removeEventListener( const Event& ev, const EventFunctor& func);

    ///	@brief Remove an listener for an event
    ///	@param ev	Reference to event-array.
    ///	@param func	Reference to functor
    void removeEventListener(const EventPtrArray &events, const EventFunctor &func);

    ///	@brief Remove all listener from the trigger.
    ///	@param	ev	Event which triggers are connected to.
    void removeAllEventListeners(const EventPtrArray &events);

    ///	@brief Test, if this instance can trigger an event.
    ///	@return true, if instance can trigger
    bool isEventTriggerable(const Event& ev);

    ///	@brief	Adds a new event for triggering.
    ///	@param	ev		[in] The event to add.
    virtual void addTriggerableEvent( const Event& ev );
    
    ///	@brief	Triggers an event.
    ///	@param	ev		[in] The event to trigger.
    ///	@param	data	[in] The event data.
    virtual void triggerEvent( const Event& ev, const EventData* data );

    ///	@brief	Clears all added events.
    void clear();

private:
    using FunctorList = std::list<EventFunctor>;
    using FunctorMap = std::map<HashId, FunctorList>;
    FunctorMap mEventList;
};

} // Namespace Common
} // Namespace OSRE
