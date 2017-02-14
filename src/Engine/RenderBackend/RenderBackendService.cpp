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

    commitUniformBuffer();

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

void RenderBackendService::commitUniformBuffer() {
    if ( m_renderTaskPtr.isValid() ) {
        for ( ui32 i = 0; i < m_paramUpdates.size(); i++ ) {
            m_renderTaskPtr->sendEvent( &OnSetParameterEvent, m_paramUpdates[ i ] );
        }
        m_paramUpdates.resize( 0 );
    }
}

void RenderBackendService::sendEvent( const Event *ev, const EventData *eventData ) {
    if ( m_renderTaskPtr.isValid() ) {
        m_renderTaskPtr->sendEvent( ev, eventData );
    }
}

void RenderBackendService::setMatrix( const String &name, const glm::mat4 &matrix ) {
    UniformVar *parameter( nullptr );
    const ui32 key( Common::StringUtils::hashName( name.c_str() ) );
    if ( !m_variables.hasKey( key ) ) {
        parameter = UniformVar::create( name, ParameterType::PT_Mat4 );
        m_variables.insert( key, parameter );
    } else {
        m_variables.getValue( key, parameter );
    }    

    ::memcpy( parameter->m_data.m_data, glm::value_ptr( matrix ), sizeof( glm::mat4 ) );

    SetParameterEventData *data = new SetParameterEventData;
    data->m_numParam = 1;
    data->m_param = new UniformVar *[ 1 ];
    data->m_param[ 0 ] = parameter;
    m_paramUpdates.add( data );
}

void RenderBackendService::attachGeo( const CPPCore::TArray<Geometry*> &geoArray ) {
    AttachGeoEventData *attachGeoEvData = new AttachGeoEventData;
    attachGeoEvData->m_numGeo = geoArray.size();
    attachGeoEvData->m_geo = new Geometry*[ attachGeoEvData->m_numGeo ];
    for ( ui32 i = 0; i < attachGeoEvData->m_numGeo; i++ ) {
        attachGeoEvData->m_geo[ i ] = geoArray[ i ];
    }
    sendEvent( &OnAttachSceneEvent, attachGeoEvData );
}

void RenderBackendService::attachView( TransformMatrixBlock &transform ) {

}

} // Namespace RenderBackend
} // Namespace OSRE
