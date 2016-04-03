/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>
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


//-------------------------------------------------------------------------------------------------
void checkOGLErrorState( const c8 *file, ui32 line );

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

enum class OGLRenderCmdType {
    SetParameterCmd,
    SetRenderTargetCmd,
    SetTextureCmd,
    SetShaderCmd,
    DrawPrimitivesCmd,
    DrawPrimitivesInstancesCmd
};

struct OGLRenderCmd {
    OGLRenderCmdType m_type;
	ui32 m_id;
    ui32 m_vaId;
    void *m_pData;
};

struct OGLRenderCmdAllocator {
	static ui32 m_lastid;

	static OGLRenderCmd *alloc( OGLRenderCmdType type, ui32 vaId, void *data ) {
		OGLRenderCmd *cmd = new OGLRenderCmd;
		cmd->m_type  = type;
		cmd->m_id    = m_lastid;
        cmd->m_vaId  = vaId;
		cmd->m_pData = data;
		m_lastid++;

		return cmd;
	}

	static void free( OGLRenderCmd *cmd ) {
		delete cmd;
	}
};

///	@brief
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

///	@brief
struct OGLPrimGroup {
    GLenum m_primitive;
    ui32   m_startIndex;
    ui32   m_numPrimitives;
    GLenum m_indexType;
};

///	@brief
struct SetParameterCmdData {
    OGLParameter **m_param;
    ui32 m_numParam;
};

///	@brief
struct SetRenderTargetCmdData {
};

///	@brief
struct SetTextureStageCmdData {
    CPPCore::TArray<OGLTexture*> m_textures;
};

///	@brief
struct SetShaderStageCmdData {
    OGLShader *m_pShader;
};

///	@brief
struct DrawInstancePrimitivesCmdData {
    ui32                  m_numInstances;   ///<
    CPPCore::TArray<ui32> m_bufferHandles;  ///<
    CPPCore::TArray<ui32> m_primitives;     ///<
};

///	@brief
struct DrawPrimitivesCmdData {
	OGLVertexArray       *m_vertexArray;    ///<
    CPPCore::TArray<ui32> m_primitives;     ///<
};

//-------------------------------------------------------------------------------------------------

} // Namespace RendeBackend
} // Namespace OSRE
