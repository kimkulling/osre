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
#ifndef ZFXCE2_RS_RENDERBACKEND_OGLRENDERBACKEND_H_INC
#define ZFXCE2_RS_RENDERBACKEND_OGLRENDERBACKEND_H_INC

#include <cppcore/Container/TArray.h>
#include <zfxce2/RenderSystem/RenderBackend/RenderBackendService.h>
#include <zfxce2/RenderSystem/RenderBackend/RenderCommon.h>

#include "OGLCommon.h"

#include <map>

namespace ZFXCE2 {

namespace Platform {
    class AbstractRenderContext;
}

namespace RenderBackend {

class OGLShader;

struct OGLBuffer;
struct OGLVertexArray;
struct OGLVertexAttribute;
struct OGLTexture;
struct RenderCommand;
struct RenderCmdData3DView;
struct Shader;
struct Parameter;
struct PrimitiveGroup;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderBackend::OGLRenderBackend
///	@ingroup	RenderSystem
///
///	@brief
//-------------------------------------------------------------------------------------------------
class OGLRenderBackend {
public:
    OGLRenderBackend();
    ~OGLRenderBackend();
    void setRenderContext( Platform::AbstractRenderContext *pRenderCtx );
    void clearRenderTarget( ui32 targets );
    void setViewport( i32 x, i32 y, i32 w, i32 h );
    OGLBuffer *createBuffer( BufferType type );
    void bindBuffer( ui32 handle );
    void bindBuffer( OGLBuffer *pBuffer );
    void unbindBuffer( OGLBuffer *pBuffer );
    void bufferData( OGLBuffer *pBuffer, void *pData, ui32 size, BufferAccessType usage );
    void releaseBuffer( OGLBuffer *pBuffer );
    void releaseAllBuffers();
    bool createVertexAttribArray( VertexType type, OGLShader *pShader, CPPCore::TArray<OGLVertexAttribute*> &attributes );
    void releasevertexAttribArray( CPPCore::TArray<OGLVertexAttribute*> &attributes );
    OGLVertexArray *createVertexArray();
    bool bindVertexAttribute( OGLVertexArray *pVertexArray, OGLShader *pShader, ui32 stride, GLint loc, OGLVertexAttribute* attrib );
    bool bindVertexAttributes( OGLVertexArray *pVertexArray, OGLShader *pShader, ui32 stride, const CPPCore::TArray<OGLVertexAttribute*> &attributes );
    void destroyVertexArray( OGLVertexArray *pVertexArray );
    void bindVertexArray( OGLVertexArray *pVertexArray );
    void unbindVertexArray( OGLVertexArray *pVertexArray );
    void releaseAllVertexArrays();
    OGLShader *createShader( const ce_string &name, Shader *pShader );
    OGLShader *getShader( const ce_string &name );
    bool useShader( OGLShader *pShader );
    OGLShader *getActiveShader() const;
    bool releaseShader( OGLShader *pShader );
    void releaseAllShaders();
    OGLTexture *createEmptyTexture( const ce_string &name, TextureTargetType target, ui32 width, ui32 height, ui32 channels );
    void updateTexture( OGLTexture *pOGLTextue, ui32 offsetX, ui32 offsetY, c8 *data, ui32 size );
    OGLTexture *createTextureFromFile( const ce_string &name, const ce_string &filename );
    OGLTexture *findTexture( const ce_string &name ) const;
    bool bindTexture( OGLTexture *pOGLTextue, TextureStageType stageType );
    void releaseTexture( OGLTexture *pTexture );
    void releaseAllTextures();
    OGLParameter *createParameter( const ce_string &name, ParameterType type, ParamDataBlob *blob, ui32 numItems );
    OGLParameter *getParameter( const ce_string &name ) const;
    void setParameter( OGLParameter *param );
    void setParameter( OGLParameter **param, ui32 numParam );
    void releaseAllParameters();
    ui32 addPrimitiveGroup( PrimitiveGroup *grp );
    void releaseAllPrimitiveGroups();
    void render( ui32 grimpGrpIdx );
    void render( ui32 primpGrpIdx, ui32 numInstances );
    void renderFrame();
    static ui32 getVertexSize( VertexType vertextype );

private:
    Platform::AbstractRenderContext *m_pRenderCtx;
    CPPCore::TArray<OGLBuffer*>      m_buffers;
    CPPCore::TArray<OGLVertexArray*> m_vertexarrays;
    CPPCore::TArray<OGLShader*>      m_shaders;
    CPPCore::TArray<OGLTexture*>     m_textures;
    CPPCore::TArray<ui32>            m_freeTexSlots;
    std::map<ce_string, ui32>        m_texLookupMap;
    CPPCore::TArray<OGLParameter*>   m_parameters;
    OGLShader                       *m_pShaderInUse;
    CPPCore::TArray<ui32>            m_freeBufferSlots;
    CPPCore::TArray<OGLPrimGroup*>   m_primitives;
};

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace ZFXCE2

#endif // ZFXCE2_RS_RENDERBACKEND_OGLRENDERBACKEND_H_INC
