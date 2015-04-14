#pragma once

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
#ifndef ZFXCE2_RS_RENDERBACKEND_RENDEREVENTHANDLER_H_INC
#define ZFXCE2_RS_RENDERBACKEND_RENDEREVENTHANDLER_H_INC

#include <zfxce2/Infrastructure/Core/AbstractEventHandler.h>
#include <zfxce2/Infrastructure/Core/Event.h>
#include <zfxce2/RenderSystem/RenderBackend/RenderBackendService.h>

#include <GL/glew.h>
#include <GL/gl.h>

namespace ZFXCE2 {

namespace Interface {
    class IWindow;
}

namespace Platform {
    class AbstractRenderContext;
}

namespace RenderBackend {

class OGLRenderContext;
class OGLRenderBackend;
class OGLShader;
class RenderCmdBuffer;

struct Vertex;
struct OGLVertexArray;
struct PrimitiveGroup;
struct Material;
struct OGLRenderCmd;
struct DrawPrimitivesCmdData;
struct SetParameterCmdData;
struct SetTextureStageCmdData;
struct SetShaderStageCmdData;
struct SetRenderTargetCmdData;
struct OGLParameter;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderBackend::OGLRenderEventHandler
///	@ingroup	RenderSystem
///
///	@brief  This class is used to handle all incoming events for the render back-end.
//-------------------------------------------------------------------------------------------------
class OGLRenderEventHandler : public Core::AbstractEventHandler {
public:
    /// The class constructor.
    OGLRenderEventHandler();
    /// The class destructor, virtual.
    virtual ~OGLRenderEventHandler();
    /// The OnEvent-callback.
    virtual bool onEvent(const Core::Event &ev, const Core::EventData *pEventData);
    /// Will set the active shader.
    void setActiveShader( OGLShader *pOGLShader );
    /// Will enqueue a new render command.
    void enqueueRenderCmd( OGLRenderCmd *pOGLRenderCmd );

protected:
    virtual bool onAttached( const Core::EventData *pEventData );
    virtual bool onDetached( const Core::EventData *pEventData );
    virtual bool onCreateRenderer( const Core::EventData *pEventData );
    virtual bool onDestroyRenderer( const Core::EventData *pEventData );
    virtual bool onAttachView( const Core::EventData *pEventData );
    virtual bool onAttachGeo( const Core::EventData *pEventData );
    virtual bool onClearGeo( const Core::EventData *pEventData );
    virtual bool onRenderFrame( const Core::EventData *pEventData );
    virtual bool onUpdateParameter( const Core::EventData *pEventData );
    
private:
    OGLRenderBackend *m_oglBackend;
    RenderCmdBuffer *m_renderCmdBuffer;
    Platform::AbstractRenderContext *m_pRenderCtx;
    OGLVertexArray *m_pVertexArray;
};

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace ZFXCE2

#endif // ZFXCE2_RS_RENDERBACKEND_RENDEREVENTHANDLER_H_INC
