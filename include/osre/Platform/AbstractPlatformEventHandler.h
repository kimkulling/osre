/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-214, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions
and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
and the following disclaimer in the documentation and/or other materials provided with the
distribution.

* Neither the name of the ZFXCE2 team, nor the names of its contributors may be used to endorse or
promote products derived from this software without specific prior written permission of the
ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
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
class DLL_EXPORT AbstractPlatformEventHandler : public Common::AbstractEventHandler {
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
} // namespace ZFXCE2
