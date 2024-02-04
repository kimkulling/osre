/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "OGLCommon.h"
#include "OGLRenderBackend.h"
#include "OGLRenderCommands.h"
#include "OGLShader.h"
#include "RenderCmdBuffer.h"

#include <osre/App/AssetRegistry.h>
#include <osre/Common/Logger.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/Uri.h>
#include <osre/Platform/AbstractOGLRenderContext.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/AbstractTimer.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Profiling/PerformanceCounterRegistry.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Shader.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::cppcore;

static constexpr c8 Tag[] = "OGLRendeEventHandler";

OGLRenderEventHandler::OGLRenderEventHandler() :
        AbstractEventHandler(),
        m_isRunning(true),
        m_oglBackend(nullptr),
        m_renderCmdBuffer(nullptr),
        m_renderCtx(nullptr),
        m_vertexArray(nullptr),
        mPipeline(nullptr) {
    // empty
}

bool OGLRenderEventHandler::onEvent(const Event &ev, const EventData *data) {
    if (!m_isRunning) {
        return true;
    }

    bool result = false;
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
    } else if (OnShutdownRequestEvent == ev) {
        result = onShutdownRequest(data);
    } else if (OnResizeEvent == ev) {
        result = onResizeRenderTarget(data);
    } else if (OnScreenshotEvent == ev) {
        result = onScreenshot(data);
    }

    return result;
}

void OGLRenderEventHandler::setActiveShader(OGLShader *oglShader) {
    osre_assert(m_renderCmdBuffer != nullptr);

    m_renderCmdBuffer->setActiveShader(oglShader);
}

void OGLRenderEventHandler::enqueueRenderCmd(OGLRenderCmd *oglRenderCmd) {
    osre_assert(m_renderCmdBuffer != nullptr);

    m_renderCmdBuffer->enqueueRenderCmd(oglRenderCmd);
}

void OGLRenderEventHandler::setParameter(const TArray<OGLParameter *> &paramArray) {
    osre_assert(m_renderCmdBuffer != nullptr);

    m_renderCmdBuffer->setParameter(paramArray);
}

bool OGLRenderEventHandler::onAttached(const EventData *) {
    if (nullptr != m_oglBackend) {
        return false;
    }

    m_oglBackend = new OGLRenderBackend;
    m_oglBackend->setTimer(PlatformInterface::getInstance()->getTimer());

    return true;
}

bool OGLRenderEventHandler::onDetached(const EventData *) {
    if (m_renderCmdBuffer != nullptr) {
        osre_error(Tag, "Render-Commandbuffer was not destroyed before detaching.");
        delete m_renderCmdBuffer;
        m_renderCmdBuffer = nullptr;
    }

    delete m_oglBackend;
    m_oglBackend = nullptr;

    return true;
}

bool OGLRenderEventHandler::onCreateRenderer(const EventData *eventData) {
    osre_assert(nullptr != m_oglBackend);

    CreateRendererEventData *createRendererEvData = (CreateRendererEventData *)eventData;
    AbstractWindow *activeSurface = createRendererEvData->m_activeSurface;
    if (nullptr == activeSurface) {
        osre_debug(Tag, "No active surface, pointer is nullptr.");
        return false;
    }

    if (!activeSurface->isCeated()) {
        if (!activeSurface->create()) {
            osre_debug(Tag, "Cannot create active surface.");
            return false;
        }
    }

    bool result = false;
    m_renderCtx = PlatformInterface::getInstance()->getRenderContext();

    if (nullptr == m_renderCtx) {
        osre_debug(Tag, "Rendercontext is nullptr.");
        return result;
    }
        
    if (!m_renderCtx->create(activeSurface)) {
        osre_debug(Tag, "Cannot create render-context.");
        return false;
    }

    m_oglBackend->create(m_renderCtx);
    if (!m_renderCtx->isActive()) {
        osre_debug(Tag, "Error while activating render-context.");
        return false;
    }

    Rect2ui rect;
    activeSurface->getWindowsRect(rect);
    m_oglBackend->setViewport(rect.x1, rect.y1, rect.width, rect.height);

    const String defaultFont = PlatformInterface::getInstance()->getDefaultFontName();
    IO::Uri fontUri("file://assets/Textures/Fonts/" + defaultFont);
    String root = App::AssetRegistry::getPath("media");
    String path = App::AssetRegistry::resolvePathFromUri(fontUri);
    fontUri.setPath(path);
    m_renderCmdBuffer = new RenderCmdBuffer(m_oglBackend, m_renderCtx);

    bool ok = Profiling::PerformanceCounterRegistry::create();
    if (!ok) {
        osre_error(Tag, "Error while destroying performance counters.");
        return false;
    }

    mPipeline = createRendererEvData->m_pipeline;
    Profiling::PerformanceCounterRegistry::registerCounter("fps");

    return true;
}

bool OGLRenderEventHandler::onDestroyRenderer(const Common::EventData *) {
    osre_assert(nullptr != m_oglBackend);

    if (nullptr == m_renderCtx) {
        return false;
    }

    if (!Profiling::PerformanceCounterRegistry::destroy()) {
        osre_error(Tag, "Error while destroying performance counters.");
    }

    onClearGeo(nullptr);
    m_renderCtx->destroy();
    delete m_renderCtx;
    m_renderCtx = nullptr;

    delete m_renderCmdBuffer;
    m_renderCmdBuffer = nullptr;

    return true;
}

bool OGLRenderEventHandler::onAttachView(const EventData *) {
    osre_assert(nullptr != m_oglBackend);

    return true;
}

bool OGLRenderEventHandler::onDetachView(const EventData *) {
    osre_assert(nullptr != m_oglBackend);
    onClearGeo(nullptr);

    return true;
}

bool OGLRenderEventHandler::onClearGeo(const EventData *) {
    osre_assert(nullptr != m_oglBackend);
    osre_assert(nullptr != m_renderCmdBuffer);

    m_oglBackend->releaseAllBuffers();
    m_oglBackend->releaseAllShaders();
    m_oglBackend->releaseAllTextures();
    m_oglBackend->releaseAllParameters();
    m_oglBackend->releaseAllPrimitiveGroups();
    m_oglBackend->releaseAllVertexArrays();
    m_renderCmdBuffer->clear();

    return true;
}

bool OGLRenderEventHandler::onRenderFrame(const EventData*) {
    osre_assert(nullptr != m_oglBackend);
    osre_assert(nullptr != m_renderCmdBuffer);
    osre_assert(m_renderCtx != nullptr);

    m_renderCmdBuffer->onPreRenderFrame(mPipeline);
    m_renderCmdBuffer->onRenderFrame();
    m_renderCmdBuffer->onPostRenderFrame();

    return true;
}

bool OGLRenderEventHandler::addMeshes(const c8 *id, cppcore::TArray<size_t> &primGroups, MeshEntry *currentMeshEntry) {
    for (ui32 meshIdx = 0; meshIdx < currentMeshEntry->mMeshArray.size(); ++meshIdx) {
        Mesh *currentMesh = currentMeshEntry->mMeshArray[meshIdx];
        if (nullptr == currentMesh) {
            osre_assert(nullptr != currentMesh);
            continue;
        }

        // register primitive groups to render
        for (size_t i = 0; i < currentMesh->getNumberOfPrimitiveGroups(); ++i) {
            const size_t primIdx(m_oglBackend->addPrimitiveGroup(currentMesh->getPrimitiveGroupAt(i)));
            primGroups.add(primIdx);
        }

        // create the default material
        SetMaterialStageCmdData *data = setupMaterial(currentMesh->getMaterial(), m_oglBackend, this);

        // setup vertex array, vertex and index buffers
        m_vertexArray = setupBuffers(currentMesh, m_oglBackend, m_renderCmdBuffer->getActiveShader());
        if (nullptr == m_vertexArray) {
            osre_debug(Tag, "Vertex-Array-pointer is a nullptr.");
            return false;
        }
        data->m_vertexArray = m_vertexArray;

        // setup the render calls
        if (0 == currentMeshEntry->numInstances) {
            setupPrimDrawCmd(id, currentMesh->isLocal(), currentMesh->getLocalMatrix(),
                    primGroups, m_oglBackend, this, m_vertexArray);
        } else {
            setupInstancedDrawCmd(id, primGroups, m_oglBackend, this, m_vertexArray,
                    currentMeshEntry->numInstances);
        }

        primGroups.resize(0);
    }

    return true;
}
 
bool OGLRenderEventHandler::onInitRenderPasses(const Common::EventData *eventData) {
    osre_assert(nullptr != m_oglBackend);

    InitPassesEventData *frameToCommitData = (InitPassesEventData*) eventData;
    if (nullptr == frameToCommitData) {
        return false;
    }

    cppcore::TArray<size_t> primGroups;
    Frame *frame = frameToCommitData->m_frame;
    for (PassData *currentPass : frame->m_newPasses) {
        if (nullptr == currentPass) {
            osre_assert(nullptr != currentPass);
            continue;
        }

        if (!currentPass->m_isDirty) {
            continue;
        }

        // ToDo: create pipeline pass for the name.
        for (RenderBatchData * currentBatchData : currentPass->m_geoBatches) {
            if (nullptr == currentBatchData) {
                continue;
            }

            // set the matrix
            MatrixBuffer &matrixBuffer = currentBatchData->m_matrixBuffer;
            getRenderCmdBuffer()->setMatrixes(matrixBuffer.m_model, matrixBuffer.m_view, matrixBuffer.m_proj);

            // set uniforms
            for (auto & uniform : currentBatchData->m_uniforms) {
                setupParameter(uniform, m_oglBackend, this);
            }

            // set meshes
            for (ui32 meshEntryIdx = 0; meshEntryIdx < currentBatchData->m_meshArray.size(); ++meshEntryIdx) {
                MeshEntry *currentMeshEntry = currentBatchData->m_meshArray[meshEntryIdx];
                if (nullptr == currentMeshEntry) {
                    osre_assert(nullptr != currentMeshEntry);
                    continue;
                }

                if (!currentMeshEntry->m_isDirty) {
                    continue;
                }

                for (ui32 meshIdx = 0; meshIdx < currentMeshEntry->mMeshArray.size(); ++meshIdx) {
                    Mesh *currentMesh = currentMeshEntry->mMeshArray[meshIdx];
                    if (nullptr == currentMesh) {
                        osre_assert(nullptr != currentMesh);
                        continue;
                    }


                    // register primitive groups to render
                    for (size_t i = 0; i < currentMesh->getNumberOfPrimitiveGroups(); ++i) {
                        const size_t primIdx(m_oglBackend->addPrimitiveGroup(currentMesh->getPrimitiveGroupAt(i)));
                        primGroups.add(primIdx);
                    }

                    // create the default material
                    SetMaterialStageCmdData *data = setupMaterial(currentMesh->getMaterial(), m_oglBackend, this);

                    // setup vertex array, vertex and index buffers
                    m_vertexArray = setupBuffers(currentMesh, m_oglBackend, m_renderCmdBuffer->getActiveShader());
                    if (nullptr == m_vertexArray) {
                        osre_debug(Tag, "Vertex-Array-pointer is a nullptr.");
                        return false;
                    }
                    data->m_vertexArray = m_vertexArray;

                    // setup the render calls
                    if (0 == currentMeshEntry->numInstances) {
                        setupPrimDrawCmd(currentBatchData->m_id, currentMesh->isLocal(), currentMesh->getLocalMatrix(),
                                primGroups, m_oglBackend, this, m_vertexArray);
                    } else {
                        setupInstancedDrawCmd(currentBatchData->m_id, primGroups, m_oglBackend, this, m_vertexArray,
                                currentMeshEntry->numInstances);
                    }

                    primGroups.resize(0);
                }
                //addMeshes(currentBatchData, primGroups, currentMeshEntry);
                currentMeshEntry->m_isDirty = false;
            }
        }
    }

    frame->m_newPasses.clear();

    m_oglBackend->useShader(nullptr);

    return true;
}


static void setName(c8 *name, size_t bufferSize, FrameSubmitCmd *cmd) {
    ::memset(name, '\0', bufferSize);
    size_t bufferLen = cmd->m_data[0];
    if (bufferLen > bufferSize-1) {
        bufferLen = bufferSize - 1;
    }
    ::strncpy(name, &cmd->m_data[1], bufferLen);
}

static constexpr size_t BufferSize = 256;

void OGLRenderEventHandler::onHandleCommit(FrameSubmitCmd *cmd) {
    if (cmd->m_updateFlags & (ui32)FrameSubmitCmd::UpdateMatrixes) {
        MatrixBuffer *buffer = (MatrixBuffer *)cmd->m_data;
        m_renderCmdBuffer->setMatrixBuffer(cmd->m_batchId, buffer);
    } else if (cmd->m_updateFlags & (ui32)FrameSubmitCmd::UpdateUniforms) {
        c8 name[BufferSize];
        setName(name, BufferSize, cmd);
        const ui32 offset = cmd->m_data[0] + 1;
        const size_t size = cmd->m_size - offset;
        OGLParameter *oglParam = m_oglBackend->getParameter(name);
        ::memcpy(oglParam->m_data->getData(), &cmd->m_data[offset], size);
    } else if (cmd->m_updateFlags & (ui32)FrameSubmitCmd::UpdateBuffer) {
        OGLBuffer *buffer = m_oglBackend->getBufferById(cmd->m_meshId);
        m_oglBackend->bindBuffer(buffer);
        m_oglBackend->copyDataToBuffer(buffer, cmd->m_data, cmd->m_size, BufferAccessType::ReadWrite);
        m_oglBackend->unbindBuffer(buffer);
    } else if (cmd->m_updateFlags & (ui32)FrameSubmitCmd::AddRenderData) {
        for (ui32 i = 0; i < cmd->m_updatedPasses.size(); ++i) {
            PassData *pd = cmd->m_updatedPasses[i];
            if (pd == nullptr) {
                continue;
            }

            for (RenderBatchData *rbd : pd->m_geoBatches) {
                for (MeshEntry *entry : rbd->m_meshArray) {
                    cppcore::TArray<size_t> primGroups;
                    addMeshes(cmd->m_batchId, primGroups, entry);
                }
            }
        }
    }
}

bool OGLRenderEventHandler::onCommitNexFrame(const EventData *eventData) {
    if (m_oglBackend == nullptr) {
        return false;
    }

    CommitFrameEventData *data = (CommitFrameEventData *)eventData;
    if (data == nullptr) {
        return false;
    }

    for (FrameSubmitCmd *cmd : data->m_frame->m_submitCmds) {
        if (cmd == nullptr) {
            continue;
        }

        onHandleCommit(cmd);
        /*if (cmd->m_updateFlags & (ui32)FrameSubmitCmd::UpdateMatrixes) {
            MatrixBuffer *buffer = (MatrixBuffer *)cmd->m_data;
            m_renderCmdBuffer->setMatrixBuffer(cmd->m_batchId, buffer);
        } else if (cmd->m_updateFlags & (ui32)FrameSubmitCmd::UpdateUniforms) {
            c8 name[BufferSize];
            setName(name, BufferSize, cmd);
            const ui32 offset = cmd->m_data[0] + 1;
            const size_t size = cmd->m_size - offset;
            OGLParameter *oglParam = m_oglBackend->getParameter(name);
            ::memcpy(oglParam->m_data->getData(), &cmd->m_data[offset], size);
        } else if (cmd->m_updateFlags & (ui32)FrameSubmitCmd::UpdateBuffer) {
            OGLBuffer *buffer = m_oglBackend->getBufferById(cmd->m_meshId);
            m_oglBackend->bindBuffer(buffer);
            m_oglBackend->copyDataToBuffer(buffer, cmd->m_data, cmd->m_size, BufferAccessType::ReadWrite);
            m_oglBackend->unbindBuffer(buffer);
        } else if (cmd->m_updateFlags & (ui32)FrameSubmitCmd::AddRenderData) {
            for (ui32 i = 0; i < cmd->m_updatedPasses.size(); ++i) {
                PassData *pd = cmd->m_updatedPasses[i];
                if (pd == nullptr) {
                    continue;
                }

                for (RenderBatchData *rbd : pd->m_geoBatches) {
                    for (MeshEntry *entry : rbd->m_meshArray) {
                        cppcore::TArray<size_t> primGroups;
                        addMeshes(cmd->m_batchId, primGroups, entry);
                    }
                }
            }
        }*/
        cmd->m_updateFlags = 0u;
    }
    
    data->m_frame->m_submitCmds.resize(0);
    data->m_frame->m_submitCmdAllocator.release();

    return true;
}

bool OGLRenderEventHandler::onShutdownRequest(const EventData*) {
    m_isRunning = false;

    return true;
}

bool OGLRenderEventHandler::onResizeRenderTarget(const EventData *eventData) {
    ResizeEventData *data = (ResizeEventData *)eventData;
    if (data != nullptr) {
        m_oglBackend->setViewport(data->m_x, data->m_y, data->m_w, data->m_h);
    }

    return true;
}

bool OGLRenderEventHandler::onScreenshot(const EventData *eventData) {
    bool result = false;
    ScreenshotEventData *data = (ScreenshotEventData*) eventData;
    if (data != nullptr){
        result = makeScreenShot(data->mFilename.c_str(), data->mWidth, data->mHeight);
    }
    return result;
}

} // Namespace RenderBackend
} // Namespace OSRE
