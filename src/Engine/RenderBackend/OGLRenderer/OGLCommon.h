/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "Common/osre_common.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/RenderStates.h"

namespace OSRE::RenderBackend {

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
#   define CHECKOGLERRORSTATE() checkOGLErrorState(__FILE__, __LINE__)
#else
#   define CHECKOGLERRORSTATE()
#endif // _DEBUG

static constexpr GLuint OGLNotSetId = 999999;   ///< Indicates a not inited opengl id.
static constexpr GLint  NoneLocation = -1;      ///< Indicates a not existing location of an uniform variable.

///	@brief  This struct declares opengl-specific buffer resources.
/// Buffer will be used to store different data like vertices, indices or binary data.
struct OGLBuffer {
    size_t     handle;    ///< The buffer type.
    BufferType type;      ///< The buffer type.
    GLuint     oglId;     ///< The OpenGL buffer id.
    size_t     geoId;     ///< The internal geo id.
    size_t     size;      ///< The buffer size.

    /// @brief The default class constructor.
    OGLBuffer() : handle(0), type(BufferType::Invalid), oglId(OGLNotSetId), geoId(0), size(0){}
};

///	@brief  This struct declares an OpenGL specific vertex attribute like position or normals.
struct OGLVertexAttribute {
    GLuint          index;              ///< The attribute index used in the shader.
    const c8       *attributeName;      ///< The attribute name.
    size_t          size;               ///< The size for one attribute.
    GLenum          type;               ///< The attribute type.
    const GLvoid   *ptr;                ///< The data pointer for the attribute.

    /// @brief The default class constructor.
    OGLVertexAttribute() :
            index(OGLNotSetId), attributeName(nullptr), size(0U), type(), ptr(nullptr) {}
};

///	@brief  The OpenGL vertex array description.
struct OGLVertexArray {
    GLuint id;          ///< The vertex array id.
    size_t slot;        ///< The slot id, used as an internal index.

    /// @brief The default class constructor.
    OGLVertexArray() :
            id(0), slot(OGLNotSetId) {}
};

///	@brief  This struct represents a txture resource information.
struct OGLTexture {
    GLuint  textureId;      ///< The OpenGL texture id.
    String  name;           ///< The texture name.
    GLenum  target;         ///< The texture target type.
    GLenum  format;         ///< The texture format type.
    size_t  slot;           ///< The slot id, used as an internal index.
    ui32    width;          ///< The texture width.
    ui32    height;         ///< The texture height.
    ui32    channels;       ///< The number of chanels of the texture.

    /// @brief The default class constructor.
    OGLTexture() : textureId(OGLNotSetId), name(), target(GL_NONE), format(GL_NONE), 
                   slot(OGLNotSetId), width(0), height(0), channels(0) {}
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
    OGLRenderCmdType    type;   ///< The command type
    ui32                id;     ///< The command id.
    void               *data;   ///< The command data.

    /// @brief The default class constructor.
    OGLRenderCmd(OGLRenderCmdType type) :
            type(type), id(OGLNotSetId), data(nullptr) {}
};

///	@brief This struct declares the needed data for a OpenGL parameter.
struct OGLParameter {
    String              name;       ///< The parameter name.
    GLint               loc;        ///< The parameter location in the shader.
    ParameterType       type;       ///< The parameter type.
    UniformDataBlob    *data;       ///< The data blob.
    size_t              numItems;   ///< Number of items.

    /// @brief The default class constructor.
    OGLParameter() :  name(""), loc(NoneLocation), type(ParameterType::PT_None), 
                      data(nullptr), numItems(0) {}
};

///	@brief This struct declares the data for a group of render primitives for a render call.
struct OGLPrimGroup {
    GLenum  primitive;     ///< The primitive type.
    ui32    startIndex;    ///< The start index in the vertex buffer.
    size_t  numIndices;    ///< The number of indices to render.
    GLenum  indexType;     ///< The index data type.

    /// @brief The default class constructor.
    OGLPrimGroup() : primitive(GL_NONE), startIndex(0), numIndices(0), indexType(GL_NONE) {} 
};

///	@brief  This struct declares the data for a rendercall to set the correct material 
///         for the coming render calls.
struct SetMaterialStageCmdData {
    using OGLTexArray = cppcore::TArray<OGLTexture*>;

    OGLShader      *shader;         ///< The shader to use.
    OGLTexArray     textures;       ///< All texture to set.
    OGLVertexArray *vertexArray;    ///< Ther vertex array.

    /// @brief The default class constructor.
    SetMaterialStageCmdData() : shader(nullptr), textures(), vertexArray(nullptr) {}
};

///	@brief  The command data for setting the render target.
struct SetRenderTargetCmdData {
    ClearState      clearState;    ///< The clear state for the render target.
    OGLFrameBuffer *frameBuffer;   ///< The framebuffer to use.

    /// @brief The default class constructor.
    SetRenderTargetCmdData() : clearState(0), frameBuffer(nullptr) {}
};

///	@brief This struct declares the data for a render call with instanced render data.
struct DrawInstancePrimitivesCmdData {
    OGLVertexArray         *vertexArray;    ///< The vertex array to use.
    size_t                  numInstances;   ///< The number of instances to render.
    cppcore::TArray<size_t> primitives;     ///< The primitives to render.
    const char             *id;             ///< The call id.

    /// @brief The default class constructor.
    DrawInstancePrimitivesCmdData() : vertexArray(nullptr), numInstances(0), primitives(), id(nullptr) {}
};

///	@brief  Thsi struct declares the data for a simple render call.
struct DrawPrimitivesCmdData {
    bool                    localMatrix;  ///< true for a local model matrix. TODO: Remove me
    glm::mat4               model;        ///< The model matrix. TODO: Remove me
    OGLVertexArray         *vertexArray;  ///< The vertex array to use.
    cppcore::TArray<size_t> primitives;   ///< The primitives to render.
    const char *id;                       ///< The id.

    /// @brief The default class constructor.
    DrawPrimitivesCmdData() : localMatrix(false), model(), vertexArray(nullptr), primitives(), id(nullptr) {
        // empty
    }
};

/// @brief This struct stores all the related data to the OpenGL driver
struct OGLDriverInfo {
    i32         openGLVersion[2];   ///< The supported OpenGL version.
    String      extensions;         ///< The list of supported extensons.
    const c8   *glVendorString;     ///< The OpenGL vendor string.
    const c8   *glRendererString;   ///< The OpenGL rendererer as a string.
    const c8   *glVersionString;    ///< The OpenGL version as a string.

    /// @brief The class constructor.
    OGLDriverInfo() :
            glVendorString(nullptr), glRendererString(nullptr), glVersionString(nullptr) {
        ::memset(openGLVersion, 0, sizeof(i32) * 2);
    }
};

/// @brief This struct declares the data structure of the GPU capabilities.
struct OGLCapabilities {
    GLfloat         maxAniso;                   ///< Max value for aniso.
    i32             contextMask;                ///< ?
    i32             max3DTextureSize;           ///< The upper limit for 3D textures.
    i32             maxTextureUnits;            ///< The maximal number of texture units.
    i32             maxTextureImageUnits;       ///< The maximal number of texture image units.
    i32             maxTextureCoords;           ///< The maximal number of texture coordinates.
    i32             maxVertexAttributes;        ///< The maximum number of vertex attributes.
    bool            instancing;                 ///< Instancing is supported.
    const c8        *glslVersionAsStr;          ///< The GLSL version as a string
    GLSLVersion     glslVersion;                ///< The GLSL version as an enum

    /// @brief The default class constructor.
    OGLCapabilities() :
            maxAniso(0.0f),
            contextMask(-1),
            max3DTextureSize(-1),
            maxTextureUnits(-1),
            maxTextureImageUnits(-1),
            maxTextureCoords(-1),
            maxVertexAttributes(-1),
            instancing(true),
            glslVersionAsStr(nullptr),
            glslVersion(GLSLVersion::Invalid) {
        // empty
    }
};

/// @brief This struct declares the data for an OpenGL framebuffer.
struct OGLFrameBuffer {
    const char *name;                   ///< The name of the framebuffer.
    GLuint      bufferId;               ///< The OpenGL buffer id.
    GLuint      depthrenderbufferId;    ///< The depth buffer id.
    GLuint      renderedTexture;        ///< The OpenGL id for the texture to rnder in.
    ui32        width;                  ///< The width of the framebuffer
    ui32        height;                 ///< The height of the framebuffer

    /// @brief The default class constructor.
    OGLFrameBuffer(const char *name, ui32 w, ui32 h) : 
        name(name), bufferId(0), depthrenderbufferId(0), 
        renderedTexture(0), width(w), height(h) {}
};

} // namespace OSRE::RenderBackend
