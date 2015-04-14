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
#ifndef ZFXCE2_RS_RENDERBACKEND_OGLCOMMON_H_INC
#define ZFXCE2_RS_RENDERBACKEND_OGLCOMMON_H_INC

#include <GL/glew.h>

#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <zfxce2/Infrastructure/Core/Logger.h>
#include <zfxce2/RenderSystem/RenderBackend/RenderCommon.h>

namespace ZFXCE2 {
namespace RenderBackend {

class OGLShader;

//-------------------------------------------------------------------------------------------------
inline
static void checkOGLErrorState( const c8 *pFile, ui32 line ) {
    GLenum error = glGetError();
    if (GL_NO_ERROR == error) {
        return;
    }

    switch (error) {
        case GL_INVALID_ENUM:
            // An unacceptable value is specified for an enumerated argument. The offending command 
            // is ignored and has no other side effect than to set the error flag.
            ::ZFXCE2::Core::errorPrint( pFile, line, "GL_INVALID_ENUM error." );
            break;

        case GL_INVALID_VALUE:
            // A numeric argument is out of range. The offending command is ignored and has no other 
            // side effect than to set the error flag.
            ::ZFXCE2::Core::errorPrint( pFile, line, "GL_INVALID_VALUE error." );
            break;

        case GL_INVALID_OPERATION:
            //The specified operation is not allowed in the current state. The offending command is 
            //ignored and has no other side effect than to set the error flag.
            ::ZFXCE2::Core::errorPrint( pFile, line, "GL_INVALID_OPERATION error." );
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            // The command is trying to render to or read from the framebuffer while the currently bound 
            // framebuffer is not framebuffer complete (i.e. the return value from glCheckFramebufferStatus is
            // not GL_FRAMEBUFFER_COMPLETE). The offending command is ignored and has no other side effect than
            // to set the error flag.
            ::ZFXCE2::Core::errorPrint( pFile, line, "GL_INVALID_FRAMEBUFFER_OPERATION error." );
            break;

        case GL_OUT_OF_MEMORY:
            // There is not enough memory left to execute the command. The state of the GL is undefined, except 
            // for the state of the error flags, after this error is recorded.
            ::ZFXCE2::Core::errorPrint( pFile, line, "GL_OUT_OF_MEMORY error." );
            break;

        default:
            break;
    }
}

//-------------------------------------------------------------------------------------------------
/// @def        ::ZFXCE2::RenderBackend::CHECKOGLERRORSTATE
/// @ingroup    RenderSystem
///
/// @brief  This macro will check the OpenGL-error state and write a message into the log.
//-------------------------------------------------------------------------------------------------
#define CHECKOGLERRORSTATE() checkOGLErrorState( __FILE__, __LINE__ )

static const GLuint OGLNotSetId  = 999999;
static const GLint  NoneLocation = -1;

struct OGLBuffer {
    ui32       m_handle;
    BufferType m_type;
    GLuint     m_id;
    ui32       m_size;
};

struct OGLVertexAttribute {
    GLuint        m_index;
    const c8     *m_pAttributeName;
    ui32          m_size;
    GLenum        m_type;
    const GLvoid *m_ptr;
};

struct OGLVertexArray {
    GLuint m_id;
    ui32 m_slot;
};

struct OGLTexture {
    GLuint m_textureId;
    ce_string m_name;
    GLenum m_target;
    GLenum m_format;
    ui32 m_slot;
    ui32 m_width;
    ui32 m_height;
    ui32 m_channels;
};

enum OGLRenderCmdType {
    SetParameterCmd,
    SetRenderTargetCmd,
    SetTextureCmd,
    SetShaderCmd,
    DrawPrimitivesCmd,
    DrawPrimitivesInstancesCmd,
};

struct OGLRenderCmd {
    OGLRenderCmdType m_type;
    void *m_pData;
};

struct OGLParameter {
    ce_string      m_name;
    GLint          m_loc;
    ParameterType  m_type;
    ParamDataBlob *m_data;
    ui32           m_numItems;

    OGLParameter()
    : m_name( "" )
    , m_loc( NoneLocation )
    , m_type( PT_None )
    , m_numItems( 1 ) {
        // empty
    }
};

struct OGLPrimGroup {
    GLenum m_primitive;
    ui32   m_startIndex;
    ui32   m_numPrimitives;
    GLenum m_indexType;
};

struct SetParameterCmdData {
    OGLParameter **m_param;
    ui32 m_numParam;
};

struct SetRenderTargetCmdData {
};

struct SetTextureStageCmdData {
    CPPCore::TArray<OGLTexture*> m_textures;
};

struct SetShaderStageCmdData {
    OGLShader *m_pShader;
};

struct DrawInstancePrimitivesCmdData {
    ui32                  m_numInstances;   ///<
    CPPCore::TArray<ui32> m_bufferHandles;  ///<
    CPPCore::TArray<ui32> m_primitives;     ///<
};

struct DrawPrimitivesCmdData {
    CPPCore::TArray<ui32> m_bufferHandles;  ///<
    CPPCore::TArray<ui32> m_primitives;     ///<
};

//-------------------------------------------------------------------------------------------------

} // Namespace RendeBackend
} // Namespace ZFXCE2

#endif // ZFXCE2_RS_RENDERBACKEND_OGLCOMMON_H_INC
