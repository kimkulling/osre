/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-2015, ZFXCE2 Development Team
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
#include "OGLRenderEventHandler.h"
#include "OGLRenderBackend.h"
#include "OGLShader.h"
#include "OGLCommon.h"
#include "RenderCmdBuffer.h"

#include <zfxce2/Infrastructure/Core/Logger.h>
#include <zfxce2/Infrastructure/Platform/PlatformInterface.h>
#include <zfxce2/Infrastructure/Platform/AbstractSurface.h>
#include <zfxce2/Infrastructure/Platform/PlatformInterface.h>
#include <zfxce2/Infrastructure/Platform/AbstractRenderContext.h>
#include <zfxce2/RenderSystem/RenderBackend/RenderCommon.h>

#include <cppcore/Container/TArray.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace ZFXCE2 {
namespace RenderBackend {

using namespace ::ZFXCE2::Core;
using namespace ::ZFXCE2::Platform;
using namespace ::CPPCore;

//-------------------------------------------------------------------------------------------------
static void setupTextures( Material *mat, OGLRenderBackend *rb, TArray<OGLTexture*> &textures ) {
    ce_assert( nullptr != mat );
    ce_assert( nullptr != rb );

    const ui32 numTextures( mat->m_numTextures );
    if( 0 == numTextures ) {
        return;
    }

    for( ui32 i = 0; i < numTextures; ++i ) {
        Texture &tex( mat->m_pTextures[ i ] );
        if( !tex.m_textureName.empty() ) {
            OGLTexture *oglTexture = rb->createTextureFromFile( tex.m_textureName, tex.m_textureName );
            if( oglTexture ) {
                textures.add( oglTexture );
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
static void setupMaterial( Material *material, OGLRenderBackend *rb, OGLRenderEventHandler *eh ) {
    ce_assert( nullptr != eh  );
    ce_assert( nullptr != material );
    ce_assert( nullptr != rb );

    switch( material->m_type ) {
        case FlatShadingMaterial:
            break;

        case ShaderMaterial: {
                TArray<OGLTexture*> textures;
                setupTextures( material, rb, textures );
                if( !textures.isEmpty() ) {
                    OGLRenderCmd *pRenderCmd = new OGLRenderCmd;
                    pRenderCmd->m_type = SetTextureCmd;
                    SetTextureStageCmdData *pData = new SetTextureStageCmdData;
                    pData->m_textures = textures;
                    pRenderCmd->m_pData = pData;
                    eh->enqueueRenderCmd( pRenderCmd );
                }

                OGLShader *pShader = rb->createShader( "mat", material->m_pShader );
                if( pShader ) {
                    SetShaderStageCmdData *data = new SetShaderStageCmdData;
                    data->m_pShader = pShader;

                    OGLRenderCmd *pRenderCmd = new OGLRenderCmd;
                    pRenderCmd->m_type       = SetShaderCmd;
                    pRenderCmd->m_pData      = data;
                    eh->enqueueRenderCmd( pRenderCmd );

                    for( ui32 i = 0; i < material->m_pShader->m_attributes.size(); i++ ) {
                        pShader->addAttribute( material->m_pShader->m_attributes[ i ] );
                    }

                    for( ui32 i = 0; i < material->m_pShader->m_parameters.size(); i++ ) {
                        pShader->addUniform( material->m_pShader->m_parameters[ i ] );
                    }

                    // TODO: must be replaced by render command buffer
                    eh->setActiveShader( pShader );
                }
            }
            break;

        default:
            break;
    }
}

//-------------------------------------------------------------------------------------------------
static void setupParameter( Geometry *geo, OGLRenderBackend *rb, OGLRenderEventHandler *ev ) {
    ce_assert( nullptr != geo );
    ce_assert( nullptr != rb );
    ce_assert( nullptr != ev );

    if( !geo->m_pParameter ) {
        return;
    }

    OGLRenderCmd *setParameterCmd = new OGLRenderCmd;
    setParameterCmd->m_type = SetParameterCmd;
    Parameter *currentParam( geo->m_pParameter );
    ::CPPCore::TArray<OGLParameter*> paramArray;
    SetParameterCmdData *paramData = new SetParameterCmdData;
    paramData->m_numParam = geo->m_numParameter;
    paramData->m_param = new OGLParameter*[ paramData->m_numParam ];

    ui32 i( 0 );
    while( currentParam ) {
        OGLParameter *oglParam = rb->createParameter( currentParam->m_name, currentParam->m_type, 
                                                      &currentParam->m_data, currentParam->m_numItems );
        currentParam = currentParam->m_next;
        paramData->m_param[ i ] = oglParam;
        i++;
    }
    setParameterCmd->m_pData = paramData;
    ev->enqueueRenderCmd( setParameterCmd  );
}

//-------------------------------------------------------------------------------------------------
static OGLVertexArray *setupBuffers( Geometry *geo, OGLRenderBackend *rb, OGLShader *oglShader ) {
    ce_assert( nullptr != geo );
    ce_assert( nullptr != rb );
    ce_assert( nullptr != oglShader );

    BufferData *pVertices = geo->m_pVertexBuffer;
    OGLBuffer *pVB = rb->createBuffer( pVertices->m_type );

    // create index buffer
    BufferData *pIndices = geo->m_pIndexBuffer;
    OGLBuffer *pIB = rb->createBuffer( pIndices->m_type );

    OGLVertexArray *vertexArray = rb->createVertexArray();
    rb->bindVertexArray( vertexArray );

    // pass triangle vertex to buffer object
    rb->bindBuffer( pVB );
    rb->bufferData( pVB, pVertices->m_pData, pVertices->m_size, pVertices->m_access );

    // enable vertex attribute arrays
    TArray<OGLVertexAttribute*> attributes;
    rb->createVertexAttribArray( geo->m_vertextype, oglShader, attributes );
    const ui32 stride = OGLRenderBackend::getVertexSize( geo->m_vertextype );
    rb->bindVertexAttributes( vertexArray, oglShader, stride, attributes );
    rb->releasevertexAttribArray( attributes );

    // pass indices to element array buffer
    rb->bindBuffer( pIB );
    rb->bufferData( pIB, pIndices->m_pData, pIndices->m_size, pIndices->m_access );

    rb->unbindVertexArray( vertexArray );

    return vertexArray;
}

//-------------------------------------------------------------------------------------------------
static void setupPrimDrawCmd( const TArray<ui32> &ids, OGLRenderBackend *rb, OGLRenderEventHandler *eh ) {
    ce_assert( nullptr != rb );
    ce_assert( nullptr != eh );

    if( ids.isEmpty() ) {
        return;
    }

    OGLRenderCmd *pRenderCmd     = new OGLRenderCmd;
    pRenderCmd->m_type           = DrawPrimitivesCmd;
    DrawPrimitivesCmdData *data = new DrawPrimitivesCmdData;
    data->m_primitives.reserve( ids.size() );
    for( ui32 i = 0; i < ids.size(); ++i ) {
        data->m_primitives.add( ids[ i ] );
    }
    pRenderCmd->m_pData = static_cast< void* >( data );
    
    eh->enqueueRenderCmd( pRenderCmd );
}

//-------------------------------------------------------------------------------------------------
static void setupInstancedDrawCmd( const TArray<ui32> &ids, AttachGeoEventData *geoInstanceData, 
                                   OGLRenderBackend *rb, OGLRenderEventHandler *eh ) {
    ce_assert( nullptr != geoInstanceData );
    ce_assert( nullptr != rb );
    ce_assert( nullptr != eh );

    if( ids.isEmpty() ) {
        return;
    }

    GeoInstanceData *instData( geoInstanceData->m_geoInstanceData );
    OGLRenderCmd *pRenderCmd = new OGLRenderCmd;
    pRenderCmd->m_type = DrawPrimitivesInstancesCmd;
    if( nullptr != instData ) {
        if( nullptr != instData->m_data ) {
            OGLBuffer *instanceDataBuffer = rb->createBuffer( InstanceBuffer );
            rb->bindBuffer( instanceDataBuffer );
            rb->bufferData( instanceDataBuffer, instData->m_data->m_pData, instData->m_data->m_size, instData->m_data->m_access );
        }
    }
    
    DrawInstancePrimitivesCmdData *data = new DrawInstancePrimitivesCmdData;
    data->m_numInstances = geoInstanceData->m_numInstances;
    data->m_primitives.reserve( ids.size() );
    for( ui32 i = 0; i < ids.size(); ++i ) {
        data->m_primitives.add( ids[ i ] );
    }
    pRenderCmd->m_pData = static_cast< void* >( data );

    eh->enqueueRenderCmd( pRenderCmd );
}

//-------------------------------------------------------------------------------------------------
OGLRenderEventHandler::OGLRenderEventHandler( )
: AbstractEventHandler()
, m_oglBackend( nullptr )
, m_renderCmdBuffer( nullptr )
, m_pRenderCtx( nullptr )
, m_pVertexArray( nullptr ) {
    // empty
}
        
//-------------------------------------------------------------------------------------------------
OGLRenderEventHandler::~OGLRenderEventHandler( ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
bool OGLRenderEventHandler::onEvent( const Event &ev, const EventData *pEventData ) {
    bool result( false );
    if ( OnAttachEventHandlerEvent == ev ) {
        result = onAttached( pEventData );
    } else if ( OnDetatachEventHandlerEvent == ev ) {
        result = onDetached( pEventData );
    } else if ( OnCreateRendererEvent == ev ) {
        result = onCreateRenderer( pEventData );
    } else if ( OnDestroyRendererEvent == ev ) {
        result = onDestroyRenderer( pEventData );
    } else if( OnAttachViewEvent == ev ) {
        result = onAttachView( pEventData );
    } else if ( OnDetachViewEvent == ev ) {
        // todo!
    } else if ( OnAttachSceneEvent == ev ) {
        result = onAttachGeo( pEventData );
    } else if( OnDetachSceneEvent == ev ) {
        // todo!
    } else if ( OnClearSceneEvent == ev ) {
        result = onClearGeo( pEventData );
    } else if( OnRenderFrameEvent == ev ) {
        result = onRenderFrame( pEventData );
    } else if( OnUpdateParameterEvent == ev ) {
        result = onUpdateParameter( pEventData );
    }

    delete pEventData;

    return result;
}

//-------------------------------------------------------------------------------------------------
void OGLRenderEventHandler::setActiveShader( OGLShader *oglShader ) {
    m_renderCmdBuffer->setActiveShader( oglShader );
}

//-------------------------------------------------------------------------------------------------
void OGLRenderEventHandler::enqueueRenderCmd( OGLRenderCmd *oglRenderCmd ) {
    ce_assert( nullptr != oglRenderCmd );

    m_renderCmdBuffer->enqueueRenderCmd( oglRenderCmd );
}

//-------------------------------------------------------------------------------------------------
bool OGLRenderEventHandler::onAttached( const EventData *evData ) {
    if( nullptr != m_oglBackend ) {
        return false;
    }

    m_oglBackend = new OGLRenderBackend;

    return true;
}

//-------------------------------------------------------------------------------------------------
bool OGLRenderEventHandler::onDetached( const EventData *evData ) {
    if( m_renderCmdBuffer ) {
        ce_error( "Renderer not destroyed." );
        delete m_renderCmdBuffer;
        m_renderCmdBuffer = nullptr;
    }

    delete m_oglBackend;
    m_oglBackend = nullptr;

    return true;
}

//-------------------------------------------------------------------------------------------------
bool OGLRenderEventHandler::onCreateRenderer( const EventData *evData ) {
    CreateRendererEventData *pCreateRendererEvData = (CreateRendererEventData*) evData;
    AbstractSurface *pActiveSurface = pCreateRendererEvData->m_pActiveSurface;
    if( !pActiveSurface ) {
        return false;
    }

    if( !pActiveSurface->create() ) {
        return false;
    }

    bool result( false );
    m_pRenderCtx = PlatformInterface::getInstance()->getRenderContext();

    if( m_pRenderCtx ) {
        result = m_pRenderCtx->create( pActiveSurface );
        if( !result ) {
            return false;
        }
    }

    m_oglBackend->setRenderContext( m_pRenderCtx );
    if ( !m_pRenderCtx ) {
        return false;
    }

    const ui32 x = pActiveSurface->getProperties()->m_x;
    const ui32 y = pActiveSurface->getProperties()->m_y;
    const ui32 w = pActiveSurface->getProperties()->m_width;
    const ui32 h = pActiveSurface->getProperties()->m_height;
    m_oglBackend->setViewport( x, y, w, h );

    m_renderCmdBuffer = new RenderCmdBuffer( m_oglBackend, m_pRenderCtx );

    return true;
}

//-------------------------------------------------------------------------------------------------
bool OGLRenderEventHandler::onDestroyRenderer( const Core::EventData * ) {
    if ( !m_pRenderCtx ) {
        return false;
    }

    m_pRenderCtx->destroy();
    delete m_pRenderCtx;
    m_pRenderCtx = nullptr;

    delete m_renderCmdBuffer;
    m_renderCmdBuffer = nullptr;

    return true;
}

//-------------------------------------------------------------------------------------------------
bool OGLRenderEventHandler::onAttachView( const EventData * ) {
    return true;
}

//-------------------------------------------------------------------------------------------------
bool OGLRenderEventHandler::onAttachGeo( const EventData *evData ) {
    AttachGeoEventData *attachSceneEvData = (AttachGeoEventData*) evData;
    if( !attachSceneEvData ) {
        ce_debug( "AttachSceneEventData-pointer is a nullptr." );
        return false;
    }
    
    Geometry *geo = attachSceneEvData->m_pGeometry;
    if( !geo ) {
        ce_debug( "Geometry-pointer is a nullptr." );
        return false;
    }

    // register primitive groups to render
    CPPCore::TArray<ui32> primGroups;
    for( ui32 i = 0; i < geo->m_numPrimGroups; ++i ) {
        const ui32 primIdx( m_oglBackend->addPrimitiveGroup( &geo->m_pPrimGroups[ i ] ) );
        primGroups.add( primIdx );
    }

    // create the default material
    setupMaterial( geo->m_pMaterial, m_oglBackend, this );

    // setup vertex array, vertex and index buffers
    m_pVertexArray = setupBuffers( geo, m_oglBackend, m_renderCmdBuffer->getActiveShader() );
    if( !m_pVertexArray ) {
        ce_debug( "Vertex-Array-pointer is a nullptr." );
        return false;
    }
    m_renderCmdBuffer->setVertexArray( m_pVertexArray );
            
    // setup global parameter
    setupParameter( geo, m_oglBackend, this );

    // setup the draw calls
    if( 0 == attachSceneEvData->m_numInstances ) {
        setupPrimDrawCmd( primGroups, m_oglBackend, this );
    } else {
        setupInstancedDrawCmd( primGroups, attachSceneEvData, m_oglBackend, this );
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
bool OGLRenderEventHandler::onClearGeo( const EventData * ) {
    m_oglBackend->releaseAllBuffers();
    m_oglBackend->releaseAllShaders();
    m_oglBackend->releaseAllTextures();
    m_oglBackend->releaseAllParameters();
    m_renderCmdBuffer->clear();

    return true;
}

//-------------------------------------------------------------------------------------------------
bool OGLRenderEventHandler::onRenderFrame( const EventData *pEventData ) {
    ce_assert( nullptr != m_oglBackend );

    if ( !m_pRenderCtx ) {
        return false;
    }

    m_renderCmdBuffer->onPreRenderFrame(); 
    m_renderCmdBuffer->onRenderFrame( pEventData );
    m_renderCmdBuffer->onPostRenderFrame();

    return true;
}

//-------------------------------------------------------------------------------------------------
bool OGLRenderEventHandler::onUpdateParameter( const EventData *pEventData ) {
    UpdateParameterEventData *updateParamData = ( UpdateParameterEventData* ) pEventData;

    if( updateParamData ) {
        for( ui32 i = 0; i < updateParamData->m_numParam; ++i ) {
            OGLParameter *oglParam = m_oglBackend->getParameter( updateParamData->m_param[ i ].m_name );
            if( oglParam ) {
                ::memcpy( oglParam->m_data->getData(), updateParamData->m_param[ i ].m_data.getData(), 
                          updateParamData->m_param[ i ].m_data.m_size );
            }
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace ZFXCE2
