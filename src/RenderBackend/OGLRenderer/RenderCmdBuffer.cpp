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
#include <src/RenderBackend/OGLRenderer/RenderCmdBuffer.h>
#include <osre/RenderBackend/TextRenderer.h>
#include <osre/Platform/AbstractRenderContext.h>
#include <osre/Debugging/osre_debugging.h>
#include "OGLCommon.h"
#include "OGLRenderBackend.h"

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;

static const String Tag = "RenderCmdBuffer";

RenderCmdBuffer::RenderCmdBuffer( OGLRenderBackend *renderBackend, AbstractRenderContext *ctx )
: m_renderbackend( renderBackend )
, m_renderCtx( ctx )
, m_activeShader( nullptr )
, m_primitives()
, m_materials()
, m_param( nullptr ) {
    OSRE_ASSERT( nullptr != m_renderbackend );
    OSRE_ASSERT( nullptr != m_renderCtx );

    // ???
    m_param = new OGLParameter;
    m_param->m_name = "tex0";
    m_param->m_type = PT_Int;
    m_param->m_data = ParamDataBlob::create( PT_Int, 1 );

    m_clearState.m_state = ClearState::ColorBit | ClearState::DepthBit;
}

RenderCmdBuffer::~RenderCmdBuffer() {
    ContainerClear( m_cmdbuffer );

    m_renderbackend = nullptr;
    m_renderCtx = nullptr;

	delete m_param;
    m_param = nullptr;
}

void RenderCmdBuffer::setActiveShader( OGLShader *oglShader ) {
    m_activeShader = oglShader;
}

OGLShader *RenderCmdBuffer::getActiveShader() const {
    return m_activeShader;
}

void RenderCmdBuffer::enqueueRenderCmd( OGLRenderCmd *renderCmd, EnqueueType type ) {
    m_cmdbuffer.add( renderCmd );
}

bool RenderCmdBuffer::onPreRenderFrame() {
    OSRE_ASSERT( nullptr!=m_renderbackend );

    if( !m_renderCtx ) {
        return false;
    }

    m_renderCtx->activate();
    m_renderbackend->clearRenderTarget( m_clearState );
    
    return true;
}

bool RenderCmdBuffer::onRenderFrame( const EventData *eventData ) {
    OSRE_ASSERT( nullptr!=m_renderbackend );

    for( ui32 i = 0; i < m_cmdbuffer.size(); ++i ) {
        // only valid pointers are allowed
        OGLRenderCmd *renderCmd = m_cmdbuffer[ i ];
        OSRE_ASSERT( nullptr != renderCmd );

        if( renderCmd->m_type == OGLRenderCmdType::SetParameterCmd ) {
            onSetParametersCmd( ( SetParameterCmdData* ) renderCmd->m_pData );
        } else if( renderCmd->m_type == OGLRenderCmdType::DrawPrimitivesCmd ) {
            onDrawPrimitivesCmd( ( DrawPrimitivesCmdData* ) renderCmd->m_pData );
        } else if( renderCmd->m_type == OGLRenderCmdType::DrawPrimitivesInstancesCmd ) {
            onDrawPrimitivesInstancesCmd( ( DrawInstancePrimitivesCmdData* ) renderCmd->m_pData );
		} else if ( renderCmd->m_type == OGLRenderCmdType::SetRenderTargetCmd) {
			onSetRenderTargetCmd( ( SetRenderTargetCmdData* ) renderCmd->m_pData);
        } else if ( renderCmd->m_type == OGLRenderCmdType::SetMaterialCmd ) {
            onSetMaterialStageCmd( ( SetMaterialStageCmdData* ) renderCmd->m_pData );
        }
        else {
            osre_error( Tag, "Unsupported render command type: " + static_cast<ui32>( renderCmd->m_type ) );
        }
    }

    // unbind the shader
    m_renderbackend->useShader( nullptr );

    m_renderbackend->renderFrame();

    return true;
}

bool RenderCmdBuffer::onPostRenderFrame() {
    return true;
}

void RenderCmdBuffer::clear() {
    if( !m_cmdbuffer.isEmpty() ) {
        m_cmdbuffer.clear();
    }
}

bool RenderCmdBuffer::onUpdateParameter( const EventData *data ) {
    UpdateParameterEventData *updateParamData = ( UpdateParameterEventData* ) data;

    if( updateParamData ) {
        for( ui32 i = 0; i < updateParamData->m_numParam; ++i ) {
            OGLParameter *oglParam = m_renderbackend->getParameter( updateParamData->m_param[ i ].m_name );
            if( oglParam ) {
                ::memcpy( oglParam->m_data->getData(), updateParamData->m_param[ i ].m_data.getData(), updateParamData->m_param[ i ].m_data.m_size );
            }
        }
    }

    return true;
}

bool RenderCmdBuffer::onSetParametersCmd( SetParameterCmdData *data ) {
    OSRE_ASSERT( nullptr != m_renderbackend );

    m_renderbackend->setParameter( data->m_param, data->m_numParam );

    return true;
}

bool RenderCmdBuffer::onDrawPrimitivesCmd( DrawPrimitivesCmdData *data ) {
    OSRE_ASSERT( nullptr != m_renderbackend );

    m_renderbackend->bindVertexArray( data->m_vertexArray );
    for( ui32 i = 0; i < data->m_primitives.size(); ++i ) {
        m_renderbackend->render( data->m_primitives[ i ] );
    }

    return true;
}

bool RenderCmdBuffer::onDrawPrimitivesInstancesCmd( DrawInstancePrimitivesCmdData *data ) {
    OSRE_ASSERT( nullptr != m_renderbackend );
    
    m_renderbackend->bindVertexArray( data->m_vertexArray );
    for( ui32 i = 0; i < data->m_primitives.size(); ++i ) {
        m_renderbackend->render( data->m_primitives[ i ], data->m_numInstances );
    }

    return true;
}

bool RenderCmdBuffer::onSetRenderTargetCmd( SetRenderTargetCmdData *data ) {
    return true;
}

bool RenderCmdBuffer::onSetMaterialStageCmd( SetMaterialStageCmdData *data ) {
    OSRE_ASSERT( nullptr != m_renderbackend );

    if ( data->m_shader ) {
        m_renderbackend->useShader( data->m_shader );
    }

    for ( ui32 i = 0; i < data->m_textures.size(); ++i ) {
        OGLTexture *pOGLTexture = data->m_textures[ i ];
        if ( pOGLTexture ) {
            m_renderbackend->bindTexture( pOGLTexture, TextureStage0 );
            ::memcpy( m_param->m_data->getData(), &i, sizeof( ui32 ) );
            m_renderbackend->setParameter( m_param );
        }
    }

    //m_renderbackend->setParameter( data->m_param, data->m_numParam );

    return true;
}

} // Namespace RenderBackend
} // Namespace OSRE
