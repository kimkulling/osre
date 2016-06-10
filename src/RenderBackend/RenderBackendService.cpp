/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Properties/Settings.h>
#include <osre/Threading/SystemTask.h>
#include "OGLRenderer/OGLRenderEventHandler.h"
#include "VulkanRenderer/VlkRenderEventHandler.h"

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Threading;
using namespace ::OSRE::Properties;

static const String Tag = "RenderBackendService";

RenderBackendService::RenderBackendService()
: AbstractService( "renderbackend/renderbackendserver" )
, m_renderTaskPtr()
, m_settings( nullptr )
, m_ownsSettingsConfig( false ) {
    // empty
}

RenderBackendService::~RenderBackendService() {
    if( m_ownsSettingsConfig ) {
        delete m_settings;
        m_settings = nullptr;
    }
}

bool RenderBackendService::onOpen() {
    if ( nullptr == m_settings ) {
        m_settings = new Properties::Settings;
        m_ownsSettingsConfig = true;
    }

    if( !m_renderTaskPtr.isValid() ) {
        m_renderTaskPtr.init( SystemTask::create( "render_task" ) );
    }

    bool ok( true );
    m_renderTaskPtr->start( nullptr );
    String api = m_settings->get( Settings::RenderAPI ).getString();
    if ( api == "opengl" ) {
        m_renderTaskPtr->attachEventHandler( new OGLRenderEventHandler );
    } else if ( api == "vulkan" ) {
        m_renderTaskPtr->attachEventHandler( new VlkRenderEventHandler );
    } else {
        osre_error( Tag, "Requested render-api unknown: " + api );
        ok = false;
    }

    return ok;
}

bool RenderBackendService::onClose() {
    if( !m_renderTaskPtr.isValid() ) {
        return false;
    }

    if ( m_renderTaskPtr->isRunning() ) {
        m_renderTaskPtr->detachEventHandler();        
        m_renderTaskPtr->stop();
    }

    return true;
}

bool RenderBackendService::onUpdate( d32 timediff ) {
    if ( !m_renderTaskPtr.isValid() ) {
        return false;
    }
    
    // synchronizing event with render back-end
    bool result( m_renderTaskPtr->sendEvent( &OnRenderFrameEvent, nullptr ) );
    m_renderTaskPtr->awaitUpdate();

    return result;
}

void RenderBackendService::setSettings( const Settings *config, bool moveOwnership ) {
    if ( m_ownsSettingsConfig && m_settings != nullptr ) {
        delete m_settings;
        m_settings = nullptr;
        m_ownsSettingsConfig = false;
    }
    m_settings = config;
    m_ownsSettingsConfig = moveOwnership;
}

const Properties::Settings *RenderBackendService::getSettings() const {
    return m_settings;
}

void RenderBackendService::sendEvent( const Event *pEvent, const EventData *eventData ) {
    if ( m_renderTaskPtr.isValid() ) {
        m_renderTaskPtr->sendEvent( pEvent, eventData );
    }
}

} // Namespace RenderBackend
} // Namespace OSRE
