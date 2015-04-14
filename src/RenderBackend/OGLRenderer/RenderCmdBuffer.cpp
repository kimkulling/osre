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
#include <Code/RenderSystem/RenderBackend/OGLRenderer/RenderCmdBuffer.h>
#include <zfxce2/Infrastructure/Debugging/ce_assert.h>
#include <zfxce2/Infrastructure/Platform/AbstractRenderContext.h>

#include "OGLCommon.h"
#include "OGLRenderBackend.h"

namespace ZFXCE2 {
namespace RenderBackend {

using namespace ::ZFXCE2::Core;
using namespace ::ZFXCE2::Platform;

//-------------------------------------------------------------------------------------------------
RenderCmdBuffer::RenderCmdBuffer( OGLRenderBackend *pRenderBackend, AbstractRenderContext *ctx )
: m_pRenderBackend( pRenderBackend )
, m_pRenderCtx( ctx )
, m_pActiveShader( nullptr )
, m_pVertexArray( nullptr )
, m_primitives()
, m_materials()
, m_param( nullptr ) {
    ce_assert( nullptr != m_pRenderBackend );
    ce_assert( nullptr != m_pRenderCtx );

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
    ce_assert( nullptr != m_pRenderBackend );

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
    ce_assert( nullptr != m_pRenderBackend );

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
            ce_error( "Unsupported render command type: " + pRenderCmd->m_type );
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
} // Namespace ZFXCE2
