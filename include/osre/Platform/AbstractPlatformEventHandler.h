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

#include <osre/Common/AbstractEventHandler.h>
#include <osre/Common/EventTriggerer.h>
#include <cppcore/Container/TArray.h>
#include <cppcore/Container/TList.h>

namespace OSRE {
namespace Platform {

class OSEventListener;

typedef CPPCore::TList<Common::EventData*> EventDataList;

//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Platform::AbstractPlatformEventHandler
///	@ingroup	Infrastructure
///
///	@brief  This abstract class defines the event handler for a platform event loop handler.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractPlatformEventHandler : public Common::AbstractEventHandler {
public:
    virtual ~AbstractPlatformEventHandler();
    virtual void registerEventListener( const CPPCore::TArray<const Common::Event*> &events, OSEventListener *pListener ) = 0;
    virtual void unregisterEventListener( const CPPCore::TArray<const Common::Event*> &events, OSEventListener *pListener ) = 0;
    virtual void enablePolling( bool enabled ) = 0;

protected:
    AbstractPlatformEventHandler();
    void processEvents( Common::EventTriggerer *pTriggerer );
    EventDataList *getActiveEventDataList();
    EventDataList *getPendingEventDataList();
    void switchEventDataList();

private:
    enum {
        numEventQueues = 2
    };
    EventDataList m_eventQueues[ numEventQueues ];
    ui32 m_activeList;
};

//-------------------------------------------------------------------------------------------------
inline
AbstractPlatformEventHandler::AbstractPlatformEventHandler( )
: AbstractEventHandler()
, m_activeList( 0 ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline
AbstractPlatformEventHandler::~AbstractPlatformEventHandler( ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline
void AbstractPlatformEventHandler::processEvents( Common::EventTriggerer *pTriggerer ) {
    EventDataList *pList = getActiveEventDataList();
    while( !pList->isEmpty() ) {
        Common::EventData *pEventData = pList->front();
        pTriggerer->triggerEvent( pEventData->getEvent(), pEventData );
        pList->removeFront();
        pEventData->release();
    }
    switchEventDataList();
}

//-------------------------------------------------------------------------------------------------
inline
EventDataList *AbstractPlatformEventHandler::getActiveEventDataList() {
    EventDataList *pActiveEventQueue( &m_eventQueues[ m_activeList ] );
    return pActiveEventQueue;
}

//-------------------------------------------------------------------------------------------------
inline
EventDataList *AbstractPlatformEventHandler::getPendingEventDataList() {
    ui32 queueToProcess = ( m_activeList + 1 ) % numEventQueues;
    m_eventQueues[ queueToProcess ].clear( );
    EventDataList *pPendingEventQueue( &m_eventQueues[ queueToProcess ] );
    return pPendingEventQueue;
}

//-------------------------------------------------------------------------------------------------
inline
void AbstractPlatformEventHandler::switchEventDataList() {
    m_activeList = ( m_activeList + 1 ) % numEventQueues;
}

//-------------------------------------------------------------------------------------------------

} // namespace Platform
} // Namespace OSRE
