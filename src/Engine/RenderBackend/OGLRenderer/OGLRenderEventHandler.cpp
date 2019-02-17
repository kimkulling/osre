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
#include "OGLRenderEventHandler.h"
#include "OGLRenderBackend.h"
#include "OGLShader.h"
#include "OGLCommon.h"
#include "RenderCmdBuffer.h"

#include <osre/Common/Logger.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/AbstractRenderContext.h>
#include <osre/Profiling/PerformanceCounterRegistry.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Shader.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/HWBufferManager.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/Uri.h>
#include <osre/Assets/AssetRegistry.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::CPPCore;

static const c8 *Tag = "OGLRendeEventHandler";

static bool setupTextures( Material *mat, OGLRenderBackend *rb, TArray<OGLTexture*> &textures ) {
    OSRE_ASSERT( nullptr != mat );
	OSRE_ASSERT( nullptr != rb );

    if ( nullptr == mat ) {
        osre_debug( Tag, "Material is nullptr." );
        return false;
    }

    if ( nullptr == rb ) {
        osre_debug( Tag, "Renderbackend is nullptr." );
        return false;
    }

    const ui32 numTextures( mat->m_numTextures );
    if( 0 == numTextures ) {
        return true;
    }

    for( ui32 i = 0; i < numTextures; ++i ) {
        Texture *tex( mat->m_textures[ i ] );
        if( !tex->m_textureName.empty() ) {
            String root = Assets::AssetRegistry::getPath( "media" );
            String path = Assets::AssetRegistry::resolvePathFromUri( tex->m_loc );
            
            IO::Uri loc( tex->m_loc );
            loc.setPath( path );

            OGLTexture *oglTexture = rb->createTextureFromFile( tex->m_textureName, loc );
            if( nullptr != oglTexture ) {
                textures.add( oglTexture );
            }
        }
    }

    return true;
}

static void setConstantBuffers(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj, 
        OGLRenderBackend *rb, OGLRenderEventHandler *eh) {
    OSRE_ASSERT(nullptr != eh);
    OSRE_ASSERT(nullptr != rb);

    eh->getRenderCmdBuffer()->setMatrixes(model, view, proj);
}

static SetMaterialStageCmdData *setupMaterial( Material *material, OGLRenderBackend *rb, OGLRenderEventHandler *eh ) {
	OSRE_ASSERT( nullptr != eh );
	OSRE_ASSERT( nullptr != material );
	OSRE_ASSERT( nullptr != rb );

    SetMaterialStageCmdData *matData = new SetMaterialStageCmdData;
    switch( material->m_type ) {
        case MaterialType::ShaderMaterial: {
                TArray<OGLTexture*> textures;
                setupTextures( material, rb, textures );
                OGLRenderCmd *renderMatCmd = OGLRenderCmdAllocator::alloc( OGLRenderCmdType::SetMaterialCmd, nullptr );
                if( !textures.isEmpty() ) {
                    matData->m_textures = textures;
                }

                OGLShader *shader = rb->createShader( "mat", material->m_shader );
                if ( nullptr != shader ) {
                    matData->m_shader = shader;
                    for( ui32 i = 0; i < material->m_shader->m_attributes.size(); i++ ) {
                        const String &attribute = material->m_shader->m_attributes[ i ];
                        //if ( shader->hasAttribute( attribute ) ) {
                            shader->addAttribute( attribute );
                        //}
                    }

                    for( ui32 i = 0; i < material->m_shader->m_parameters.size(); i++ ) {
                        shader->addUniform( material->m_shader->m_parameters[ i ] );
                    }

                    // for setting up all buffer objects
                    eh->setActiveShader( shader );
                }
                renderMatCmd->m_data = matData;
                eh->enqueueRenderCmd( renderMatCmd );
            }
            break;

        default:
            break;
    }

    return matData;
}

static void setupParameter( UniformVar *param, OGLRenderBackend *rb, OGLRenderEventHandler *ev ) {
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



static OGLVertexArray *setupBuffers( Mesh *mesh, OGLRenderBackend *rb, OGLShader *oglShader ) {
	OSRE_ASSERT( nullptr != mesh );
	OSRE_ASSERT( nullptr != rb );
	OSRE_ASSERT( nullptr != oglShader );

    rb->useShader( oglShader );

    OGLVertexArray *vertexArray = rb->createVertexArray();
    rb->bindVertexArray( vertexArray );
    BufferData *vertices = mesh->m_vb;
	if ( nullptr == vertices ) {
		osre_debug( Tag, "No vertex buffer data for setting up data." );
		return nullptr;
    }

    BufferData *indices = mesh->m_ib;
    if ( nullptr == indices ) {
        osre_debug( Tag, "No index buffer data for setting up data." );
        return nullptr;
    }

    // create vertex buffer and  and pass triangle vertex to buffer object
    OGLBuffer *vb = rb->createBuffer( vertices->m_type );
    vb->m_geoId = mesh->m_id;
    rb->bindBuffer( vb );
    rb->copyDataToBuffer( vb, vertices->getData(), vertices->getSize(), vertices->m_access );

    // enable vertex attribute arrays
    TArray<OGLVertexAttribute*> attributes;
    rb->createVertexCompArray( mesh->m_vertextype, oglShader, attributes );
    const ui32 stride = Mesh::getVertexSize( mesh->m_vertextype );
    rb->bindVertexLayout( vertexArray, oglShader, stride, attributes );
    rb->releaseVertexCompArray( attributes );

    // create index buffer and pass indices to element array buffer
    OGLBuffer *ib = rb->createBuffer( indices->m_type );
    ib->m_geoId = mesh->m_id;
    rb->bindBuffer( ib );
    rb->copyDataToBuffer( ib, indices->getData(), indices->getSize(), indices->m_access );

    rb->unbindVertexArray();

    return vertexArray;
}

static void setupPrimDrawCmd(const char *id, bool useLocalMatrix, const glm::mat4 &model, const TArray<ui32> &primGroups, OGLRenderBackend *rb, 
        OGLRenderEventHandler *eh, OGLVertexArray *va ) {
	OSRE_ASSERT( nullptr != rb );
	OSRE_ASSERT( nullptr != eh );

    if( primGroups.isEmpty() ) {
        return;
    }

	OGLRenderCmd *renderCmd = OGLRenderCmdAllocator::alloc( OGLRenderCmdType::DrawPrimitivesCmd, nullptr );
    DrawPrimitivesCmdData *data = new DrawPrimitivesCmdData;
    if ( useLocalMatrix ) {
        data->m_model = model;
        data->m_localMatrix = useLocalMatrix;
    }
    data->m_id = id;
    data->m_vertexArray = va;
    data->m_primitives.reserve( primGroups.size() );
    for( ui32 i = 0; i < primGroups.size(); ++i ) {
        data->m_primitives.add( primGroups[ i ] );
    }
    renderCmd->m_data = static_cast<void*>( data );
    
    eh->enqueueRenderCmd( renderCmd );
}

static void setupInstancedDrawCmd(const char *id, const TArray<ui32> &ids, OGLRenderBackend *rb, OGLRenderEventHandler *eh, OGLVertexArray *va, ui32 numInstances ) {
	OSRE_ASSERT( nullptr != rb );
	OSRE_ASSERT( nullptr != eh );

    if( ids.isEmpty() ) {
        return;
    }

	OGLRenderCmd *renderCmd = OGLRenderCmdAllocator::alloc( OGLRenderCmdType::DrawPrimitivesInstancesCmd, nullptr );
    
    DrawInstancePrimitivesCmdData *data = new DrawInstancePrimitivesCmdData;
    data->m_vertexArray = va;
    data->m_numInstances = numInstances;
    data->m_primitives.reserve( ids.size() );
    for( ui32 j = 0; j < ids.size(); ++j ) {
        data->m_primitives.add( ids[ j ] );
    }
    renderCmd->m_data = static_cast< void* >( data );
    eh->enqueueRenderCmd( renderCmd );
}

OGLRenderEventHandler::OGLRenderEventHandler( )
: AbstractEventHandler()
, m_isRunning ( true )
, m_oglBackend( nullptr )
, m_renderCmdBuffer( nullptr )
, m_renderCtx( nullptr )
, m_vertexArray( nullptr )
, m_hwBufferManager( nullptr ) {
    // empty
}
        
OGLRenderEventHandler::~OGLRenderEventHandler( ) {
    delete m_hwBufferManager;
    m_hwBufferManager = nullptr;
}

bool OGLRenderEventHandler::onEvent( const Event &ev, const EventData *data ) {
    bool result( false );
    if ( !m_isRunning ) {
        return true;
    }

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
    } else if (OnRenderFrameEvent == ev) {
        result = onRenderFrame(data);
    } else if ( OnInitPassesEvent == ev) {
        result = onInitRenderPasses(data);
    } else if ( OnCommitFrameEvent == ev ) {
        result = onCommitNexFrame( data );
    } else if ( OnClearSceneEvent == ev ) {
        result = onClearGeo( data );
    } else if ( OnShutdownRequest == ev ) {
        result = onShutdownRequest( data );
    } else if ( OnResizeEvent == ev ) {
        result = onResizeRenderTarget( data );
    }
 
    return result;
}

void OGLRenderEventHandler::setActiveShader( OGLShader *oglShader ) {
    m_renderCmdBuffer->setActiveShader( oglShader );
}

void OGLRenderEventHandler::enqueueRenderCmd( OGLRenderCmd *oglRenderCmd ) {
    m_renderCmdBuffer->enqueueRenderCmd( "pass0", oglRenderCmd );
}

void OGLRenderEventHandler::setParameter( const ::CPPCore::TArray<OGLParameter*> &paramArray ) {
    m_renderCmdBuffer->setParameter( paramArray );
}

bool OGLRenderEventHandler::onAttached( const EventData* ) {
    if( nullptr != m_oglBackend ) {
        return false;
    }

    m_oglBackend = new OGLRenderBackend;
    m_oglBackend->setTimer( PlatformInterface::getInstance()->getTimer() );
    m_hwBufferManager = new HWBufferManager;

    return true;
}

bool OGLRenderEventHandler::onDetached( const EventData* ) {
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
    AbstractWindow *activeSurface = createRendererEvData->m_activeSurface;
    if( nullptr == activeSurface ) {
        osre_debug( Tag, "No active surface, pointer is nullptr." );
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
    if ( !m_renderCtx->isActive() ) {
        osre_debug( Tag, "Error while activafing render context." );
        return false;
    }

    Rect2ui rect = activeSurface->getWindowsRect();
    m_oglBackend->setViewport( rect.m_x1, rect.m_y1, rect.m_width, rect.m_height );

    const String defaultFont( PlatformInterface::getInstance()->getDefaultFontName() );
    IO::Uri fontUri( "file://assets/Textures/Fonts/" + defaultFont );
    String root = Assets::AssetRegistry::getPath( "media" );
    String path = Assets::AssetRegistry::resolvePathFromUri( fontUri );
    fontUri.setPath( path );
    m_oglBackend->createFont( fontUri );
    m_renderCmdBuffer = new RenderCmdBuffer( m_oglBackend, m_renderCtx, createRendererEvData->m_pipeline );

    bool ok( Profiling::PerformanceCounterRegistry::create() );
    if ( !ok ) {
        osre_error( Tag, "Error while destroying performance counters." );
        return false;
    }

    Profiling::PerformanceCounterRegistry::registerCounter( "fps" );

    return true;
}

bool OGLRenderEventHandler::onDestroyRenderer( const Common::EventData * ) {
	OSRE_ASSERT( nullptr != m_oglBackend );
	
	if ( nullptr != m_renderCtx ) {
        return false;
    }

    bool ok( Profiling::PerformanceCounterRegistry::destroy() );
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

    OSRE_ASSERT(nullptr != m_renderCmdBuffer);
    m_renderCmdBuffer->onPreRenderFrame();
    m_renderCmdBuffer->onRenderFrame( eventData );
    m_renderCmdBuffer->onPostRenderFrame();

    return true;
}

bool OGLRenderEventHandler::onInitRenderPasses( const Common::EventData *eventData ) {
    OSRE_ASSERT( nullptr != m_oglBackend );

    InitPassesEventData *frameToCommitData = (InitPassesEventData* ) eventData;
    if ( nullptr == frameToCommitData ) {
        return false;
    }
    
    CPPCore::TArray<ui32> primGroups;
    Frame *frame = frameToCommitData->m_frame;
    
    for (ui32 passIdx = 0; passIdx < frame->m_newPasses.size(); ++passIdx) {
        PassData *currentPass = frame->m_newPasses[passIdx];
        OSRE_ASSERT(nullptr != currentPass);
        
        if (!currentPass->m_isDirty) {
            continue;
        }
        // ToDo: create pipeline pass for the name.
        for (ui32 batchIdx = 0; batchIdx < currentPass->m_geoBatches.size(); ++batchIdx) {
            GeoBatchData *currentBatchData = currentPass->m_geoBatches[passIdx];
            OSRE_ASSERT(nullptr != currentBatchData);

            // set the matrix
            MatrixBuffer &matrixBuffer = currentBatchData->m_matrixBuffer;
            setConstantBuffers(matrixBuffer.m_model, matrixBuffer.m_view, matrixBuffer.m_proj, m_oglBackend, this );
            
            // set uniforms
            for (ui32 uniformIdx = 0; uniformIdx < currentBatchData->m_uniforms.size(); ++uniformIdx) {
                setupParameter(currentBatchData->m_uniforms[uniformIdx], m_oglBackend, this);
            }

            // set meshes
            for (ui32 meshEntryIdx = 0; meshEntryIdx < currentBatchData->m_meshArray.size(); ++meshEntryIdx) {
                MeshEntry *currentMeshEntry = currentBatchData->m_meshArray[meshEntryIdx];
                OSRE_ASSERT(nullptr != currentMeshEntry);
                if (!currentMeshEntry->m_isDirty) {
                    continue;
                }

                for (ui32 meshIdx = 0; meshIdx < currentMeshEntry->m_geo.size(); ++meshIdx) {
                    Mesh *currentMesh = currentMeshEntry->m_geo[meshIdx];
                    OSRE_ASSERT(nullptr != currentMesh);

                    // register primitive groups to render
                    for (ui32 i = 0; i < currentMesh->m_numPrimGroups; ++i) {
                        const ui32 primIdx(m_oglBackend->addPrimitiveGroup(&currentMesh->m_primGroups[i]));
                        primGroups.add(primIdx);
                    }

                    // create the default material
                    SetMaterialStageCmdData *data = setupMaterial(currentMesh->m_material, m_oglBackend, this);

                    // setup vertex array, vertex and index buffers
                    m_vertexArray = setupBuffers(currentMesh, m_oglBackend, m_renderCmdBuffer->getActiveShader());
                    if (nullptr == m_vertexArray) {
                        osre_debug(Tag, "Vertex-Array-pointer is a nullptr.");
                        return false;
                    }
                    data->m_vertexArray = m_vertexArray;

                    // setup the draw calls
                    if (0 == currentMeshEntry->numInstances) {
                        setupPrimDrawCmd(currentBatchData->m_id, currentMesh->m_localMatrix, currentMesh->m_model, primGroups, m_oglBackend, this, m_vertexArray);
                    } else {
                        setupInstancedDrawCmd(currentBatchData->m_id, primGroups, m_oglBackend, this, m_vertexArray, currentMeshEntry->numInstances);
                    }
                    
                    primGroups.resize(0);
                }
                currentMeshEntry->m_isDirty = false;
            }
        }
    }

    frame->m_newPasses.clear();

    m_oglBackend->useShader( nullptr );

    return true;
}

bool OGLRenderEventHandler::onCommitNexFrame(const Common::EventData *eventData) {

    CommitFrameEventData *data = (CommitFrameEventData*)eventData;
    if (nullptr == data) {
        return false;
    }

    for (ui32 i = 0; i < data->m_frame->m_submitCmds.size(); ++i) {
        FrameSubmitCmd *cmd = data->m_frame->m_submitCmds[i];
        if (nullptr == cmd) {
            continue;
        }

        if (cmd->m_updateFlags & (ui32)FrameSubmitCmd::UpdateMatrixes) {
            MatrixBuffer *buffer = (MatrixBuffer*) cmd->m_data;

            m_renderCmdBuffer->setMatrixBuffer(cmd->m_batchId, buffer);
        } else if (cmd->m_updateFlags & (ui32)FrameSubmitCmd::UpdateUniforms) {
            setupParameter(cmd->m_var, m_oglBackend, this);
        }
    }
    data->m_frame->m_submitCmds.resize(0);
    m_oglBackend->useShader(nullptr);

    return true;
}

bool OGLRenderEventHandler::onShutdownRequest( const Common::EventData *eventData ) {
    OSRE_ASSERT( nullptr != eventData );

    m_isRunning = false;

    return true;
}

bool OGLRenderEventHandler::onResizeRenderTarget( const Common::EventData *eventData ) {
    OSRE_ASSERT( nullptr != eventData );

    ResizeEventData *data = ( ResizeEventData* ) eventData;
    const ui32 x( data->m_x );
    const ui32 y( data->m_y );
    const ui32 w( data->m_w );
    const ui32 h( data->m_h );
    m_oglBackend->setViewport( x, y, w, h );

    return true;
}

} // Namespace RenderBackend
} // Namespace OSRE
