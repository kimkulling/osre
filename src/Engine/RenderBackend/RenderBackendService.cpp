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
#include <osre/Profiling/PerformanceCounters.h>
#include <osre/Threading/SystemTask.h>
#include <osre/Scene/DbgRenderer.h>
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
, m_ownsSettingsConfig( false )
, m_nextFrame()
, m_newGeo()
, m_variables()
, m_uniformUpdates() {
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

    // Run the render task
    bool ok( true );
    m_renderTaskPtr->start( nullptr );
    
    // Create render event handler for backend 
    String api = m_settings->get( Settings::RenderAPI ).getString();
    if ( api == "opengl" ) {
        m_renderTaskPtr->attachEventHandler( new OGLRenderEventHandler );
    } else if ( api == "vulkan" ) {
        m_renderTaskPtr->attachEventHandler( new VlkRenderEventHandler );
    } else {
        osre_error( Tag, "Requested render-api unknown: " + api );
        ok = false;
    }

    // Create debug renderer
    if ( !Scene::DbgRenderer::create( this ) ) {
        osre_error( Tag, "Cannot create Debug renderer" );
        ok = false;
    }

    return ok;
}

bool RenderBackendService::onClose() {
    if( !m_renderTaskPtr.isValid() ) {
        return false;
    }

    if ( !Scene::DbgRenderer::destroy() ) {
        osre_error( Tag, "Cannot destroy Debug renderer" );
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

    commitNextFrame();

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

const Settings *RenderBackendService::getSettings() const {
    return m_settings;
}

void RenderBackendService::commitNextFrame() {
    if ( !m_renderTaskPtr.isValid() ) {
        return;
    }
        
    CommitFrameEventData *data = new CommitFrameEventData;
    if ( !m_newGeo.isEmpty() ) {
        m_nextFrame.m_numNewGeo = m_newGeo.size();
        m_nextFrame.m_newGeo = new Geometry *[ m_nextFrame.m_numNewGeo ];
        for ( ui32 i = 0; i < m_nextFrame.m_numNewGeo; i++ ) {
            m_nextFrame.m_newGeo[ i ] = m_newGeo[ i ];
        }
        m_newGeo.resize( 0 );
    }

    if ( !m_uniformUpdates.isEmpty() ) {
        m_nextFrame.m_numVars = m_uniformUpdates.size();
        m_nextFrame.m_vars = new UniformVar *[ m_nextFrame.m_numVars ];
        for ( ui32 i = 0; i < m_nextFrame.m_numVars; i++ ) {
            m_nextFrame.m_vars[ i ] = m_uniformUpdates[ i ];
        }
        m_uniformUpdates.resize( 0 );
    }
    data->m_frame = &m_nextFrame;
    m_renderTaskPtr->sendEvent( &OnCommitFrameEvent, data );
}

void RenderBackendService::sendEvent( const Event *ev, const EventData *eventData ) {
    if ( m_renderTaskPtr.isValid() ) {
        m_renderTaskPtr->sendEvent( ev, eventData );
    }
}

void RenderBackendService::setMatrix( const String &name, const glm::mat4 &matrix ) {
    UniformVar *uniform( nullptr );
    const ui32 key( Common::StringUtils::hashName( name.c_str() ) );
    if ( !m_variables.hasKey( key ) ) {
        uniform = UniformVar::create( name, ParameterType::PT_Mat4 );
        m_variables.insert( key, uniform );
    } else {
        m_variables.getValue( key, uniform );
    }    

    ::memcpy( uniform->m_data.m_data, glm::value_ptr( matrix ), sizeof( glm::mat4 ) );
    m_uniformUpdates.add( uniform );
}

void RenderBackendService::attachGeo( Geometry *geo ) {
    if ( nullptr == geo ) {
        return;
    }
    m_newGeo.add( geo );
}

void RenderBackendService::attachGeo( const CPPCore::TArray<Geometry*> &geoArray ) {
    m_newGeo.add( &geoArray[ 0 ], geoArray.size() );
}

void RenderBackendService::attachView( TransformMatrixBlock &transform ) {

}

} // Namespace RenderBackend
} // Namespace OSRE
