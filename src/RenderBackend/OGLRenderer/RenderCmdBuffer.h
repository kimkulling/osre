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
#ifndef ZFXCE2_RS_RENDERBACKEND_RENDERCMDBUFFER_H_INC
#define ZFXCE2_RS_RENDERBACKEND_RENDERCMDBUFFER_H_INC

#include <cppcore/Container/TArray.h>

namespace ZFXCE2 {
    
namespace Core {
    class EventData;
}

namespace Platform {
    class AbstractRenderContext;
}

namespace RenderBackend {

class OGLRenderBackend;
class OGLShader;

struct OGLVertexArray;
struct OGLRenderCmd;
struct DrawPrimitivesCmdData;
struct DrawInstancePrimitivesCmdData;
struct SetParameterCmdData;
struct SetTextureStageCmdData;
struct SetShaderStageCmdData;
struct SetRenderTargetCmdData;
struct OGLParameter;
struct PrimitiveGroup;
struct Material;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderBackend::RenderCmdBuffer
///	@ingroup	RenderSystem
///
///	@brief  This class is used to manage a render command buffer.
//-------------------------------------------------------------------------------------------------
class RenderCmdBuffer {
public:
    /// @brief  Describes the requested enqueue type.
    enum EnqueueType {
        RCE_Back    ///< Enqueue render command at the end 
    };

public:
    /// The class constructor.
    RenderCmdBuffer( OGLRenderBackend *pRenderBackend, Platform::AbstractRenderContext *ctx );
    /// The class destructor.
    virtual ~RenderCmdBuffer();
    /// Will set the active vertex array object.
    void setVertexArray( OGLVertexArray *pVertexArray );
    /// Will set the active shader.
    void setActiveShader( OGLShader *pOGLShader );
    ///
    OGLShader *getActiveShader() const;
    /// Will enqueue a new render command.
    void enqueueRenderCmd( OGLRenderCmd *pOGLRenderCmd, EnqueueType type = RCE_Back );
    /// The callback before rendering.
    bool onPreRenderFrame();
    /// The render callback.
    bool onRenderFrame( const Core::EventData *pEventData );
    /// The callback after rendering.
    bool onPostRenderFrame();
    /// The buffer and all attached commands will be cleared.
    void clear();

protected:
    virtual bool onUpdateParameter( const Core::EventData *pEventData );
    virtual bool onSetParametersCmd( SetParameterCmdData *pData );
    virtual bool onDrawPrimitivesCmd( DrawPrimitivesCmdData *pData );
    virtual bool onDrawPrimitivesInstancesCmd( DrawInstancePrimitivesCmdData *data );
    virtual bool onSetTextureStageCmd( SetTextureStageCmdData *pData );
    virtual bool onSetShaderStageCmd( SetShaderStageCmdData *pData );
    virtual bool onSetRenderTargetCmd( SetRenderTargetCmdData *pData );

private:
    OGLRenderBackend *m_pRenderBackend;
    Platform::AbstractRenderContext *m_pRenderCtx;
    CPPCore::TArray<OGLRenderCmd*> m_cmdbuffer;
    OGLShader *m_pActiveShader;
    OGLVertexArray *m_pVertexArray;
    CPPCore::TArray<PrimitiveGroup*> m_primitives;
    CPPCore::TArray<Material*> m_materials;
    OGLParameter *m_param;
};

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace ZFXCE2

#endif // ZFXCE2_RS_RENDERBACKEND_RENDERCMDBUFFER_H_INC
