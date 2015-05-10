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

#include <osre/Common/TFunctor.h>
#include <osre/Common/StringUtils.h>

#include <cassert>

namespace OSRE {
    namespace Common {

class EventTriggerer;
class Event;
class EventData;

//-------------------------------------------------------------------------------------------------
///	@def		::ZFXCE2::Common::DECL_EVENT
///	@ingroup	Infrastructure
///
///	@brief	Event type declaration helper macro. This is a shortcut to define global events like 
///	windows messages.
//-------------------------------------------------------------------------------------------------
#define DECL_EVENT( NAME ) const Common::Event NAME ( #NAME )
    
//-------------------------------------------------------------------------------------------------
/// @class		::ZFXCE2::Common::Event
///	@ingroup	Infrastructure
///
///	@brief	The base class of zfxce2-events, describes type of an event. To create your own events
///	it is possible to build derives from the base class.
//-------------------------------------------------------------------------------------------------
class Event {
    friend class EventTriggerer;

public:
    ///	@brief	the class constructor with description id.
    ///	@param	id	Id description, will be used to describe the event type.
    Event( const String &id );

    ///	@brief The class destructor, virtual.
    virtual ~Event();
    
    ///	@brief	Assigns a new event data instance to the event instance.
    ///	@param	pData	Pointer to event data instance.
    void setEventData( const EventData *pData);

    ///	@brief	Event data getter.
    ///	@return	Pointer to assigned event data instance.
    const EventData *getEventData() const;
    
    ///	@brief	Returns event ID.
    ///	@return	Const reference to id.
    const String &getIDAsStr() const;

    /// @brief  Returns the hash id of the event id.
    /// @return The hash id.
    ui32 getHash() const;

    ///	@brief	A reference ownership will be marked.
    void get();

    ///	@brief	An owned reference will be released.
    void release();

    ///	@brief	Implementation of == operator.
    ///	@param	other	Instance to compare.
    ///	@return	true, if both instances are equal, else false.
    bool operator == (const Event &other) const;

private:
    Event( const Event & );
    Event &operator = ( const Event & );

private:
    ui32 m_NumRefs;
    String m_ID;
    HashId m_hash;
    const EventData *m_pEventData;
};

//--------------------------------------------------------------------------------------------------------------------
inline 
Event::Event( const String &id )
: m_NumRefs( 1 )
, m_ID( id )
, m_hash( StringUtils::hashName( id.c_str() ) )
, m_pEventData( nullptr ) {
    // empty
}

//--------------------------------------------------------------------------------------------------------------------
inline 
Event::~Event() {
    // empty
}

//--------------------------------------------------------------------------------------------------------------------
inline 
void Event::setEventData( const EventData *pData ) {
    m_pEventData = pData;
}

//--------------------------------------------------------------------------------------------------------------------
inline 
const EventData *Event::getEventData() const {
    return m_pEventData;
}

//--------------------------------------------------------------------------------------------------------------------
inline 
const String &Event::getIDAsStr() const {
    return m_ID;
}

//--------------------------------------------------------------------------------------------------------------------
inline
ui32 Event::getHash() const {
    return m_hash;
}

//--------------------------------------------------------------------------------------------------------------------
inline 
void Event::get() {
    ++m_NumRefs;
}

//--------------------------------------------------------------------------------------------------------------------
inline 
void Event::release() {
    --m_NumRefs;
    if ( 0 == m_NumRefs ) {
        delete this;
    }
}

//--------------------------------------------------------------------------------------------------------------------
inline 
bool Event::operator == ( const Event &rhs ) const  {
    return (m_ID == rhs.m_ID); 
}

//--------------------------------------------------------------------------------------------------------------------
///	@class		ZFXCE::Common::EventData
///	@ingroup	Infrastructure
///
///	@brief	The base class for event specific data. For instance if you get a mouse event from your
///	input device the assigned event-data-instance will hold the assigned data like mouse button 
///	clicked or the current position of the mouse cursor. Each event data instance holds a reference 
///	to its assigned event-instance.
//--------------------------------------------------------------------------------------------------------------------
class EventData {
public:
    ///	@brief	The class constructor with event and trigger.
    ///	@param	rEvent		Owning event.
    ///	@param	pTriggerer	Event-triggering instance.
    EventData( const Event& rEvent, EventTriggerer *pTriggerer );
    
    ///	@brief	The class copy constructor.
    ///	@param	other	Instance to copy from.
    EventData( const EventData &other );
    
    ///	@brief The class destructor, virtual.
    virtual ~EventData();

    ///	@brief	Copy an instance.
    ///	@param	other	Instance to copy from.
    void copy( const EventData &other );

    ///	@brief	Returns assigned event.
    ///	@return	Assigned event instance.
    const Event& getEvent() const;

    ///	@brief	Returns event-trigger instance.
    ///	@return	Pointer to assigned event instance.
    EventTriggerer* getEventSource() const;

    void get();
    void release();

    ///	@brief	Equal operator implementation.
    ///	@param	other	Instance to compare.
    ///	@return	true, if both instances are equal.
    bool operator == (const EventData &other) const;

    ///	@brief	Assignment operator implementation.
    EventData &operator = (const EventData &other);

private:
    const Event& m_Event;
    EventTriggerer* m_Source;
    d32 m_timestamp;
    ui32 m_numRefs;
};

//--------------------------------------------------------------------------------------------------------------------
inline 
EventData::EventData(const Event& e, EventTriggerer* c) :
    m_Event( e ),
    m_Source( c ),
    m_timestamp( 0.0 ),
    m_numRefs( 1 )
{
    // empty
}

//--------------------------------------------------------------------------------------------------------------------
inline 
EventData::EventData(const EventData &other) :
    m_Event(other.m_Event)
{
    copy( other );
}

//--------------------------------------------------------------------------------------------------------------------
inline 
EventData::~EventData() {
    // empty
}

//--------------------------------------------------------------------------------------------------------------------
inline 
void EventData::copy( const EventData &other ) {
    m_Source = other.m_Source;
}

//--------------------------------------------------------------------------------------------------------------------
inline 
const Event& EventData::getEvent() const {
    return m_Event; 
}

//--------------------------------------------------------------------------------------------------------------------
inline 
EventTriggerer* EventData::getEventSource() const  {
    return m_Source; 
}

//--------------------------------------------------------------------------------------------------------------------
inline 
void EventData::get() {
    ++m_numRefs;
}

//--------------------------------------------------------------------------------------------------------------------
inline 
void EventData::release() {
    if ( m_numRefs ) {
        --m_numRefs;
        if ( 0 == m_numRefs ) {
            delete this;
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------
inline bool 
EventData::operator == (const EventData &other) const {
    return (m_Event == other.m_Event && m_Source == other.m_Source);
}

//--------------------------------------------------------------------------------------------------------------------
inline 
EventData &EventData::operator = (const EventData &other) {
    if (other == *this) {
        return *this;
    }

    copy( other );

    return *this;
}

//--------------------------------------------------------------------------------------------------------------------
///	Base event functor.
typedef Functor<void, const Event&, const EventData*> ceEventFunctor;

//--------------------------------------------------------------------------------------------------------------------

} // Namespace Common
} // Namespace ZFXCE2
