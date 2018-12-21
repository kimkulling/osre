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
#include "DX11RenderVEventHandler.h"
#include "DX11Renderer.h"

#include <osre/Common/Logger.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/AbstractRenderContext.h>
#include <osre/Profiling/PerformanceCounterRegistry.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/Uri.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/RenderBackend/RenderBackendService.h>

#include <d3d11.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::CPPCore;

static const c8 *Tag = "DX11RenderEventHandler";

DX11RenderEventHandler::DX11RenderEventHandler()
: AbstractEventHandler()
, m_isRunning( true )
, m_dx11Renderer( nullptr )
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

bool DX11RenderEventHandler::onCreateRenderer(const Common::EventData *eventData) {
    if (nullptr != m_dx11Renderer) {
        return false;
    }

    m_dx11Renderer = new DX11Renderer();
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
    result = m_dx11Renderer->create(activeSurface);
    if (!result) {
        osre_error( Tag, "Error occurred while creating DX11Renderer.")
    }
    
    // Create Matrix buffer
    struct MatrixBuffer {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
    };
    BufferData *data = BufferData::alloc(BufferType::ConstantBuffer, sizeof(MatrixBuffer), BufferAccessType::ReadWrite);
    m_matrixBuffer = m_dx11Renderer->createBuffer(BufferType::ConstantBuffer, data, BufferAccessType::ReadWrite);
    
    return result;
}

bool DX11RenderEventHandler::onDestroyRenderer(const Common::EventData *) {
    if (nullptr == m_dx11Renderer) {
        return false;
    }

    m_dx11Renderer->releaseBuffer(m_matrixBuffer);
    m_matrixBuffer = nullptr;

    delete m_dx11Renderer;
    m_dx11Renderer = nullptr;

    return true;
}

bool DX11RenderEventHandler::onAttachView(const Common::EventData *) {
    return true;
}

bool DX11RenderEventHandler::onDetachView(const Common::EventData *) {
    return true;
}

bool DX11RenderEventHandler::onClearGeo(const Common::EventData *) {
    return true;
}

bool DX11RenderEventHandler::onRenderFrame(const Common::EventData *) {
    // triggers the render frame loop
    Color4 clear(0, 1, 0, 0);
    m_dx11Renderer->beginScene(clear);
    for (ui32 i = 0; i < m_renderCmds.size(); ++i) {
        m_dx11Renderer->render(m_renderCmds[i]);
    }

    m_dx11Renderer->endScene();

    return true;
}

bool DX11RenderEventHandler::onCommitNexFrame(const Common::EventData *eventData) {
    CommitFrameEventData *frameToCommitData = (CommitFrameEventData*)eventData;
    if (nullptr == frameToCommitData) {
        return false;
    }
    Frame *frame = frameToCommitData->m_frame;
    for (ui32 geoPackageIdx = 0; geoPackageIdx<frame->m_numGeoPackages; ++geoPackageIdx) {
        GeometryPackage *currentGeoPackage(frame->m_geoPackages[geoPackageIdx]);
        if (nullptr == currentGeoPackage) {
            continue;
        }

        for (ui32 geoIdx = 0; geoIdx < currentGeoPackage->m_numNewGeo; ++geoIdx) {
            Mesh *geo = currentGeoPackage->m_newGeo[geoIdx];

            ID3D11Buffer *vb = m_dx11Renderer->createBuffer(BufferType::VertexBuffer, geo->m_vb, geo->m_vb->getBufferAccessType() );
            ID3D11Buffer *ib = m_dx11Renderer->createBuffer(BufferType::IndexBuffer, geo->m_ib, geo->m_ib->getBufferAccessType() );

            RenderCmd *cmd = new RenderCmd;
            cmd->m_vb = vb;
            cmd->m_ib = ib;
            
            m_renderCmds.add(cmd);
        }
    }

    delete[] frame->m_geoUpdates;
    frame->m_geoUpdates = nullptr;
    frame->m_numGeoUpdates = 0;

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
