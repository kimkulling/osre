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
#pragma once

#include <osre/Common/AbstractEventHandler.h>
#include <osre/Common/EventTriggerer.h>
#include <osre/Common/Object.h>
#include <cppcore/Container/TArray.h>
#include <cppcore/Container/TList.h>

namespace OSRE {

namespace RenderBackend {
    class RenderBackendService;
}

namespace Platform {

// Forward declarations
class OSEventListener;

using EventDataList = CPPCore::TList<Common::EventData*> ;
using EventPtrArray = CPPCore::TArray<const Common::Event*>;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This abstract class defines the event handler for a platform event loop handler.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractPlatformEventQueue : public Common::Object {
public:
    ///	@brief  The class destructor, virtual.
    virtual ~AbstractPlatformEventQueue();
    
    ///	@brief  Used to register a event listener.
    ///	@param  events      List of events.
    /// @param  pListener   The listener instance
    virtual void registerEventListener( const EventPtrArray &events, OSEventListener *pListener ) = 0;
    
    ///	@brief  Used to unregister a event listener.
    ///	@param  events      List of events.
    /// @param  pListener   The listener instance
    virtual void unregisterEventListener( const EventPtrArray &events, OSEventListener *pListener ) = 0;
    
    ///	@brief  Set the polling state.
    ///	@param  enabled     true for enabling polling.
    virtual void enablePolling( bool enabled ) = 0;

    /// @brief  Returns the currently active polling state.
    /// @return The active polling state.
    virtual bool isPolling() const = 0;

    virtual bool update() = 0;

    void setRenderBackendService( RenderBackend::RenderBackendService *rbSrv );
    
    RenderBackend::RenderBackendService *getRenderBackendService() const;

protected:
    /// @brief  The class constructor.
    AbstractPlatformEventQueue();

    ///	@brief  Will be called to process events.
    /// @param  pTriggerer  The event trigger.
    void processEvents( Common::EventTriggerer *pTriggerer );
    
    ///	@brief  Returns the active event data list.
    /// @return The active event data list.
    EventDataList *getActiveEventDataList();

    ///	@brief  Returns the active event data list.
    /// @return The pending event data list.
    EventDataList *getPendingEventDataList();
    
    ///	@brief  Toggles between the active and pending list.
    void switchEventDataList();


private:
    enum {
        numEventQueues = 2
    };
    EventDataList m_eventQueues[ numEventQueues ];
    ui32 m_activeList;
    RenderBackend::RenderBackendService *m_rbSrv;
};

inline
AbstractPlatformEventQueue::AbstractPlatformEventQueue()
: Object("Platform/AbstractPlatformEventQueue")
, m_activeList( 0 )
, m_rbSrv( nullptr ) {
    // empty
}

inline
AbstractPlatformEventQueue::~AbstractPlatformEventQueue( ) {
    // empty
}

inline
void AbstractPlatformEventQueue::processEvents( Common::EventTriggerer *triggerer ) {
    if ( nullptr == triggerer ) {
        return;
    }

    EventDataList *theList = getActiveEventDataList();
    if ( nullptr == theList ) {
        return;
    }

    while( !theList->isEmpty() ) {
        Common::EventData *eventData = theList->front();
        if ( nullptr != eventData ) {
            triggerer->triggerEvent( eventData->getEvent(), eventData );
            theList->removeFront();
            eventData->release();
        }
    }
    switchEventDataList();
}

inline
EventDataList *AbstractPlatformEventQueue::getActiveEventDataList() {
    EventDataList *activeEventQueue( &m_eventQueues[ m_activeList ] );
    return activeEventQueue;
}

inline
EventDataList *AbstractPlatformEventQueue::getPendingEventDataList() {
    ui32 queueToProcess = ( m_activeList + 1 ) % numEventQueues;
    m_eventQueues[ queueToProcess ].clear( );
    EventDataList *pendingEventQueue( &m_eventQueues[ queueToProcess ] );

    return pendingEventQueue;
}

inline
void AbstractPlatformEventQueue::switchEventDataList() {
    m_activeList = ( m_activeList + 1 ) % numEventQueues;
}

inline
void AbstractPlatformEventQueue::setRenderBackendService( RenderBackend::RenderBackendService *rbSrv ) {
    m_rbSrv = rbSrv;
}

inline
RenderBackend::RenderBackendService *AbstractPlatformEventQueue::getRenderBackendService() const {
    return m_rbSrv;
}

} // Namespace Platform
} // Namespace OSRE
