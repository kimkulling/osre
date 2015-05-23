/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <osre/Common/Logger.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace RenderBackend {

class OGLShader;

static const String Tag = "OGLRenderBackend";

//-------------------------------------------------------------------------------------------------
inline
static void checkOGLErrorState( const c8 *file, ui32 line ) {
    GLenum error = glGetError();
    if (GL_NO_ERROR == error) {
        return;
    }

    switch (error) {
        case GL_INVALID_ENUM:
            // An unacceptable value is specified for an enumerated argument. The offending command 
            // is ignored and has no other side effect than to set the error flag.
            ::OSRE::Common::errorPrint( Tag, file, line, "GL_INVALID_ENUM error." );
            break;

        case GL_INVALID_VALUE:
            // A numeric argument is out of range. The offending command is ignored and has no other 
            // side effect than to set the error flag.
            ::OSRE::Common::errorPrint( Tag, file, line, "GL_INVALID_VALUE error." );
            break;

        case GL_INVALID_OPERATION:
            //The specified operation is not allowed in the current state. The offending command is 
            //ignored and has no other side effect than to set the error flag.
            ::OSRE::Common::errorPrint( Tag, file, line, "GL_INVALID_OPERATION error." );
            break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            // The command is trying to render to or read from the framebuffer while the currently bound 
            // framebuffer is not framebuffer complete (i.e. the return value from glCheckFramebufferStatus is
            // not GL_FRAMEBUFFER_COMPLETE). The offending command is ignored and has no other side effect than
            // to set the error flag.
            ::OSRE::Common::errorPrint( Tag, file, line, "GL_INVALID_FRAMEBUFFER_OPERATION error." );
            break;

        case GL_OUT_OF_MEMORY:
            // There is not enough memory left to execute the command. The state of the GL is undefined, except 
            // for the state of the error flags, after this error is recorded.
            ::OSRE::Common::errorPrint( Tag, file, line, "GL_OUT_OF_MEMORY error." );
            break;

        default:
            break;
    }
}

//-------------------------------------------------------------------------------------------------
/// @def        ::OSRE::RenderBackend::CHECKOGLERRORSTATE
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
    String m_name;
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
    String      m_name;
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
} // Namespace OSRE
