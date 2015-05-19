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
#include <src/RenderBackend/OGLRenderer/RenderCmdBuffer.h>
#include <osre/Platform/AbstractRenderContext.h>
#include "OGLCommon.h"
#include "OGLRenderBackend.h"
#include <cassert>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;

//-------------------------------------------------------------------------------------------------
RenderCmdBuffer::RenderCmdBuffer( OGLRenderBackend *pRenderBackend, AbstractRenderContext *ctx )
: m_pRenderBackend( pRenderBackend )
, m_pRenderCtx( ctx )
, m_pActiveShader( nullptr )
, m_pVertexArray( nullptr )
, m_primitives()
, m_materials()
, m_param( nullptr ) {
    assert( nullptr != m_pRenderBackend );
    assert( nullptr != m_pRenderCtx );

    m_param = new OGLParameter;
    m_param->m_name = "tex0";
    m_param->m_type = PT_Int;
    m_param->m_data = ParamDataBlob::create( PT_Int, 1 );
}

//-------------------------------------------------------------------------------------------------
RenderCmdBuffer::~RenderCmdBuffer() {
    ContainerClear( m_cmdbuffer );

    m_pRenderBackend = nullptr;
    m_pRenderCtx = nullptr;

    delete m_param;
    m_param = nullptr;
}

//-------------------------------------------------------------------------------------------------
void RenderCmdBuffer::setVertexArray( OGLVertexArray *pVertexArray ) {
    m_pVertexArray = pVertexArray;
}

//-------------------------------------------------------------------------------------------------
void RenderCmdBuffer::setActiveShader( OGLShader *pOGLShader ) {
    m_pActiveShader = pOGLShader;
}

//-------------------------------------------------------------------------------------------------
OGLShader *RenderCmdBuffer::getActiveShader() const {
    return m_pActiveShader;
}

//-------------------------------------------------------------------------------------------------
void RenderCmdBuffer::enqueueRenderCmd( OGLRenderCmd *pOGLRenderCmd, EnqueueType type ) {
    m_cmdbuffer.add( pOGLRenderCmd );
}

//-------------------------------------------------------------------------------------------------
bool RenderCmdBuffer::onPreRenderFrame() {
    assert( nullptr != m_pRenderBackend );

    if( !m_pRenderCtx ) {
        return false;
    }

    m_pRenderCtx->activate();
    m_pRenderBackend->clearRenderTarget( ColorBit | DepthBit );
    
    // use base shader
    m_pRenderBackend->useShader( m_pActiveShader );
    if( m_pVertexArray ) {
        m_pRenderBackend->bindVertexArray( m_pVertexArray );
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
bool RenderCmdBuffer::onRenderFrame( const EventData *pEventData ) {
    assert( nullptr != m_pRenderBackend );

    for( ui32 i = 0; i < m_cmdbuffer.size(); ++i ) {
        OGLRenderCmd *pRenderCmd = m_cmdbuffer[ i ];
        if( pRenderCmd->m_type == SetParameterCmd ) {
            onSetParametersCmd( ( SetParameterCmdData* ) pRenderCmd->m_pData );
        } else if( pRenderCmd->m_type == DrawPrimitivesCmd ) {
            onDrawPrimitivesCmd( ( DrawPrimitivesCmdData* ) pRenderCmd->m_pData );
        } else if( pRenderCmd->m_type == DrawPrimitivesInstancesCmd ) {
            onDrawPrimitivesInstancesCmd( ( DrawInstancePrimitivesCmdData* ) pRenderCmd->m_pData );
        } else if( pRenderCmd->m_type == SetTextureCmd ) {
            onSetTextureStageCmd( ( SetTextureStageCmdData* ) pRenderCmd->m_pData );
        } else if( pRenderCmd->m_type == SetShaderCmd ) {
            onSetShaderStageCmd( ( SetShaderStageCmdData* ) pRenderCmd->m_pData );
        } else if( pRenderCmd->m_type == SetRenderTargetCmd ) {
            onSetRenderTargetCmd( ( SetRenderTargetCmdData* ) pRenderCmd->m_pData );
        } else {
            osre_error( Tag, "Unsupported render command type: " + pRenderCmd->m_type );
        }
    }

    // unbind the shader
    m_pRenderBackend->useShader( nullptr );

    m_pRenderBackend->renderFrame();

    return true;
}

//-------------------------------------------------------------------------------------------------
bool RenderCmdBuffer::onPostRenderFrame() {
    return true;
}

//-------------------------------------------------------------------------------------------------
void RenderCmdBuffer::clear() {
    if( !m_cmdbuffer.isEmpty() ) {
        m_cmdbuffer.clear();
    }
}

//-------------------------------------------------------------------------------------------------
bool RenderCmdBuffer::onUpdateParameter( const EventData *data ) {
    UpdateParameterEventData *updateParamData = ( UpdateParameterEventData* ) data;

    if( updateParamData ) {
        for( ui32 i = 0; i < updateParamData->m_numParam; ++i ) {
            OGLParameter *oglParam = m_pRenderBackend->getParameter( updateParamData->m_param[ i ].m_name );
            if( oglParam ) {
                ::memcpy( oglParam->m_data->getData(), updateParamData->m_param[ i ].m_data.getData(), updateParamData->m_param[ i ].m_data.m_size );
            }
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
bool RenderCmdBuffer::onSetParametersCmd( SetParameterCmdData *data ) {
    m_pRenderBackend->setParameter( data->m_param, data->m_numParam );

    return true;
}

//-------------------------------------------------------------------------------------------------
bool RenderCmdBuffer::onDrawPrimitivesCmd( DrawPrimitivesCmdData *pData ) {
    for( ui32 i = 0; i < pData->m_primitives.size(); ++i ) {
        m_pRenderBackend->render( pData->m_primitives[ i ] );
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
bool RenderCmdBuffer::onDrawPrimitivesInstancesCmd( DrawInstancePrimitivesCmdData *data ) {
    for( ui32 i = 0; i < data->m_primitives.size(); ++i ) {
        m_pRenderBackend->render( data->m_primitives[ i ], data->m_numInstances );
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
bool RenderCmdBuffer::onSetTextureStageCmd( SetTextureStageCmdData *data ) {
    for( ui32 i = 0; i < data->m_textures.size(); ++i ) {
        OGLTexture *pOGLTexture = data->m_textures[ i ];
        if( pOGLTexture ) {
            m_pRenderBackend->bindTexture( pOGLTexture, TextureStage0 );
            ::memcpy( m_param->m_data->getData(), &i, sizeof( ui32 ) );
            m_pRenderBackend->setParameter( m_param );
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
bool RenderCmdBuffer::onSetShaderStageCmd( SetShaderStageCmdData *data ) {
    if( data->m_pShader ) {
        m_pRenderBackend->useShader( data->m_pShader );
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
bool RenderCmdBuffer::onSetRenderTargetCmd( SetRenderTargetCmdData *pData ) {
    return true;
}

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace OSRE
