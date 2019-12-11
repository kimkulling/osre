/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "DX11RenderVEventHandler.h"
#include "DX11Renderer.h"

#include <osre/Common/Logger.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/AbstractOGLRenderContext.h>
#include <osre/Profiling/PerformanceCounterRegistry.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/Uri.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/RenderBackend/RenderBackendService.h>

#pragma warning( push )
#   pragma warning( disable : 4005 )
#   include <d3d11.h>
#pragma warning( pop )

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::CPPCore;

static const c8 *Tag = "DX11RenderEventHandler";

static void setConstantBuffers(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &proj, DX11Renderer *dx11Renderer ) {
    OSRE_ASSERT(nullptr != dx11Renderer);

    dx11Renderer->setMatrix(MatrixType::Model, model);
    dx11Renderer->setMatrix(MatrixType::View, view);
    dx11Renderer->setMatrix(MatrixType::Projection, proj);
}

static void setupUniforms( void *bufferData, DX11Renderer *dx11Renderer, DX11RenderEventHandler *eh) {
    OSRE_ASSERT(nullptr != dx11Renderer);
}

static void setupConstantBuffer(void* bufferData, DX11Renderer* dx11Renderer, DX11RenderEventHandler *eh) {
    OSRE_ASSERT(nullptr != dx11Renderer);
}

DX11RenderEventHandler::DX11RenderEventHandler()
: AbstractEventHandler()
, m_isRunning( true )
, m_dx11Backend( nullptr )
, m_renderCmds()
, m_matrixBuffer( nullptr )
, m_currentLayout( nullptr ) {
    // empty
}

DX11RenderEventHandler::~DX11RenderEventHandler() {
    // empty
}

bool DX11RenderEventHandler::onEvent( const Event &ev, const EventData *data ) {
    bool result(false);
    if (!m_isRunning) {
        return true;
    }

    if (OnAttachEventHandlerEvent == ev) {
        result = onAttached(data);
    } else if (OnDetatachEventHandlerEvent == ev) {
        result = onDetached(data);
    } else if (OnCreateRendererEvent == ev) {
        result = onCreateRenderer(data);
    } else if (OnDestroyRendererEvent == ev) {
        result = onDestroyRenderer(data);
    } else if (OnAttachViewEvent == ev) {
        result = onAttachView(data);
    } else if (OnDetachViewEvent == ev) {
        result = onDetachView(data);
    } else if (OnRenderFrameEvent == ev) {
        result = onRenderFrame(data);
    } else if (OnInitPassesEvent == ev) {
        result = onInitRenderPasses(data);
    } else if (OnCommitFrameEvent == ev) {
        result = onCommitNexFrame(data);
    } else if (OnClearSceneEvent == ev) {
        result = onClearGeo(data);
    } else if (OnShutdownRequest == ev) {
        result = onShutdownRequest(data);
    } else if (OnResizeEvent == ev) {
        result = onResizeRenderTarget(data);
    }

    return result;
}

bool DX11RenderEventHandler::onAttached(const EventData *) {
    return true;
}

bool DX11RenderEventHandler::onDetached(const EventData *) {
    return true;
}

bool DX11RenderEventHandler::onCreateRenderer(const EventData *eventData) {
    if (nullptr != m_dx11Backend) {
        return false;
    }

    m_dx11Backend = new DX11Renderer();
    CreateRendererEventData *createRendererEvData = (CreateRendererEventData*)eventData;
    AbstractWindow *activeSurface = createRendererEvData->m_activeSurface;
    if (nullptr == activeSurface) {
        osre_debug(Tag, "No active surface, pointer is nullptr.");
        return false;
    }

    if (!activeSurface->create()) {
        osre_debug(Tag, "Cannot create active surface.");
        return false;
    }

    bool result( false );
    result = m_dx11Backend->create(activeSurface);
    if (!result) {
        osre_error(Tag, "Error occurred while creating DX11Renderer.");
        return false;
    }
    
    // Create Matrix buffer
    struct MatrixBuffer {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
    };
    BufferData *data = BufferData::alloc(BufferType::ConstantBuffer, sizeof(MatrixBuffer), BufferAccessType::ReadWrite);
    m_matrixBuffer = m_dx11Backend->createBuffer(BufferType::ConstantBuffer, data, BufferAccessType::ReadWrite);
    
    return result;
}

bool DX11RenderEventHandler::onDestroyRenderer(const EventData *) {
    if (nullptr == m_dx11Backend) {
        return false;
    }

    m_dx11Backend->releaseBuffer(m_matrixBuffer);
    m_matrixBuffer = nullptr;

    delete m_dx11Backend;
    m_dx11Backend = nullptr;

    return true;
}

bool DX11RenderEventHandler::onAttachView(const EventData *) {
    return true;
}

bool DX11RenderEventHandler::onDetachView(const EventData *) {
    return true;
}

bool DX11RenderEventHandler::onClearGeo(const EventData *) {
    return true;
}

bool DX11RenderEventHandler::onRenderFrame(const EventData *) {
    OSRE_ASSERT(nullptr != m_dx11Backend);

    // triggers the render frame loop
    Color4 clear(0, 1, 0, 0);
    m_dx11Backend->beginScene(clear);
    
    for (ui32 i = 0; i < m_renderCmds.size(); ++i) {
        m_dx11Backend->render(m_renderCmds[i]);
    }

    m_dx11Backend->endScene();

    return true;
}

bool DX11RenderEventHandler::onInitRenderPasses( const EventData *eventData ) {
    OSRE_ASSERT(nullptr != m_dx11Backend);

    InitPassesEventData *frameToCommitData = (InitPassesEventData*)eventData;
    if (nullptr == frameToCommitData) {
        return false;
    }

    CPPCore::TArray<ui32> primGroups;
    Frame *frame = frameToCommitData->m_frame;
    for (ui32 passIdx = 0; passIdx < frame->m_newPasses.size(); ++passIdx) {
        PassData *currentPass = frame->m_newPasses[passIdx];
        if (nullptr == currentPass) {
            continue;
        }

        if (!currentPass->m_isDirty) {
            continue;
        }

        // ToDo: create pipeline pass for the name.
        for (ui32 batchIdx = 0; batchIdx < currentPass->m_geoBatches.size(); ++batchIdx) {
            GeoBatchData *currentBatchData = currentPass->m_geoBatches[batchIdx];
            OSRE_ASSERT(nullptr != currentBatchData);

            // set the matrix
            MatrixBuffer &matrixBuffer = currentBatchData->m_matrixBuffer;
            setConstantBuffers(matrixBuffer.m_model, matrixBuffer.m_view, matrixBuffer.m_proj, m_dx11Backend );

            // set uniforms
            for (ui32 uniformIdx = 0; uniformIdx < currentBatchData->m_uniforms.size(); ++uniformIdx) {
                setupUniforms(nullptr, m_dx11Backend);
                setupConstantBuffer( currentBatchData->m_uniforms[uniformIdx], m_dx11Backend );
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
                        //const ui32 primIdx(m_oglBackend->addPrimitiveGroup(&currentMesh->m_primGroups[i]));
                      //  primGroups.add(primIdx);
                    }

                    // create the default material
                    //SetMaterialStageCmdData *data = setupMaterial(currentMesh->m_material, m_oglBackend, this);

                    // setup vertex array, vertex and index buffers
                    //m_vertexArray = setupBuffers(currentMesh, m_oglBackend, m_renderCmdBuffer->getActiveShader());
                    //if (nullptr == m_vertexArray) {
                        osre_debug(Tag, "Vertex-Array-pointer is a nullptr.");
                        return false;
                    //}
                    //data->m_vertexArray = m_vertexArray;

                    // setup the draw calls
                    if (0 == currentMeshEntry->numInstances) {
                     /*   setupPrimDrawCmd(currentBatchData->m_id, currentMesh->m_localMatrix, currentMesh->m_model,
                            primGroups, m_oglBackend, this, m_vertexArray);*/
                    }
                    else {
                        /*setupInstancedDrawCmd(currentBatchData->m_id, primGroups, m_oglBackend, this, m_vertexArray,
                            currentMeshEntry->numInstances);*/
                    }

                    primGroups.resize(0);
                }
                currentMeshEntry->m_isDirty = false;
            }
        }
    }

    frame->m_newPasses.clear();

    //m_oglBackend->useShader(nullptr);

    return true;
}

bool DX11RenderEventHandler::onCommitNexFrame(const EventData *eventData) {
    OSRE_ASSERT(nullptr != m_dx11Backend);

    CommitFrameEventData *frameToCommitData = (CommitFrameEventData*)eventData;
    if (nullptr == frameToCommitData) {
        return false;
    }

    Frame *frame = frameToCommitData->m_frame;
    if (nullptr == frame) {
        return true;
    }

    return true;
}

bool DX11RenderEventHandler::onShutdownRequest(const EventData *) {
    return true;
}

bool DX11RenderEventHandler::onResizeRenderTarget(const EventData *) {
    return true;
}

} // Namespace RenderBackend
} // Namespace OSRE
