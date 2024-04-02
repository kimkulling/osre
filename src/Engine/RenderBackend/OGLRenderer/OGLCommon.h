/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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

//  Forward declarations --------------------------------------------------------------------------
class OGLShader;

struct OGLFrameBuffer;
struct UniformDataBlob;

/// @brief Check if an OpenGL-spcific eror was detected.
/// @param[in] file     The filename, where the error has occurred.
/// @param[in] line     The linenumber, where the error has occurred.
void checkOGLErrorState(const c8 *file, ui32 line);

//-------------------------------------------------------------------------------------------------
/// @ingroup    Engine
///
/// @brief  This macro will check the OpenGL-error state and write a message into the log.
//-------------------------------------------------------------------------------------------------
#ifdef _DEBUG
#define CHECKOGLERRORSTATE() checkOGLErrorState(__FILE__, __LINE__)
#else
#define CHECKOGLERRORSTATE()
#endif // _DEBUG

static constexpr GLuint OGLNotSetId = 999999;   ///< Indicates a not inited opengl id.
static constexpr GLint  NoneLocation = -1;      ///< Indicates a not existing location of an uniform variable.

///	@brief  This struct declares opengl-specific buffer resources.
/// Buffer will be used to store different data like vertices, indices or binary data.
struct OGLBuffer {
    size_t m_handle;    ///< The buffer type.
    BufferType m_type;  ///< The buffer type.
    GLuint m_oglId;     ///< The OpenGL buffer id.
    size_t m_geoId;     ///< The internal geo id.
    size_t m_size;      ///< The buffer size.

    /// @brief The default class constructor.
    OGLBuffer() : m_handle(0), m_type(BufferType::InvalidType), m_oglId(OGLNotSetId), m_geoId(0), m_size(0){}
    
    /// @brief  The class destructor, default implementation.
    ~OGLBuffer() = default;
};

///	@brief  This struct declares an OpenGL specific vertex attribute like position or normals.
struct OGLVertexAttribute {
    GLuint m_index;             ///< The attribute index used in the shader.
    const c8 *m_pAttributeName; ///< The attribute name.
    size_t m_size;              ///< The size for one attribute.
    GLenum m_type;              ///< The attribute type.
    const GLvoid *m_ptr;        ///< The data pointer for the attribute.

    /// @brief The default class constructor.
    OGLVertexAttribute() : m_index(999999), m_pAttributeName(nullptr), m_size(0U), m_type(), m_ptr(nullptr) {}

    /// @brief  The class destructor, default implementation.
    ~OGLVertexAttribute() = default;
};

///	@brief  The OpenGL vertex array description.
struct OGLVertexArray {
    GLuint m_id;    ///< The vertex array id.
    size_t m_slot;  ///< The slot id, used as an internal index.

    /// @brief The default class constructor.
    OGLVertexArray() : m_id(0), m_slot(99999999) {}

    /// @brief  The class destructor, default implementation.
    ~OGLVertexArray() = default;
};

///	@brief  This struct represents a txture resource information.
struct OGLTexture {
    GLuint m_textureId;     ///< The OpenGL texture id.
    String m_name;          ///< The texture name.
    GLenum m_target;        ///< The texture target type.
    GLenum m_format;        ///< The texture format type.
    size_t m_slot;          ///< The slot id, used as an internal index.
    ui32 m_width;
    ui32 m_height;
    ui32 m_channels;

    /// @brief The default class constructor.
    OGLTexture() : m_textureId(OGLNotSetId), m_name(), m_target(GL_NONE), m_format(GL_NONE), 
                   m_slot(OGLNotSetId), m_width(0), m_height(0), m_channels(0) {}

    /// @brief  The class destructor, default implementation.
    ~OGLTexture() = default;
};

using OGLTextureArray = cppcore::TArray<OGLTexture*>;

///	@brief  This enum is used to describe a render command type.
enum class OGLRenderCmdType {
    SetParameterCmd,
    SetRenderTargetCmd,
    SetMaterialCmd,
    DrawPrimitivesCmd,
    DrawPrimitivesInstancesCmd,
    None
};

///	@brief This struct declares the render command data.
struct OGLRenderCmd {
    OGLRenderCmdType m_type;    ///< The command type
    ui32 m_id;                  ///< The command id.
    void *m_data;               ///< The command data.

    /// @brief The default class constructor.
    OGLRenderCmd(OGLRenderCmdType type) : m_type(type), m_id(999999), m_data(nullptr) {}

    /// @brief  The class destructor, default implementation.
    ~OGLRenderCmd() = default;
};

///	@brief This struct declares the needed data for a OpenGL parameter.
struct OGLParameter {
    String m_name;              ///< The parameter name.
    GLint m_loc;                ///< The parameter location in the shader.
    ParameterType m_type;       ///< The parameter type.
    UniformDataBlob *m_data;    ///< The data blob.
    size_t m_numItems;          ///< Number of items.

    /// @brief The default class constructor.
    OGLParameter() :  m_name(""), m_loc(NoneLocation), m_type(ParameterType::PT_None), 
                      m_data(nullptr), m_numItems(0) {}

    /// @brief  The class destructor, default implementation.
    ~OGLParameter() = default;
};

///	@brief This struct declares the data for a group of render primitives for a render call.
struct OGLPrimGroup {
    GLenum m_primitive;     ///< The primitive type.
    ui32 m_startIndex;      ///< The start index in the vertex buffer.
    size_t m_numIndices;    ///< The number of indices to render.
    GLenum m_indexType;     ///< The index data type.

    /// @brief The default class constructor.
    OGLPrimGroup() : m_primitive(GL_NONE), m_startIndex(0), m_numIndices(0), m_indexType(GL_NONE) {} 

    /// @brief  The class destructor, default implementation.
    ~OGLPrimGroup() = default;
};

///	@brief  This struct declares the data for a rendercall to set the correct material 
///         for the coming render calls.
struct SetMaterialStageCmdData {
    OGLShader *m_shader;                        ///< The shader to use.
    cppcore::TArray<OGLTexture *> m_textures;   ///< All texture to set.
    OGLVertexArray *m_vertexArray;              ///< Ther vertex array.

    /// @brief The default class constructor.
    SetMaterialStageCmdData() : m_shader(nullptr), m_textures(), m_vertexArray(nullptr) {}

    /// @brief  The class destructor, default implementation.
    ~SetMaterialStageCmdData() = default;
};

///	@brief  The command data for setting the render target.
struct SetRenderTargetCmdData {
    ClearState mClearState;         ///< The clear state for the render target.
    OGLFrameBuffer *mFrameBuffer;   ///< The framebuffer to use.

    /// @brief The default class constructor.
    SetRenderTargetCmdData() : mClearState( 0 ), mFrameBuffer( nullptr ) {}

    /// @brief  The class destructor, default implementation.
    ~SetRenderTargetCmdData() = default;
};

///	@brief This struct declares the data for a render call with instanced render data.
struct DrawInstancePrimitivesCmdData {
    OGLVertexArray *m_vertexArray;          ///< The vertex array to use.
    size_t m_numInstances;                  ///< The number of instances to render.
    cppcore::TArray<size_t> m_primitives;   ///< The primitives to render.
    const char *m_id;                       ///< The call id.

    /// @brief The default class constructor.
    DrawInstancePrimitivesCmdData() : m_vertexArray(nullptr), m_numInstances(0), m_primitives(), m_id(nullptr) {}

    /// @brief  The class destructor, default implementation.
    ~DrawInstancePrimitivesCmdData() = default;
};

///	@brief  Thsi struct declares the data for a simple render call.
struct DrawPrimitivesCmdData {
    bool m_localMatrix;                     ///< true for a local model matrix. TODO: Remove me
    glm::mat4 m_model;                      ///< The model matrix. TODO: Remove me
    OGLVertexArray *m_vertexArray;          ///< The vertex array to use.
    cppcore::TArray<size_t> m_primitives;   ///< The primitives to render.
    const char *m_id;                       ///< The id.

    /// @brief The default class constructor.
    DrawPrimitivesCmdData() : m_localMatrix(false), m_model(), m_vertexArray(nullptr), m_primitives(), m_id(nullptr) {}

    /// @brief  The class destructor, default implementation.
    ~DrawPrimitivesCmdData() = default;
};

struct OGLDriverInfo {
    i32 mOpenGLVersion[2];
    String mExtensions;
    const c8 *mGLVendorString;
    const c8 *mGLRendererString;
    const c8 *mGLVersionString;

    OGLDriverInfo() :
            mGLVendorString(nullptr), mGLRendererString(nullptr), mGLVersionString(nullptr) {
        ::memset(mOpenGLVersion, 0, sizeof(i32) * 2);
    }
};

/// @brief This struct declares the data structure of the GPU capabilities.
struct OGLCapabilities {
    GLfloat mMaxAniso;          ///< Max value for aniso.
    i32 mContextMask;           ///< ?
    i32 mMax3DTextureSize;      ///< The upper limit for 3D textures.
    i32 mMaxTextureUnits;       ///< The maximal number of texture units.
    i32 mMaxTextureImageUnits;  ///< The maximal number of texture image units.
    i32 mMaxTextureCoords;      ///< The maximal number of texture coordinates.
    bool mInstancing;           ///< Instancing is supported.
    c8 *mGLSLVersionAsStr;      ///< The GLSL version as a string
    GLSLVersion mGLSLVersion;   ///< The GLSL version as an enum

    /// @brief The default class constructor.
    OGLCapabilities() :
            mMaxAniso(0.0f),
            mContextMask(-1),
            mMax3DTextureSize(-1),
            mMaxTextureUnits(-1),
            mMaxTextureImageUnits(-1),
            mMaxTextureCoords(-1),
            mInstancing(true),
            mGLSLVersionAsStr(nullptr),
            mGLSLVersion(GLSLVersion::Invalid) {
        // empty
    }

    /// @brief  The class destructor, default implementation.
    ~OGLCapabilities() = default;
};

/// @brief This struct declares the data for an OpenGL framebuffer.
struct OGLFrameBuffer {
    const char *m_name;             ///< The name of the framebuffer.
    GLuint m_bufferId;              ///< The OpenGL buffer id.
    GLuint m_depthrenderbufferId;   ///< The depth buffer id.
    GLuint m_renderedTexture;       ///< The OpenGL id for the texture to rnder in.
    ui32 m_width;                   
    ui32 m_height;

    /// @brief The default class constructor.
    OGLFrameBuffer(const char *name, ui32 w, ui32 h) : 
        m_name(name), m_bufferId(0), m_depthrenderbufferId(0), 
        m_renderedTexture(0), m_width(w), m_height(h) {}

    /// @brief  The class destructor, default implementation.
    ~OGLFrameBuffer() = default;
};

} // namespace RenderBackend
} // namespace OSRE
