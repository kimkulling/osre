/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/osre_common.h>

#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderStates.h>

namespace OSRE {
namespace RenderBackend {

//  Forward declarations
class OGLShader;

void checkOGLErrorState(const c8 *file, ui32 line);

//-------------------------------------------------------------------------------------------------
/// @ingroup    RenderSystem
///
/// @brief  This macro will check the OpenGL-error state and write a message into the log.
//-------------------------------------------------------------------------------------------------
#ifdef _DEBUG
#define CHECKOGLERRORSTATE() checkOGLErrorState(__FILE__, __LINE__)
#else
#define CHECKOGLERRORSTATE()
#endif // _DEBUG

static const GLuint OGLNotSetId = 999999;
static const GLint NoneLocation = -1;

///	@brief  This struct stores data for a OpenGL-based buffer.
struct OGLBuffer {
    size_t m_handle;
    BufferType m_type;
    GLuint m_oglId;
    size_t m_geoId;
    size_t m_size;

    /// @brief The default class constructor.
    OGLBuffer() :
            m_handle(0),
            m_type(BufferType::InvalidBufferType ),
            m_oglId(0),
            m_geoId(0),
            m_size(0) {
        // empty
    }
};

///	@brief  This struct stores data for a OpenGL-based vertex attribute.
struct OGLVertexAttribute {
    GLuint m_index;
    const c8 *m_pAttributeName;
    size_t m_size;
    GLenum m_type;
    const GLvoid *m_ptr;

    /// @brief The default class constructor.
    OGLVertexAttribute() :
            m_index( NoneLocation ),
            m_pAttributeName( nullptr ),
            m_size( 0 ),
            m_type( 0 ),
            m_ptr( nullptr ) {
        // empty
    }
};

///	@brief  Stis struct stores data for a OpenGL-based vertex-array.
struct OGLVertexArray {
    GLuint m_id;
    size_t m_slot;

    /// @brief The default class constructor.
    OGLVertexArray() :
            m_id(0),
            m_slot(99999999) {
        // empty
    }
};

///	@brief  This struct stores data for a OpenGL-based texture.
struct OGLTexture {
    GLuint m_textureId;
    String m_name;
    GLenum m_target;
    GLenum m_format;
    size_t m_slot;
    ui32 m_width;
    ui32 m_height;
    ui32 m_channels;

    OGLTexture():
            m_textureId(0),
            m_name(),
            m_target(0),
            m_format( 0 ),
            m_slot( 0 ),
            m_width( 0 ),
            m_height( 0 ),
            m_channels( 0 ) {
        // empty
    }
};

///	@brief  This enum describes the provided types of render commands.
enum class OGLRenderCmdType {
    SetParameterCmd,
    SetRenderTargetCmd,
    SetMaterialCmd,
    DrawPrimitivesCmd,
    DrawPrimitivesInstancesCmd,
    None
};

///	@brief
struct OGLRenderCmd {
    OGLRenderCmdType m_type;
    ui32 m_id;
    void *m_data;

    /// @brief The default class constructor.
    OGLRenderCmd(OGLRenderCmdType type);
};

inline OGLRenderCmd::OGLRenderCmd(OGLRenderCmdType type) :
        m_type(type),
        m_id(999999),
        m_data(nullptr) {
    // empty
}

struct UniformDataBlob;

///	@brief  This struct stores data for a opengl parameter aka uniform.
struct OGLParameter {
    String m_name;
    GLint m_loc;
    ParameterType m_type;
    UniformDataBlob *m_data;
    size_t m_numItems;

    /// @brief The default class constructor.
    OGLParameter() :
            m_name(),
            m_loc(NoneLocation),
            m_type(ParameterType::PT_None),
            m_data(nullptr),
            m_numItems(1) {
        // empty
    }
};

///	@brief  This struct describes a primitive group.
struct OGLPrimGroup {
    GLenum m_primitive;
    ui32 m_startIndex;
    size_t m_numIndices;
    GLenum m_indexType;
};

///	@brief  This struct stores data for the uniform setup for render batches.
struct SetParameterCmdData {
    MatrixBuffer *mBuffer;

    /// @brief The default class constructor.
    SetParameterCmdData() :
            mBuffer( nullptr ) {
        // empty
    }
};

///	@brief  This struct stores data for a material setup command.
struct SetMaterialStageCmdData {
    OGLShader *m_shader;
    CPPCore::TArray<OGLTexture *> m_textures;
    OGLVertexArray *m_vertexArray;

    /// @brief The default class constructor.
    SetMaterialStageCmdData() :
            m_shader(nullptr),
            m_textures(),
            m_vertexArray(nullptr) {
        // empty
    }
};

///	@brief  This struct stores data for the setup of a render-target.
struct SetRenderTargetCmdData {
    ClearState m_clearState;

    /// @brief The default class constructor.
    SetRenderTargetCmdData() {
        // empty
    }
};

///	@brief  This struct stores data for a instanced primitive render call.
struct DrawInstancePrimitivesCmdData {
    OGLVertexArray *m_vertexArray;
    size_t m_numInstances;
    CPPCore::TArray<size_t> m_primitives;
    const char *m_id;

    /// @brief The default class constructor.
    DrawInstancePrimitivesCmdData() :
            m_vertexArray(nullptr),
            m_numInstances(0),
            m_primitives(),
            m_id(nullptr) {
        // empty
    }
};

///	@brief  This struct stores data for a primitive render call.
struct DrawPrimitivesCmdData {
    bool m_localMatrix;
    glm::mat4 m_model;
    OGLVertexArray *m_vertexArray;
    CPPCore::TArray<size_t> m_primitives;
    const char *m_id;

    /// @brief The default class constructor.
    DrawPrimitivesCmdData() :
            m_localMatrix(false),
            m_model(),
            m_vertexArray(nullptr),
            m_primitives(),
            m_id(nullptr) {
        // empty
    }
};

/// @brief This struct collects all capabilities of the current opengl render-context.
struct OGLCapabilities {
    GLfloat mMaxAniso;
    i32 mContextMask;
    i32 mMax3DTextureSize;
    i32 mMaxTextureUnits;
    i32 mMaxTextureImageUnits;
    i32 mMaxTextureCoords;
    bool mInstancing;

    /// @brief The default class constructor.
    OGLCapabilities() :
            mMaxAniso(0.0f),
            mContextMask(-1),
            mMax3DTextureSize(-1),
            mMaxTextureUnits(-1),
            mMaxTextureImageUnits(-1),
            mMaxTextureCoords(-1),
            mInstancing(true) {
        // empty
    }
};

struct OGLFrameBuffer {
    const char *m_name;             ///<
    GLuint m_bufferId;              ///<
    GLuint m_depthrenderbufferId;   ///<
    GLuint m_renderedTexture;       ///<
    ui32 m_width;                   ///< The width
    ui32 m_height;                  ///<

    /// @brief The class constructor with the framebuffer parameter.
    /// @param name     The framebuffer name.
    /// @param w        The width for the framebuffer.
    /// @param h        The height for the framebuffer.
    OGLFrameBuffer(const char *name, ui32 w, ui32 h) :
            m_name(name),
            m_bufferId(0),
            m_depthrenderbufferId(0),
            m_renderedTexture(0),
            m_width(w),
            m_height(h) {
        // empty
    }
};

} // namespace RenderBackend
} // Namespace OSRE
