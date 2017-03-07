/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "OGLRenderEventHandler.h"
#include "OGLRenderBackend.h"
#include "OGLShader.h"
#include "OGLCommon.h"
#include "RenderCmdBuffer.h"

#include <osre/Common/Logger.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractSurface.h>
#include <osre/Platform/AbstractRenderContext.h>
#include <osre/Profiling/PerformanceCounters.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/Uri.h>
#include <osre/Assets/AssetRegistry.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::CPPCore;

static const String Tag = "OGLRendeEventHandler";

static const String VsSrc =
	"layout(location = 0) in vec3 position;	      // object space vertex position\n"
	"layout(location = 1) in vec3 normal;	      // object space vertex normal\n"
	"layout(location = 2) in vec2 texcoord0;	  // texture coordinate\n"
	"out vec2 vUV;\n"
	"\n"
	"void main() {\n"
	"    vec3 vertexPosition_homoneneousspace = position;\n"
	"    gl_Position = vec4(position, 1 );\n"
	"    // UV of the vertex. No special space for this one.\n"
	"    UV = texcoord0;\n"
	"};\n";

static const String FsSrc =
	"in vec2 vUV;\n"
	"// Output data\n"
	"out vec4 vFragColor;\n"
	"uniform sampler2D tex0;\n"

	"void main() {\n"
	"    vFragColor = texture( tex0, vUV );\n"
	"};\n";


static void setupTextures( Material *mat, OGLRenderBackend *rb, TArray<OGLTexture*> &textures ) {
    OSRE_ASSERT( nullptr != mat );
	OSRE_ASSERT( nullptr != rb );

    const ui32 numTextures( mat->m_numTextures );
    if( 0 == numTextures ) {
        return;
    }

    for( ui32 i = 0; i < numTextures; ++i ) {
        Texture *tex( mat->m_textures[ i ] );
        if( !tex->m_textureName.empty() ) {
            String root = Assets::AssetRegistry::getPath( "media" );
            String path = Assets::AssetRegistry::resolvePathFromUri( tex->m_loc );
            
            IO::Uri loc( tex->m_loc );
            loc.setPath( path );

            OGLTexture *oglTexture = rb->createTextureFromFile( tex->m_textureName, loc );
            if( oglTexture ) {
                textures.add( oglTexture );
            }
        }
    }
}

static SetMaterialStageCmdData *setupMaterial( Material *material, OGLRenderBackend *rb, OGLRenderEventHandler *eh ) {
	OSRE_ASSERT( nullptr != eh );
	OSRE_ASSERT( nullptr != material );
	OSRE_ASSERT( nullptr != rb );

    SetMaterialStageCmdData *matData = new SetMaterialStageCmdData;
    switch( material->m_type ) {
        case MaterialType::FlatShadingMaterial:
            break;

        case MaterialType::ShaderMaterial: {
                TArray<OGLTexture*> textures;
                setupTextures( material, rb, textures );
                OGLRenderCmd *renderMatCmd = new OGLRenderCmd;
                renderMatCmd->m_type = OGLRenderCmdType::SetMaterialCmd;
                if( !textures.isEmpty() ) {
                    matData->m_textures = textures;
                }

                OGLShader *shader = rb->createShader( "mat", material->m_pShader );
                if ( nullptr != shader ) {
                    matData->m_shader = shader;
                    for( ui32 i = 0; i < material->m_pShader->m_attributes.size(); i++ ) {
                        const String &attribute = material->m_pShader->m_attributes[ i ];
                        //if ( shader->hasAttribute( attribute ) ) {
                            shader->addAttribute( attribute );
                        //}
                    }

                    for( ui32 i = 0; i < material->m_pShader->m_parameters.size(); i++ ) {
                        const UniformVar *uniformParam( material->m_pShader->m_parameters[ i ] );
                        if ( nullptr != uniformParam ) {
                            shader->addUniform( uniformParam->m_name );
                        }
                    }

                    // for setting up all buffer objects
                    eh->setActiveShader( shader );
                }
                renderMatCmd->m_pData = matData;
                eh->enqueueRenderCmd( renderMatCmd );
            }
            break;

        default:
            break;
    }

    return matData;
}

static void setupParameter( UniformVar *param, ui32 numParam, OGLRenderBackend *rb, OGLRenderEventHandler *ev ) {
	OSRE_ASSERT( nullptr != param );
	OSRE_ASSERT( nullptr != rb );
	OSRE_ASSERT( nullptr != ev );

    if( !param ) {
        return;
    }

    ::CPPCore::TArray<OGLParameter*> paramArray;
    OGLParameter *oglParam = rb->getParameter( param->m_name );
    if ( nullptr == oglParam ) {
        oglParam = rb->createParameter( param->m_name, param->m_type, &param->m_data, param->m_numItems );
    } else {
        ::memcpy( oglParam->m_data->getData(), param->m_data.getData(), param->m_data.m_size );
    }

    paramArray.add( oglParam );
    ev->setParameter( paramArray );
}

static OGLVertexArray *setupBuffers( Geometry *geo, OGLRenderBackend *rb, OGLShader *oglShader ) {
	OSRE_ASSERT( nullptr != geo );
	OSRE_ASSERT( nullptr != rb );
	OSRE_ASSERT( nullptr != oglShader );

    rb->useShader( oglShader );

    OGLVertexArray *vertexArray = rb->createVertexArray();
    rb->bindVertexArray( vertexArray );
    BufferData *vertices = geo->m_vb;
	if ( nullptr == vertices ) {
		osre_debug( Tag, "No vertex buffer data for setting up data." );
		return nullptr;
    }

    BufferData *indices = geo->m_ib;
    if ( nullptr == indices ) {
        osre_debug( Tag, "No index buffer data for setting up data." );
        return nullptr;
    }

    // create vertex buffer and  and pass triangle vertex to buffer object

    OGLBuffer *vb = rb->createBuffer( vertices->m_type );
    vb->m_geoId = geo->m_id;
    rb->bindBuffer( vb );
    rb->bufferData( vb, vertices->m_data, vertices->m_size, vertices->m_access );

    // enable vertex attribute arrays
    TArray<OGLVertexAttribute*> attributes;
    rb->createVertexCompArray( geo->m_vertextype, oglShader, attributes );
    const ui32 stride = OGLRenderBackend::getVertexSize( geo->m_vertextype );
    rb->bindVertexLayout( vertexArray, oglShader, stride, attributes );
    rb->releaseVertexCompArray( attributes );

    // create index buffer and pass indices to element array buffer
    OGLBuffer *ib = rb->createBuffer( indices->m_type );
    ib->m_geoId = geo->m_id;
    rb->bindBuffer( ib );
    rb->bufferData( ib, indices->m_data, indices->m_size, indices->m_access );

    rb->unbindVertexArray();

    return vertexArray;
}

static void setupPrimDrawCmd( const TArray<ui32> &primGroups, OGLRenderBackend *rb, OGLRenderEventHandler *eh, OGLVertexArray *va ) {
	OSRE_ASSERT( nullptr != rb );
	OSRE_ASSERT( nullptr != eh );

    if( primGroups.isEmpty() ) {
        return;
    }

	OGLRenderCmd *renderCmd = OGLRenderCmdAllocator::alloc( OGLRenderCmdType::DrawPrimitivesCmd, nullptr );
    DrawPrimitivesCmdData *data = new DrawPrimitivesCmdData;
    data->m_vertexArray = va;
    data->m_primitives.reserve( primGroups.size() );
    for( ui32 i = 0; i < primGroups.size(); ++i ) {
        data->m_primitives.add( primGroups[ i ] );
    }
    renderCmd->m_pData = static_cast< void* >( data );
    
    eh->enqueueRenderCmd( renderCmd );
}

static void setupInstancedDrawCmd( const TArray<ui32> &ids, Frame *currentFrame, 
                                   OGLRenderBackend *rb, OGLRenderEventHandler *eh, OGLVertexArray *va ) {
	OSRE_ASSERT( nullptr != currentFrame );
	OSRE_ASSERT( nullptr != rb );
	OSRE_ASSERT( nullptr != eh );

    if( ids.isEmpty() ) {
        return;
    }

    GeoInstanceData *instData( currentFrame->m_geoInstanceData );
	OGLRenderCmd *renderCmd = OGLRenderCmdAllocator::alloc( OGLRenderCmdType::DrawPrimitivesInstancesCmd, nullptr );
    if( nullptr != instData ) {
        if( nullptr != instData->m_data ) {
            OGLBuffer *instanceDataBuffer = rb->createBuffer( BufferType::InstanceBuffer );
            rb->bindBuffer( instanceDataBuffer );
            rb->bufferData( instanceDataBuffer, instData->m_data->m_data, instData->m_data->m_size, instData->m_data->m_access );
        }
    }
    
    for ( ui32 i=0; i<currentFrame->m_numGeoPackages; i++ ) {
        GeometryPackage *currentGeoPackage( currentFrame->m_geoPackages[ i ] );
        if ( nullptr == currentGeoPackage ) {
            continue;
        }
        DrawInstancePrimitivesCmdData *data = new DrawInstancePrimitivesCmdData;
        data->m_vertexArray = va;
        data->m_numInstances = currentGeoPackage->m_numInstances;
        data->m_primitives.reserve( ids.size() );
        for( ui32 j = 0; j < ids.size(); ++j ) {
            data->m_primitives.add( ids[ j ] );
        }
        renderCmd->m_pData = static_cast< void* >( data );
        eh->enqueueRenderCmd( renderCmd );
    }
}

OGLRenderEventHandler::OGLRenderEventHandler( )
: AbstractEventHandler()
, m_oglBackend( nullptr )
, m_renderCmdBuffer( nullptr )
, m_renderCtx( nullptr )
, m_vertexArray( nullptr )  {
    // empty
}
        
OGLRenderEventHandler::~OGLRenderEventHandler( ) {
    // empty
}

bool OGLRenderEventHandler::onEvent( const Event &ev, const EventData *data ) {
    bool result( false );
    if ( OnAttachEventHandlerEvent == ev ) {
        result = onAttached( data );
    } else if ( OnDetatachEventHandlerEvent == ev ) {
        result = onDetached( data );
    } else if ( OnCreateRendererEvent == ev ) {
        result = onCreateRenderer( data );
    } else if ( OnDestroyRendererEvent == ev ) {
        result = onDestroyRenderer( data );
    } else if( OnAttachViewEvent == ev ) {
        result = onAttachView( data );
    } else if ( OnDetachViewEvent == ev ) {
        result = onDetachView( data );
    } else if ( OnRenderFrameEvent == ev ) {
        result = onRenderFrame( data );
    } else if ( OnCommitFrameEvent == ev ) {
        result = onCommitNexFrame( data );
    } else if ( OnClearSceneEvent == ev ) {
        result = onClearGeo( data );
    }
 
    return result;
}

void OGLRenderEventHandler::setActiveShader( OGLShader *oglShader ) {
    if ( nullptr == m_renderCmdBuffer ) {
        osre_error( Tag, "Renderer not up and running." );
        return;
    }
    m_renderCmdBuffer->setActiveShader( oglShader );
}

void OGLRenderEventHandler::enqueueRenderCmd( OGLRenderCmd *oglRenderCmd ) {
    if ( nullptr == m_renderCmdBuffer ) {
        osre_error( Tag, "Renderer not up and running." );
        return;
    }
    m_renderCmdBuffer->enqueueRenderCmd( oglRenderCmd );
}

void OGLRenderEventHandler::setParameter( const ::CPPCore::TArray<OGLParameter*> &paramArray ) {
    m_renderCmdBuffer->setParameter( paramArray );
}

bool OGLRenderEventHandler::onAttached( const EventData *eventData ) {
    if( nullptr != m_oglBackend ) {
        return false;
    }

    m_oglBackend = new OGLRenderBackend;
    m_oglBackend->setTimer( PlatformInterface::getInstance()->getTimer() );
        
    return true;
}

bool OGLRenderEventHandler::onDetached( const EventData *eventData ) {
    if( m_renderCmdBuffer ) {
        osre_error( Tag, "Renderer not destroyed." );
        delete m_renderCmdBuffer;
        m_renderCmdBuffer = nullptr;
    }

    delete m_oglBackend;
    m_oglBackend = nullptr;

    return true;
}

bool OGLRenderEventHandler::onCreateRenderer( const EventData *eventData ) {
	OSRE_ASSERT( nullptr != m_oglBackend );
	
	CreateRendererEventData *createRendererEvData = ( CreateRendererEventData* ) eventData;
    AbstractSurface *activeSurface = createRendererEvData->m_activeSurface;
    if( !activeSurface ) {
        osre_debug( Tag, "No active surface." );
        return false;
    }

    if( !activeSurface->create() ) {
        osre_debug( Tag, "Cannot create active surface." );
        return false;
    }

    bool result( false );
    m_renderCtx = PlatformInterface::getInstance()->getRenderContext();

    if( nullptr !=  m_renderCtx ) {
        result = m_renderCtx->create( activeSurface );
        if( !result ) {
            osre_debug( Tag, "Cannot create render context." );
            return false;
        }
    }

    m_oglBackend->setRenderContext( m_renderCtx );
    if ( !m_renderCtx ) {
        return false;
    }

    const ui32 x = activeSurface->getProperties()->m_x;
    const ui32 y = activeSurface->getProperties()->m_y;
    const ui32 w = activeSurface->getProperties()->m_width;
    const ui32 h = activeSurface->getProperties()->m_height;
    m_oglBackend->setViewport( x, y, w, h );

    const String defaultFont( PlatformInterface::getInstance()->getDefaultFontName() );
    IO::Uri fontUri( "file://assets/Textures/Fonts/" + defaultFont );
    String root = Assets::AssetRegistry::getPath( "media" );
    String path = Assets::AssetRegistry::resolvePathFromUri( fontUri );
    fontUri.setPath( path );
    m_oglBackend->createFont( fontUri );
    m_renderCmdBuffer = new RenderCmdBuffer( m_oglBackend, m_renderCtx, createRendererEvData->m_pipeline );

    bool ok( Profiling::PerformanceCounters::create() );
    if ( !ok ) {
        osre_error( Tag, "Error while destroying performance counters." );
        return false;
    }

    Profiling::PerformanceCounters::registerCounter( "fps" );

    return true;
}

bool OGLRenderEventHandler::onDestroyRenderer( const Common::EventData * ) {
	OSRE_ASSERT( nullptr != m_oglBackend );
	
	if ( nullptr != m_renderCtx ) {
        return false;
    }

    bool ok( Profiling::PerformanceCounters::destroy() );
    if ( !ok ) {
        osre_error( Tag, "Error while destroying performance counters." );
    }

    m_renderCtx->destroy();
    delete m_renderCtx;
    m_renderCtx = nullptr;

    delete m_renderCmdBuffer;
    m_renderCmdBuffer = nullptr;

    return true;
}

bool OGLRenderEventHandler::onAttachView( const EventData * ) {
	OSRE_ASSERT( nullptr != m_oglBackend );
	
	return true;
}

bool OGLRenderEventHandler::onDetachView( const EventData * ) {
    OSRE_ASSERT( nullptr != m_oglBackend );

    return true;
}

bool OGLRenderEventHandler::onClearGeo( const EventData * ) {
	OSRE_ASSERT( nullptr != m_oglBackend );
	
	m_oglBackend->releaseAllBuffers();
    m_oglBackend->releaseAllShaders();
    m_oglBackend->releaseAllTextures();
    m_oglBackend->releaseAllParameters();
    m_renderCmdBuffer->clear();

    return true;
}

bool OGLRenderEventHandler::onRenderFrame( const EventData *eventData ) {
	OSRE_ASSERT( nullptr != m_oglBackend );

    if ( nullptr == m_renderCtx ) {
		osre_debug( Tag, "Render context is nullptr." );
        return false;
    }


    m_renderCmdBuffer->onPreRenderFrame();
    m_renderCmdBuffer->onRenderFrame( eventData );
    m_renderCmdBuffer->onPostRenderFrame();

    return true;
}

bool OGLRenderEventHandler::onCommitNexFrame( const Common::EventData *eventData ) {
    OSRE_ASSERT( nullptr != m_oglBackend );

    CommitFrameEventData *frameToCommitData = ( CommitFrameEventData* ) eventData;
    if ( nullptr == frameToCommitData ) {
        return false;
    }
    
    Frame *frame = frameToCommitData->m_frame;
    for ( ui32 geoPackageIdx = 0; geoPackageIdx<frame->m_numGeoPackages; geoPackageIdx++ ) {
        GeometryPackage *currentGeoPackage( frame->m_geoPackages[ geoPackageIdx ] );
        if ( nullptr == currentGeoPackage ) {
            continue;
        }

        for( ui32 geoIdx = 0; geoIdx < currentGeoPackage->m_numNewGeo; ++geoIdx ) {
            Geometry *geo = currentGeoPackage->m_newGeo[ geoIdx ];
            if( nullptr == geo ) {
                osre_debug( Tag, "Geometry-pointer is a nullptr." );
                return false;
            }

            // register primitive groups to render
            CPPCore::TArray<ui32> primGroups;
            for( ui32 i = 0; i < geo->m_numPrimGroups; ++i ) {
                const ui32 primIdx( m_oglBackend->addPrimitiveGroup( &geo->m_pPrimGroups[ i ] ) );
                primGroups.add( primIdx );
            }

            // create the default material
            SetMaterialStageCmdData *data = setupMaterial( geo->m_material, m_oglBackend, this );

            // setup vertex array, vertex and index buffers
            m_vertexArray = setupBuffers( geo, m_oglBackend, m_renderCmdBuffer->getActiveShader() );
            if( nullptr == m_vertexArray ) {
                osre_debug( Tag, "Vertex-Array-pointer is a nullptr." );
                return false;
            }
            data->m_vertexArray = m_vertexArray;

            // setup the draw calls
            if( 0 == currentGeoPackage->m_numInstances ) {
                setupPrimDrawCmd( primGroups, m_oglBackend, this, m_vertexArray );
            } else {
                setupInstancedDrawCmd( primGroups, frame, m_oglBackend, this, m_vertexArray );
            }
        }
    }

    // setup global parameter
    if( frame->m_numVars > 0 ) {
        for( ui32 i = 0; i < frame->m_numVars; i++ ) {
            setupParameter( frame->m_vars[ i ], 1, m_oglBackend, this );
        }
    }

    if ( nullptr != frame->m_geoPackages ) {
        delete[] frame->m_geoPackages;
        frame->m_geoPackages = nullptr;
        frame->m_numGeoPackages = 0;
    }

    for ( ui32 i=0; i<frame->m_numGeoUpdates; i++ ) {
        Geometry *geo = frame->m_geoUpdates[ i ];
        if ( nullptr == geo ) {
            osre_debug(Tag, "Geometry-update-pointer is a nullptr.");
            return false;
        }

        OGLBuffer *buffer(m_oglBackend->getBufferById(geo->m_id));
        if (nullptr != buffer) {
            m_oglBackend->bindBuffer(buffer);
            m_oglBackend->bufferData(buffer, geo->m_vb->m_data, geo->m_vb->m_size, geo->m_vb->m_access);
            m_oglBackend->unbindBuffer(buffer);
        }
    }
    delete[] frame->m_geoUpdates;
    frame->m_geoUpdates = nullptr;
    frame->m_numGeoUpdates = 0;

    m_oglBackend->useShader( nullptr );

    return true;
}

} // Namespace RenderBackend
} // Namespace OSRE
