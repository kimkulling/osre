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
#pragma once

#include <osre/Common/osre_common.h>
#include <GL/glew.h>
#include <GL/gl.h>

#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderStates.h>

namespace OSRE {
namespace RenderBackend {

//  Forward declarations
class OGLShader;

void checkOGLErrorState( const c8 *file, ui32 line );

//-------------------------------------------------------------------------------------------------
/// @ingroup    RenderSystem
///
/// @brief  This macro will check the OpenGL-error state and write a message into the log.
//-------------------------------------------------------------------------------------------------
#ifdef _DEBUG
#   define CHECKOGLERRORSTATE() checkOGLErrorState( __FILE__, __LINE__ )
#else
#   define CHECKOGLERRORSTATE()
#endif // _DEBUG

static const GLuint OGLNotSetId  = 999999;
static const GLint  NoneLocation = -1;

///	@brief
struct OGLBuffer {
    ui32       m_handle;
    BufferType m_type;
    GLuint     m_oglId;
    ui32       m_geoId;
    ui32       m_size;
};

///	@brief
struct OGLVertexAttribute {
    GLuint        m_index;
    const c8     *m_pAttributeName;
    ui32          m_size;
    GLenum        m_type;
    const GLvoid *m_ptr;
};

///	@brief
struct OGLVertexArray {
    GLuint m_id;
    ui32   m_slot;

    OGLVertexArray()
    : m_id( 0 )
    , m_slot( 99999999 ) {
        // empty
    }
    
    ~OGLVertexArray() {
        // empty
    }
};

///	@brief
struct OGLTexture {
    GLuint m_textureId;
    String m_name;
    GLenum m_target;
    GLenum m_format;
    ui32   m_slot;
    ui32   m_width;
    ui32   m_height;
    ui32   m_channels;
};

///	@brief
enum class OGLRenderCmdType {
    SetParameterCmd,
    SetRenderTargetCmd,
    SetMaterialCmd,
    DrawPrimitivesCmd,
    DrawPrimitivesInstancesCmd,
    None
};

struct OGLRenderCmdAllocator;

///	@brief
struct OGLRenderCmd {
    friend struct OGLRenderCmdAllocator;

    OGLRenderCmdType m_type;
	ui32             m_id;
    void            *m_data;

private:
    OGLRenderCmd();
};

inline 
OGLRenderCmd::OGLRenderCmd() 
: m_type( OGLRenderCmdType::None )
, m_id( 999999 )
, m_data( nullptr ) {
    // empty
}

///	@brief
struct OGLRenderCmdAllocator {
	static ui32 m_lastid;

	static OGLRenderCmd *alloc( OGLRenderCmdType type, void *data ) {
		OGLRenderCmd *cmd = new OGLRenderCmd;
		cmd->m_type  = type;
		cmd->m_id    = m_lastid;
		cmd->m_data = data;
		++m_lastid;

		return cmd;
	}

	static void free( OGLRenderCmd *cmd ) {
		delete cmd;
	}

    OGLRenderCmdAllocator() = delete;
    ~OGLRenderCmdAllocator() = delete;
};

struct UniformDataBlob;

///	@brief
struct OGLParameter {
    String           m_name;
    GLint            m_loc;
    ParameterType    m_type;
    UniformDataBlob *m_data;
    ui32             m_numItems;

    OGLParameter()
    : m_name( "" )
    , m_loc( NoneLocation )
    , m_type( ParameterType::PT_None )
    , m_numItems( 1 ) {
        // empty
    }
};

///	@brief
struct OGLPrimGroup {
    GLenum m_primitive;
    ui32   m_startIndex;
    ui32   m_numIndices;
    GLenum m_indexType;
};

///	@brief
struct SetMaterialStageCmdData {
    OGLShader                   *m_shader;
    CPPCore::TArray<OGLTexture*> m_textures;
    OGLVertexArray              *m_vertexArray;

    SetMaterialStageCmdData()
    : m_shader( nullptr )
    , m_textures()
    , m_vertexArray( nullptr ) {
        // empty
    }
};

///	@brief
struct SetRenderTargetCmdData {
    ClearState m_clearState;
};

///	@brief
struct DrawInstancePrimitivesCmdData {
    OGLVertexArray        *m_vertexArray;
    ui32                   m_numInstances;
    CPPCore::TArray<ui32>  m_primitives;
    const char *m_id;

    DrawInstancePrimitivesCmdData()
    : m_vertexArray( nullptr )
    , m_numInstances( 0 )
    , m_primitives() {
        // empty
    }
};

///	@brief
struct DrawPrimitivesCmdData {
    bool                   m_localMatrix;
    glm::mat4              m_model;
    OGLVertexArray        *m_vertexArray;
    CPPCore::TArray<ui32>  m_primitives;
    const char *m_id;

    DrawPrimitivesCmdData()
    : m_localMatrix( false )
    , m_model()
    , m_vertexArray( nullptr )
    , m_primitives() {
        // empty
    }
};

struct OGLCapabilities {
    GLfloat m_maxAniso;

};

} // Namespace RendeBackend
} // Namespace OSRE
