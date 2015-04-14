#pragma once

/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/Types.h>

namespace OSRE {
    namespace Common {

class Event;
class EventData;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Common::AbstractEventHandler
///	@ingroup	Infrastructure
///
///	@brief This abstract interface declares the way how events should be handled.
/// 
/// An event-handler is the way a user can set his own behavior of a task. Event sent to a task
/// will be marshaled to all assigned event handlers, which is a derived class of IEventHandler.
//-------------------------------------------------------------------------------------------------
class AbstractEventHandler {
public:
    ///	@brief	The class destructor, virtual.
    virtual ~AbstractEventHandler();

    /// @brief  Will be called when event handler is attached to a event sink.
    /// @param  eventData   [in] The event data.
    virtual bool attach( const Common::EventData *eventData );

    /// @brief  Will be called when event handler is detached from a event sink.
    /// @param  eventData   [in] The event data.
    virtual bool detach( const Common::EventData *eventData );

    ///	@brief	Override this method for your own event handling functions.
    ///	@param	ev		    [in] The event.
    ///	@param	eventData   [in] A pointer showing to the event data.
    ///	@return	Will return true, if the event was handled, false if not.
    virtual bool onEvent( const Event &ev, const EventData *eventData ) = 0;

protected:
    ///	@brief	The default class constructor.
    AbstractEventHandler();

    /// @brief  Will be called when event handler is attached to a task.
    ///	@param	eventData	[in] A pointer showing to the event data.
    ///	@return	Will return true, if the event was handled, false if not.
    virtual bool onAttached( const EventData *eventData ) = 0;
    
    /// @brief  Will be called when the event handler is detached from a task
    ///	@param	eventData	[in] A pointer showing to the event data.
    ///	@return	Will return true, if the event was handled, false if not.
    virtual bool onDetached( const EventData *eventData ) = 0;
};

//-------------------------------------------------------------------------------------------------
inline
AbstractEventHandler::AbstractEventHandler() {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline
AbstractEventHandler::~AbstractEventHandler() {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline
bool AbstractEventHandler::attach( const EventData *eventData ) {
    return onAttached( eventData );
}

//-------------------------------------------------------------------------------------------------
inline
bool AbstractEventHandler::detach( const EventData *eventData ) {
    return onDetached( eventData );
}

//-------------------------------------------------------------------------------------------------

} // Namespace Common
} // Namespace ZFXCE2

