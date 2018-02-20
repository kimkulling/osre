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

RenderCmdBuffer::RenderCmdBuffer( OGLRenderBackend *renderBackend, AbstractRenderContext *ctx, Pipeline *pipeline )
: m_renderbackend( renderBackend )
, m_renderCtx( ctx )
, m_activeShader( nullptr )
, m_primitives()
, m_materials()
, m_paramArray()
, m_pipeline( pipeline ) {
    OSRE_ASSERT( nullptr != m_renderbackend );
    OSRE_ASSERT( nullptr != m_renderCtx );
    OSRE_ASSERT( nullptr != m_pipeline );

    m_clearState.setClearState( (int) ClearState::ClearBitType::ColorBit | (int) ClearState::ClearBitType::DepthBit );
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

void RenderCmdBuffer::enqueueRenderCmd( const String &groupName, OGLRenderCmd *renderCmd, EnqueueType type ) {
    if ( nullptr == renderCmd ) {
        osre_debug( Tag, "Nullptr to render-command detected." );
        return;
    }
    if ( nullptr == renderCmd->m_data ) {
        osre_debug( Tag, "Nullptr in render-command data detected." );
        return;
    }

    if ( EnqueueType::PushBack == type ) {
        m_cmdbuffer.add( renderCmd );
    }
}

void RenderCmdBuffer::enqueueRenderCmdGroup( const String &groupName, CPPCore::TArray<OGLRenderCmd*>& cmdGroup, 
        EnqueueType type ) {
    if ( groupName.empty() ) {
        osre_debug( Tag, "No name for render command group defined." );
        return;
    }

    if ( cmdGroup.isEmpty() ) {
        osre_debug( Tag, "Render command group is empty." );
        return;
    }

    if ( EnqueueType::PushBack == type ) {
        m_cmdbuffer.add( &cmdGroup[ 0 ], cmdGroup.size() );
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

    ui32 numPasses = m_pipeline->beginFrame();

    for ( ui32 passId = 0; passId < numPasses; passId++ ) {
        PipelinePass *pass = m_pipeline->beginPass( passId );
        if ( nullptr == pass ) {
            osre_debug(Tag, "Ponter to pipeline pass is nullptr.");
            continue;
        }
        PipelineStates states;
        states.m_polygonState = pass->getPolygonState();
        states.m_cullState = pass->getCullState();
        states.m_blendState = pass->getBlendState();
        states.m_samplerState = pass->getSamplerState();
        states.m_stencilState = pass->getStencilState();
        m_renderbackend->setFixedPipelineStates(states);
        for ( ui32 i = 0; i < m_cmdbuffer.size(); ++i ) {
            // only valid pointers are allowed
            OGLRenderCmd *renderCmd = m_cmdbuffer[ i ];
            OSRE_ASSERT( nullptr != renderCmd );

            if ( renderCmd->m_type == OGLRenderCmdType::DrawPrimitivesCmd ) {
                onDrawPrimitivesCmd( ( DrawPrimitivesCmdData* ) renderCmd->m_data );
            } else if ( renderCmd->m_type == OGLRenderCmdType::DrawPrimitivesInstancesCmd ) {
                onDrawPrimitivesInstancesCmd( ( DrawInstancePrimitivesCmdData* ) renderCmd->m_data );
            } else if ( renderCmd->m_type == OGLRenderCmdType::SetRenderTargetCmd ) {
                onSetRenderTargetCmd( ( SetRenderTargetCmdData* ) renderCmd->m_data );
            } else if ( renderCmd->m_type == OGLRenderCmdType::SetMaterialCmd ) {
                onSetMaterialStageCmd( ( SetMaterialStageCmdData* ) renderCmd->m_data );
            } else {
                osre_error( Tag, "Unsupported render command type: " + static_cast< ui32 >( renderCmd->m_type ) );
            }
        }

        m_pipeline->endPass( passId );
    }
    m_pipeline->endFrame();

    m_renderbackend->renderFrame();
}

void RenderCmdBuffer::onPostRenderFrame() {
    OSRE_ASSERT( nullptr != m_renderbackend );

    // unbind the active shader
    m_renderbackend->useShader( nullptr );
    m_renderbackend->unbindVertexArray();
}

void RenderCmdBuffer::clear() {
    ContainerClear( m_cmdbuffer );
    m_paramArray.resize(0);
}

static bool hasParam( const String &name, const ::CPPCore::TArray<OGLParameter*> &paramArray ) {
    for ( ui32 i = 0; i < paramArray.size(); i++ ) {
        if ( name == paramArray[ i ]->m_name ) {
            return true;
        }
    }
    return false;
}

void RenderCmdBuffer::setParameter( OGLParameter* param ) {
    if ( !hasParam( param->m_name, m_paramArray ) ) {
        m_paramArray.add( param );
    }
}

void RenderCmdBuffer::setParameter( const ::CPPCore::TArray<OGLParameter*> &paramArray ) {
    for ( ui32 i = 0; i < paramArray.size(); i++ ) {
        if ( !hasParam( paramArray[ i ]->m_name, m_paramArray ) ) {
            m_paramArray.add( paramArray[ i ] );
        }
    }
}

void RenderCmdBuffer::commitParameters() {
    for ( ui32 i = 0; i < m_paramArray.size(); i++ ) {
        m_renderbackend->setParameter( m_paramArray[ i ] );
    }
}

bool RenderCmdBuffer::onDrawPrimitivesCmd( DrawPrimitivesCmdData *data ) {
    OSRE_ASSERT( nullptr != m_renderbackend );
    if ( nullptr == data ) {
        return false;
    }

    m_renderbackend->bindVertexArray( data->m_vertexArray );
    if ( data->m_numParam > 0 ) {
        for ( ui32 i = 0; i < data->m_numParam; i++ ) {
            m_renderbackend->setParameter( m_paramArray[ i ] );
        }
    }

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
    OSRE_ASSERT( nullptr != m_renderbackend );

    return true;
}

bool RenderCmdBuffer::onSetMaterialStageCmd( SetMaterialStageCmdData *data ) {
    OSRE_ASSERT( nullptr != m_renderbackend );

    m_renderbackend->bindVertexArray( data->m_vertexArray );
    m_renderbackend->useShader( data->m_shader );
    
    commitParameters();

    for ( ui32 i = 0; i < data->m_textures.size(); ++i ) {
        OGLTexture *oglTexture = data->m_textures[ i ];
        if ( nullptr != oglTexture ) {
            m_renderbackend->bindTexture( oglTexture, (TextureStageType) i );
        }
    }

    return true;
}

} // Namespace RenderBackend
} // Namespace OSRE
