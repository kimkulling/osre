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
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Properties/ConfigurationMap.h>
#include <osre/Threading/SystemTask.h>
#include "OGLRenderer/OGLRenderEventHandler.h"

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Threading;
using namespace ::OSRE::Properties;

//-------------------------------------------------------------------------------------------------
RenderBackendService::RenderBackendService()
: AbstractService( "renderbackend/renderbackendserver" )
, m_RenderTaskPtr()
, m_pConfigMap( nullptr )
, m_ownConfig( false ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
RenderBackendService::~RenderBackendService() {
    if( m_ownConfig ) {
        delete m_pConfigMap;
        m_pConfigMap = nullptr;
    }
}

//-------------------------------------------------------------------------------------------------
bool RenderBackendService::onOpen() {
    if (!m_pConfigMap) {
        m_pConfigMap = new Properties::ConfigurationMap;
        m_ownConfig = true;
    }

    if( !m_RenderTaskPtr.isValid() ) {
        m_RenderTaskPtr.init( SystemTask::create( "render_task" ) );
    }

    m_RenderTaskPtr->start( nullptr );
    m_RenderTaskPtr->attachEventHandler( new OGLRenderEventHandler );

    return true;
}

//-------------------------------------------------------------------------------------------------
bool RenderBackendService::onClose() {
    if( !m_RenderTaskPtr.isValid() ) {
        return false;
    }

    if ( m_RenderTaskPtr->isRunning() ) {
        m_RenderTaskPtr->detachEventHandler();        
        m_RenderTaskPtr->stop();
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
bool RenderBackendService::onUpdate( d32 timediff ) {
    if ( !m_RenderTaskPtr.isValid() ) {
        return false;
    }
    
    // synchronizing event with render back-end
    bool result( m_RenderTaskPtr->sendEvent( &OnRenderFrameEvent, nullptr ) );
    m_RenderTaskPtr->await();

    return result;
}

//-------------------------------------------------------------------------------------------------
void RenderBackendService::setConfig( const ConfigurationMap *pConfiguration ) {
    m_pConfigMap = pConfiguration;
}

//-------------------------------------------------------------------------------------------------
const Properties::ConfigurationMap *RenderBackendService::getConfig() const {
    return m_pConfigMap;
}

//-------------------------------------------------------------------------------------------------
void RenderBackendService::sendEvent( const Event *pEvent, const EventData *eventData ) {
    if ( m_RenderTaskPtr.isValid() ) {
        m_RenderTaskPtr->sendEvent( pEvent, eventData );
    }
}

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace OSRE
