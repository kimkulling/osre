/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "App/AssetRegistry.h"
#include "Common/Logger.h"
#include "Debugging/osre_debugging.h"
#include "IO/Uri.h"
#include "Platform/AbstractOGLRenderContext.h"
#include "Platform/AbstractWindow.h"
#include "Platform/AbstractTimer.h"
#include "Platform/PlatformInterface.h"
#include "Profiling/PerformanceCounterRegistry.h"
#include "RenderBackend/Mesh.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/Shader.h"

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
        mActivePipeline(nullptr) {
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
    AbstractWindow *activeSurface = createRendererEvData->ActiveSurface;
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
    m_oglBackend->setViewport(0, 0, rect.width, rect.height);

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

    mActivePipeline = createRendererEvData->RequestedPipeline;
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

    m_renderCmdBuffer->onPreRenderFrame(mActivePipeline);
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
    Frame *frame = frameToCommitData->NextFrame;
    for (PassData *currentPass : frame->newPasses) {
        if (nullptr == currentPass) {
            osre_assert(nullptr != currentPass);
            continue;
        }

        if (!currentPass->isDirty) {
            continue;
        }

        // ToDo: create pipeline pass for the name.
        for (RenderBatchData * currentBatchData : currentPass->meshBatches) {
            if (nullptr == currentBatchData) {
                continue;
            }

            // set the matrix
            MatrixBuffer &matrixBuffer = currentBatchData->matrixBuffer;
            getRenderCmdBuffer()->setMatrixes(matrixBuffer.m_model, matrixBuffer.m_view, matrixBuffer.m_proj);

            // set uniforms
            for (auto & uniform : currentBatchData->uniforms) {
                setupParameter(uniform, m_oglBackend, this);
            }

            // set meshes
            for (ui32 meshEntryIdx = 0; meshEntryIdx < currentBatchData->meshArray.size(); ++meshEntryIdx) {
                MeshEntry *currentMeshEntry = currentBatchData->meshArray[meshEntryIdx];
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
                    data->vertexArray = m_vertexArray;

                    // setup the render calls
                    if (0 == currentMeshEntry->numInstances) {
                        setupPrimDrawCmd(currentBatchData->id, currentMesh->isLocal(), currentMesh->getLocalMatrix(),
                                primGroups, m_oglBackend, this, m_vertexArray);
                    } else {
                        setupInstancedDrawCmd(currentBatchData->id, primGroups, m_oglBackend, this, m_vertexArray,
                                currentMeshEntry->numInstances);
                    }

                    primGroups.resize(0);
                }
                currentMeshEntry->mMeshArray.resize(0);
                currentMeshEntry->m_isDirty = false;
            }
        }
    }

    frame->newPasses.clear();

    m_oglBackend->useShader(nullptr);

    return true;
}

static void setName(c8 *name, size_t bufferSize, FrameSubmitCmd *cmd) {
    ::memset(name, '\0', bufferSize);
    size_t bufferLen = cmd->data[0];
    if (bufferLen > bufferSize-1) {
        bufferLen = bufferSize - 1;
    }
    ::strncpy(name, &cmd->data[1], bufferLen);
}

static constexpr size_t BufferSize = 256;

void OGLRenderEventHandler::onHandleCommit(FrameSubmitCmd *cmd) {
    if (cmd->updateFlags & (ui32)FrameSubmitCmd::UpdateMatrixes) {
        MatrixBuffer *buffer = (MatrixBuffer *)cmd->data;
        osre_assert(cmd->batchId != nullptr);
        m_renderCmdBuffer->setMatrixBuffer(cmd->batchId, buffer);
    } else if (cmd->updateFlags & (ui32)FrameSubmitCmd::UpdateUniforms) {
        c8 name[BufferSize];
        setName(name, BufferSize, cmd);
        const ui32 offset = cmd->data[0] + 1;
        const size_t size = cmd->size - offset;
        OGLParameter *oglParam = m_oglBackend->getParameter(name);
        ::memcpy(oglParam->data->getData(), &cmd->data[offset], size);
    } else if (cmd->updateFlags & (ui32)FrameSubmitCmd::UpdateBuffer) {
        OGLBuffer *buffer = m_oglBackend->getBufferById(cmd->meshId);
        m_oglBackend->bindBuffer(buffer);
        m_oglBackend->copyDataToBuffer(buffer, cmd->data, cmd->size, BufferAccessType::ReadWrite);
        m_oglBackend->unbindBuffer(buffer);
    } else if (cmd->updateFlags & (ui32)FrameSubmitCmd::AddRenderData) {
        for (ui32 i = 0; i < cmd->updatedPasses.size(); ++i) {
            PassData *pd = cmd->updatedPasses[i];
            if (pd == nullptr) {
                continue;
            }

            for (RenderBatchData *rbd : pd->meshBatches) {
                for (MeshEntry *entry : rbd->meshArray) {
                    cppcore::TArray<size_t> primGroups;
                    addMeshes(cmd->batchId, primGroups, entry);
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

    for (FrameSubmitCmd *cmd : data->NextFrame->submitCmds) {
        if (cmd == nullptr) {
            continue;
        }

        onHandleCommit(cmd);
        cmd->updateFlags = 0u;
    }

    data->NextFrame->submitCmds.resize(0);
    data->NextFrame->submitCmdAllocator.release();

    return true;
}

bool OGLRenderEventHandler::onShutdownRequest(const EventData*) {
    m_isRunning = false;

    return true;
}

bool OGLRenderEventHandler::onResizeRenderTarget(const EventData *eventData) {
    ResizeEventData *data = (ResizeEventData*) eventData;
    if (data != nullptr) {
        m_oglBackend->setViewport(data->X, data->Y, data->W, data->H);
    }

    return true;
}

bool OGLRenderEventHandler::onScreenshot(const EventData *eventData) {
    bool result = false;
    ScreenshotEventData *data = (ScreenshotEventData*) eventData;
    if (data != nullptr){
        result = makeScreenShot(data->Filename.c_str(), data->Width, data->Height);
    }

    return result;
}

} // Namespace RenderBackend
} // Namespace OSRE
