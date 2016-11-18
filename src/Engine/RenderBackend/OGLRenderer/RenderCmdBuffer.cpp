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
#include <src/Engine/RenderBackend/OGLRenderer/RenderCmdBuffer.h>
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
, m_materials() {
    OSRE_ASSERT( nullptr != m_renderbackend );
    OSRE_ASSERT( nullptr != m_renderCtx );

    m_clearState.m_state = ClearState::ColorBit | ClearState::DepthBit;
}

RenderCmdBuffer::~RenderCmdBuffer() {
    clear();

    m_renderbackend = nullptr;
    m_renderCtx = nullptr;
}

void RenderCmdBuffer::setActiveShader( OGLShader *oglShader ) {
    m_activeShader = oglShader;
}

OGLShader *RenderCmdBuffer::getActiveShader() const {
    return m_activeShader;
}

void RenderCmdBuffer::enqueueRenderCmd( OGLRenderCmd *renderCmd, EnqueueType type ) {
    if ( nullptr == renderCmd ) {
        return;
    }
    if ( nullptr == renderCmd->m_pData ) {
        osre_debug( Tag, "Nullptr in rener command data detected." );
    }
    if ( EnqueueType::RCE_Back == type  ) {
        m_cmdbuffer.add( renderCmd );
    }
}

void RenderCmdBuffer::onPreRenderFrame() {
    OSRE_ASSERT( nullptr!=m_renderbackend );

    if( nullptr == m_renderCtx ) {
        return;
    }

    m_renderCtx->activate();
    m_renderbackend->clearRenderTarget( m_clearState );
}

void RenderCmdBuffer::onRenderFrame( const EventData *eventData ) {
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
        } else {
            osre_error( Tag, "Unsupported render command type: " + static_cast<ui32>( renderCmd->m_type ) );
        }
    }

    m_renderbackend->renderFrame();
}

void RenderCmdBuffer::onPostRenderFrame() {
    OSRE_ASSERT( nullptr != m_renderbackend );

    // unbind the active shader
    m_renderbackend->useShader( nullptr );
}

void RenderCmdBuffer::clear() {
    ContainerClear( m_cmdbuffer );
}

bool RenderCmdBuffer::onUpdateParameter( const EventData *data ) {
    UpdateParameterEventData *updateParamData = ( UpdateParameterEventData* ) data;
    if( nullptr != updateParamData ) {
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

    if ( nullptr == data ) {
        return false;
    }

    m_renderbackend->setParameter( data->m_param, data->m_numParam );

    return true;
}

bool RenderCmdBuffer::onDrawPrimitivesCmd( DrawPrimitivesCmdData *data ) {
    OSRE_ASSERT( nullptr != m_renderbackend );
    if ( nullptr == data ) {
        return false;
    }

    m_renderbackend->bindVertexArray( data->m_vertexArray );
    for( ui32 i = 0; i < data->m_primitives.size(); ++i ) {
        m_renderbackend->render( data->m_primitives[ i ] );
    }

    return true;
}

bool RenderCmdBuffer::onDrawPrimitivesInstancesCmd( DrawInstancePrimitivesCmdData *data ) {
    OSRE_ASSERT( nullptr != m_renderbackend );
    if ( nullptr == data ) {
        return false;
    }

    m_renderbackend->bindVertexArray( data->m_vertexArray );
    for( ui32 i = 0; i < data->m_primitives.size(); i++ ) {
        m_renderbackend->render( data->m_primitives[ i ], data->m_numInstances );
    }

    return true;
}

bool RenderCmdBuffer::onSetRenderTargetCmd( SetRenderTargetCmdData *data ) {
    return true;
}

bool RenderCmdBuffer::onSetMaterialStageCmd( SetMaterialStageCmdData *data ) {
    OSRE_ASSERT( nullptr != m_renderbackend );

    m_renderbackend->useShader( data->m_shader );
    for ( ui32 i = 0; i < data->m_textures.size(); ++i ) {
        OGLTexture *oglTexture = data->m_textures[ i ];
        if ( nullptr != oglTexture ) {
            m_renderbackend->bindTexture( oglTexture, (TextureStageType) i );
        }
    }

    if ( nullptr != data->m_param ) {
        m_renderbackend->setParameter( data->m_param, data->m_numParam );
    }

    return true;
}

} // Namespace RenderBackend
} // Namespace OSRE
