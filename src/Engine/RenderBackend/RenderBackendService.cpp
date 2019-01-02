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
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Properties/Settings.h>
#include <osre/Profiling/PerformanceCounterRegistry.h>
#include <osre/Threading/SystemTask.h>
#include <osre/Scene/DbgRenderer.h>
#include <osre/UI/Widget.h>

#include "OGLRenderer/OGLRenderEventHandler.h"
#include "VulkanRenderer/VlkRenderEventHandler.h"
#ifdef OSRE_WINDOWS
#   include "DX11Renderer/DX11Renderer.h"
#   include "DX11Renderer/DX11RenderVEventHandler.h"
#endif

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Threading;
using namespace ::OSRE::Properties;

static const c8 *Tag = "RenderBackendService";

RenderBackendService::RenderBackendService()
: AbstractService( "renderbackend/renderbackendserver" )
, m_matrixBuffer()
, m_renderTaskPtr()
, m_settings( nullptr )
, m_ownsSettingsConfig( false )
, m_nextFrame()
, m_screen( nullptr )
, m_passes()
, m_currentPass(nullptr)
, m_newGeo()
, m_geoUpdates()
, m_newInstances()
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

static const c8 *OGL_API    = "opengl";
static const c8 *Vulkan_API = "vulkan";
#ifdef OSRE_WINDOWS
static const c8 *DX11_API   = "dx11";
#endif // OSRE_WINDOWS

bool RenderBackendService::onOpen() {
    if ( nullptr == m_settings ) {
        m_settings = new Settings;
        m_ownsSettingsConfig = true;
    }

    if( !m_renderTaskPtr.isValid() ) {
        m_renderTaskPtr.init( SystemTask::create( "render_task" ) );
    }

    bool ok( true );

    // Run the render task
    ok = m_renderTaskPtr->start( nullptr );
    if ( !ok ) {
        osre_error( Tag, "Cannot run render task." );
        return ok;
    }
    
    // Create render event handler for back-end
    const String api = m_settings->get( Settings::RenderAPI ).getString();
    if ( api == OGL_API ) {
        m_renderTaskPtr->attachEventHandler( new OGLRenderEventHandler );
    } else if ( api == Vulkan_API ) {
        m_renderTaskPtr->attachEventHandler( new VlkRenderEventHandler );
    } 
#ifdef OSRE_WINDOWS
    else if (api == DX11_API) {
        m_renderTaskPtr->attachEventHandler( new DX11RenderEventHandler );
    } 
#endif // OSRE_WINDOWS
    else {
        osre_error( Tag, "Requested render-api unknown: " + api );
        ok = false;
    }

    // Create the debug renderer instance
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

bool RenderBackendService::onUpdate() {
    if ( !m_renderTaskPtr.isValid() ) {
        return false;
    }

    commitNextFrame();

    // Synchronizing event with render back-end
    auto result( m_renderTaskPtr->sendEvent( &OnRenderFrameEvent, nullptr ) );
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

static void setupGeoPackage(NewGeoEntry *newEntry, GeometryPackage *package ) {
    OSRE_ASSERT( nullptr != newEntry );
    OSRE_ASSERT( nullptr != package );
    
    const ui32 numNewGeo( newEntry->m_geo.size() );
    package->m_newGeo = new Mesh*[numNewGeo];
    package->m_numNewGeo = numNewGeo;
    package->m_numInstances = newEntry->numInstances;
    for ( ui32 i=0; i<numNewGeo; i++ ) {
        Mesh *geo( newEntry->m_geo[ i ] );
        package->m_newGeo[ i ] = geo;
    }
}

void RenderBackendService::commitNextFrame() {
    if ( !m_renderTaskPtr.isValid() ) {
        return;
    }
    
    m_nextFrame.m_model = m_matrixBuffer.m_model;
    m_nextFrame.m_view = m_matrixBuffer.m_view;
    m_nextFrame.m_proj = m_matrixBuffer.m_proj;
    if ( !m_newGeo.isEmpty() ) {
        m_nextFrame.m_numGeoPackages = m_newGeo.size();
        m_nextFrame.m_geoPackages = new GeometryPackage*[ m_newGeo.size() ];
        for (ui32 i = 0; i < m_newGeo.size(); i++) {
            m_nextFrame.m_geoPackages[ i ]= new GeometryPackage;
            setupGeoPackage( m_newGeo[ i ], m_nextFrame.m_geoPackages[ i ] );
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

    if ( !m_geoUpdates.isEmpty() ) {
        m_nextFrame.m_numGeoUpdates = m_geoUpdates.size();
        m_nextFrame.m_geoUpdates = new Mesh*[m_nextFrame.m_numGeoUpdates];
        for (ui32 i = 0; i < m_nextFrame.m_numGeoUpdates; i++) {
            m_nextFrame.m_geoUpdates[i] = m_geoUpdates[i];
        }
        m_geoUpdates.resize(0);
    }
    CommitFrameEventData *data = new CommitFrameEventData;
    data->m_frame = &m_nextFrame;
    m_renderTaskPtr->sendEvent( &OnCommitFrameEvent, data );
}

void RenderBackendService::sendEvent( const Event *ev, const EventData *eventData ) {
    if ( m_renderTaskPtr.isValid() ) {
        m_renderTaskPtr->sendEvent( ev, eventData );
    }
}

bool RenderBackendService::beginPass() {
    if (nullptr != m_currentPass) {
        osre_warn(Tag, "Pass recording already active.");
        return false;
    }

    m_currentPass = new PassData;

    return true;
}

bool RenderBackendService::beginRenderBatch() {
    if (nullptr != m_currentPass) {
        osre_warn(Tag, "Pass recording not active.");
        return false;
    }

    return true;
}

void RenderBackendService::setMatrix(MatrixType type, const glm::mat4 &m) {
    switch (type) {
        case MatrixType::Model:
            m_matrixBuffer.m_model = m;
            break;
        case MatrixType::View:
            m_matrixBuffer.m_view = m;
            break;
        case MatrixType::Projection:
            m_matrixBuffer.m_proj = m;
            break;
        default:
            break;
    }
}

void RenderBackendService::setMatrix( const String &name, const glm::mat4 &matrix ) {
    UniformVar *uniform( nullptr );
    const ui32 key( StringUtils::hashName( name.c_str() ) );
    if ( !m_variables.hasKey( key ) ) {
        uniform = UniformVar::create( name, ParameterType::PT_Mat4 );
        m_variables.insert( key, uniform );
    } else {
        m_variables.getValue( key, uniform );
    }    

    ::memcpy( uniform->m_data.m_data, glm::value_ptr( matrix ), sizeof( glm::mat4 ) );
    m_uniformUpdates.add( uniform );
}

void RenderBackendService::setMatrixArray(const String &name, ui32 numMat, const glm::mat4 *matrixArray) {
    UniformVar *uniform( nullptr );
    const ui32 key(Common::StringUtils::hashName(name.c_str()));
    if ( !m_variables.hasKey( key ) ) {
        uniform = UniformVar::create( name, ParameterType::PT_Mat4Array, numMat );
        m_variables.insert( key, uniform );
    } else {
        m_variables.getValue( key, uniform );
    }
    ::memcpy(uniform->m_data.m_data, glm::value_ptr( matrixArray[0] ), sizeof( glm::mat4 ) * numMat );
    m_uniformUpdates.add( uniform );
}

void RenderBackendService::attachGeo( Mesh *geo, ui32 numInstances ) {
    if ( nullptr == geo ) {
        osre_debug( Tag, "Pointer to geometry is nullptr." );
        return;
    }
    NewGeoEntry *entry = new NewGeoEntry;
    entry->m_geo.add( geo );
    entry->numInstances = numInstances;
    m_newGeo.add( entry );
}

void RenderBackendService::attachGeo( const CPPCore::TArray<Mesh*> &geoArray, ui32 numInstances ) {
    NewGeoEntry *entry = new NewGeoEntry;
    entry->numInstances = numInstances;
    entry->m_geo.add( &geoArray[ 0 ], geoArray.size() );
    m_newGeo.add( entry );
}

void RenderBackendService::attachGeoUpdate( Mesh *geo ) {
    if ( nullptr == geo ) {
        osre_debug( Tag, "Pointer to geometry is nullptr." );
        return;
    }
    m_geoUpdates.add( geo );
}

void RenderBackendService::attachGeoUpdate( const CPPCore::TArray<Mesh*> &geoArray ) {
    m_geoUpdates.add( &geoArray[ 0 ], geoArray.size() );
}

void RenderBackendService::attachGeoInstance( GeoInstanceData *instanceData ) {
    if ( nullptr == instanceData ) {
        osre_debug( Tag, "Pointer to geometry is nullptr." );
        return;
    }
    m_newInstances.add( instanceData );
}

void RenderBackendService::attachGeoInstance( const CPPCore::TArray<GeoInstanceData*> &instanceData ) {
    if (instanceData.isEmpty() ) {
        return;
    }

    m_newInstances.add( &instanceData[ 0 ], instanceData.size() );
}

bool RenderBackendService::endRenderBatch() {
    return true;
}

bool RenderBackendService::endPass() {
    if (nullptr != m_currentPass) {
        return false;
    }

    m_passes.add(m_currentPass);
    m_currentPass = nullptr;

    return true;
}

void RenderBackendService::attachView( TransformMatrixBlock &transform ) {

}

void RenderBackendService::resize( ui32 x, ui32 y, ui32 w, ui32 h ) {
    ResizeEventData *data = new ResizeEventData( x, y, w, h );
    m_renderTaskPtr->sendEvent( &OnResizeEvent, data );

    if ( m_screen != nullptr ) {
        m_screen->resize( x, y, w, h );
    }

}

void RenderBackendService::focusLost() {
    // todo: implement me
}

} // Namespace RenderBackend
} // Namespace OSRE
