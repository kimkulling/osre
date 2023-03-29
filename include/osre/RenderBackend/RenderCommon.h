/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/TResource.h>
#include <osre/RenderBackend/Shader.h>
#include <osre/Common/osre_common.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/Uri.h>
#include <osre/Common/glm_common.h>

#include <cppcore/Container/TArray.h>
#include <cppcore/Container/THashMap.h>
#include <cppcore/Container/TStaticArray.h>
#include <cppcore/Memory/TPoolAllocator.h>

namespace OSRE {
namespace RenderBackend {

// Forward declarations ---------------------------------------------------------------------------
struct UniformVar;
struct FrameBuffer;

class Mesh;
class Shader;
class Pipeline;

/// @brief An array to stoer meshes.
using MeshArray = cppcore::TArray<RenderBackend::Mesh*>;

/// Describes an unset id.
static constexpr i32 UnsetHandle = -1;

/// Upper limits for names.
static constexpr ui32 MaxEntNameLen = 256;

///	@brief  This enum describes the usage of a GPU-buffer-object.
///
/// Buffer objects are used to store vertex-, index- or image data on the GPU-memory
/// 
enum class BufferType {
    InvalidType = -1,   ///< Enum for invalid enum.
    EmptyBuffer = 0,    ///< Empty buffer, no special use.
    VertexBuffer,       ///< Vertex buffer, stores vertex data inside.
    IndexBuffer,        ///< Index buffer, stores indices inside.
    InstanceBuffer,     ///< Instance buffer, will store instance-specific data.
    UniformBuffer,      ///< Uniform buffer, used for structured uniform data.
    NumBufferTypes      ///< Number of enums.
};

/// @brief  This enum describes the supported access types for render buffers.
enum class BufferAccessType {
    InvalidType = -1,       ///< Enum for invalid enum.
    ReadOnly = 0,           ///< Read only access.
    WriteOnly,              ///< Write only access.
    ReadWrite,              ///< Read and write access.
    NumBufferAccessTypes    ///< Number of enum's.
};

/// @brief The enum is used to describe the different pixel formats
enum class PixelFormatType {
    InvaliTextureType=-1,   ///< Marker for an invalid texture.
    R8G8B8 = 0,             ///< 24 bit data, r, g, b
    R8G8B8A8,               ///< 32 bit data, r, g, b, a
    NumPixelFormatTypes     ///< The number of formats
};

///	@brief  This enum describes the build-in vertex types provided by OSRE, mainly used for demos and examples.
enum class VertexType {
    InvalidVetexType = -1,  ///< Marker for an invalid data type.
    ColorVertex = 0,        ///< A simple vertex consisting of position and color.
    RenderVertex,           ///< A render vertex with position, color, normals and texture coordinates.
    NumVertexTypes          ///< Number of enums.
};

///	@brief  This enum describes the supported texture target types.
enum class TextureTargetType {
    InvalidTextureTargetType = -1,  ///< Enum for invalid enum.
    Texture1D = 0,                  ///< 1D-textures, used for simple arrays in shaders.
    Texture2D,                      ///< 2D-textures, used for images and render targets.
    Texture3D,                      ///< 3D-textures, used for volume rendering.
    NumTextureTargetTypes           ///< Number of enums.

};

///	@brief  This enum describes the supported texture stages. A texture stage describes one layer of
/// a texture composition like a diffuse texture at stage0 and a lighting information at statge1.
enum class TextureStageType {
    InvalidTextureStageType = -1,   ///< Enum for invalid enum.
    TextureStage0 = 0,              ///< Texture state level 0
    TextureStage1,                  ///< Texture state level 1
    TextureStage2,                  ///< Texture state level 2
    TextureStage3,                  ///< Texture state level 3
    NumTextureStageTypes            ///< Number of enums.
};

///	@brief  This enum describes different texture parameter names.
enum class TextureParameterName {
    InvalidTextureParameterName = -1,   ///< Enum for invalid enum.
    TextureParamMinFilter = 0,          ///< The min filter name.
    TextureParamMagFilter,              ///< The mag filter name.
    TextureParamWrapS,                  ///< The wrap name for s
    TextureParamWrapT,                  ///< The warp name for t.
    NumTextureParameterNames            ///< Number of enums.
};

///	@brief  This enum describes the parameters which are related the the parameter names ( @see TextureParameterName ).
enum class TextureParameterType {
    TexturePTNearest = 0, ///< Use nearest filter mode.
    TexturePTLinear, ///< Use linear interpolation mode.
    TexturePTClamp, ///< Use clamp mode, texture data will be clamped.
    TexturePTMirroredRepeat, ///< Use mirror repeat mode, texture will be repeated mirrored.
    TexturePTRepeat, ///< Use repeat mode, texture will be repeated mirrored.
    NumTextureParameterTypes, ///< Number of enums.

    InvalidTextureParameterType, ///< Enum for invalid enum.
};

///	@brief  This enum describes the index data type.
enum class IndexType {
    UnsignedByte = 0, ///< Bytes are used for the index data.
    UnsignedShort, ///< Unsigned short for the index data.
    UnsignedInt, ///< Unsigned int for the index data.
    NumIndexTypes, ///< Number of enums.

    InvalidIndexType, ///< Enum for invalid enum.
};

///	@brief  This enum describes the primitive types for rendering vertex information.
enum class PrimitiveType {
    PointList = 0, ///< A list of points, one index per point.
    LineList, ///< A list of separate lines, 2 indices per line.
    LineStrip, ///< A line strip, Start and end-index and all indices between.
    TriangleList, ///< A list of triangles, 3 indices per triangle.
    TriangelStrip, ///< A strip of triangles
    TriangleFan, ///< A triangle fan.
    NumPrimitiveTypes, ///< Number of enums.

    InvalidPrimitiveType, ///< Enum for invalid enum.
};

/// @brief  This enum is used to describe the type of build-in matrices.
enum class MatrixType {
    Model = 0, ///<
    View, ///<
    Projection, ///<
    Normal, ///<
    NumMatrixTypes, ///< Number of matrix types-

    InvalidMatrixType ///< Enum for invalid values.
};

/// @brief  This enum is used to describe the data-type of a parameter.
enum class ParameterType {
    PT_None,
    PT_Int,
    PT_IntArray,
    PT_Float,
    PT_FloatArray,
    PT_Float2,
    PT_Float2Array,
    PT_Float3,
    PT_Float3Array,
    PT_Mat4,
    PT_Mat4Array,

    InvalidParameterType
};

///	@brief  This enum to describes the type of the vertex attribute.
enum class VertexAttribute : int {
    Position = 0, ///< "position"
    Normal, ///< "normal"
    TexCoord0, ///< "texcoord0"
    TexCoord1, ///< "texcoord1"
    TexCoord2, ///< "texcoord2"
    TexCoord3, ///< "texcoord3"
    Tangent, ///< "tangent
    Binormal, ///< "binormal"
    Weights, ///< "weights" (skin weights)
    Indices, ///< "indices" (skin indices)
    Color0, ///< "color0"
    Color1, ///< "color1"
    Instance0, ///< "instance0"
    Instance1, ///< "instance1"
    Instance2, ///< "instance2"
    Instance3, ///< "instance3"
    NumVertexAttrs, ///< Number of enums.

    InvalidVertexAttr ///< Enum for invalid enum.
};

///	@brief  This enum describes the vertex data format.
enum class VertexFormat : int {
    Float, ///< single component float, expanded to (x, 0, 0, 1)
    Float2, ///< 2-component float, expanded to (x, y, 0, 1)
    Float3, ///< 3-component float, expanded to (x, y, z, 1)
    Float4, ///< 4-component float
    Byte4, ///< 4-component float (-128.0f..+127.0f) mapped to byte (-128..+127)
    UByte4, ///< 4-component float (0.0f..255.0f) mapped to byte (0..255)
    Short2, ///< 2-component float (-32768.0f..+32767.0f) mapped to short (-32768..+32768)
    Short4, ///< 4-component float (-32768.0f..+32767.0f) mapped to short (-32768..+32768)
    NumVertexFormats, ///< Number of enums.

    InvalidVertexFormat, ///< Enum for invalid enum.
};

///	@brief
enum class LightType {
    Directional = 0,
    Point,
    Spot,
    NumLightTypes,

    InvalidLightType ///< Enum for invalid enum.
};

///	@brief  This struct declares a render vertex for textured geometry.
struct OSRE_EXPORT ColorVert {
    glm::vec3 position; ///< The position ( x|y|z )
    glm::vec3 normal; ///< The normal vector ( x|y|z )
    glm::vec3 color0; ///< The diffuse color ( r|g|b )

    ColorVert();
    ~ColorVert() = default;

    /// @brief  Returns the number of attributes.
    static size_t getNumAttributes();
    /// @brief  Returns the attribute array.
    static const String *getAttributes();
};

///	@brief  This struct declares a render vertex for textured geometry.
struct OSRE_EXPORT RenderVert {
    glm::vec3 position; ///< The position ( x|y|z )
    glm::vec3 normal; ///< The normal vector ( x|y|z )
    glm::vec3 color0; ///< The diffuse color ( r|g|b )
    glm::vec2 tex0; ///< The texture color ( r|g|b )

    RenderVert();
    ~RenderVert();
    bool operator==(const RenderVert &rhs) const;
    bool operator!=(const RenderVert &rhs) const;

    /// @brief  Returns the number of attributes.
    static size_t getNumAttributes();

    /// @brief  Returns the attribute array.
    static const String *getAttributes();
};

OSRE_EXPORT const String &getVertCompName(VertexAttribute attrib);

struct OSRE_EXPORT UIVert {
    glm::vec2 position; ///< The position ( x|y )
    glm::vec4 color0; ///< The diffuse color ( r|g|b|a )

    UIVert() = default;
    ~UIVert() = default;

    /// @brief  Returns the number of attributes.
    static size_t getNumAttributes();

    /// @brief  Returns the attribute array.
    static const String *getAttributes();
};

///	@brief  Utility function for calculate the vertex format size.
inline size_t getVertexFormatSize(VertexFormat format) {
    ui32 size(0);
    switch (format) {
        case VertexFormat::Float:
            size = sizeof(f32);
            break;
        case VertexFormat::Float2:
            size = sizeof(f32) * 2;
            break;
        case VertexFormat::Float3:
            size = sizeof(f32) * 3;
            break;
        case VertexFormat::Float4:
            size = sizeof(f32) * 4;
            break;
        case VertexFormat::Byte4:
            size = sizeof(c8) * 4;
            break;
        case VertexFormat::UByte4:
            size = sizeof(uc8) * 4;
            break;
        case VertexFormat::Short2:
            size = sizeof(ui16) * 2;
            break;
        case VertexFormat::Short4:
            size = sizeof(ui16) * 4;
            break;
        case VertexFormat::NumVertexFormats:
        case VertexFormat::InvalidVertexFormat:
            break;

        default:
            break;
    }

    return size;
}

/// @brief  This struct declares an extension description.
struct ExtensionProperty {
    c8 m_extensionName[MaxEntNameLen];
    ui32 m_version;

    ExtensionProperty() {
        ::memset(m_extensionName, '\0', sizeof(c8) * MaxEntNameLen);
        m_version = 0;
    }
    ~ExtensionProperty() = default;
};

///	@brief
struct OSRE_EXPORT VertComponent {
    VertexAttribute m_attrib;
    VertexFormat m_format;

    VertComponent();
    VertComponent(VertexAttribute attrib, VertexFormat format);
    ~VertComponent();

    OSRE_NON_COPYABLE(VertComponent)
};

///	@brief
struct OSRE_EXPORT VertexLayout {
    static VertComponent ErrorComp;
    String *m_attributes;
    cppcore::TArray<VertComponent *> m_components;
    cppcore::TArray<size_t> m_offsets;
    size_t m_currentOffset;
    size_t m_sizeInBytes;

    VertexLayout();
    ~VertexLayout();
    void clear();
    size_t numComponents() const;
    size_t sizeInBytes();
    VertexLayout &add(VertComponent *comp);
    VertComponent &getAt(size_t idx) const;
    const String *getAttributes();

    OSRE_NON_COPYABLE(VertexLayout)
};

///	@brief  This struct is used to describe data for a GPU buffer.
struct OSRE_EXPORT BufferData {
    using BufferDataAllocator = ::cppcore::TPoolAllocator<BufferData>;
    friend BufferDataAllocator;
    static BufferDataAllocator sBufferDataAllocator;

    BufferType m_type; ///< The buffer type ( @see BufferType )
    MemoryBuffer m_buffer; ///< The memory buffer
    size_t m_cap; ///<
    BufferAccessType m_access; ///< Access token ( @see BufferAccessType )

    static BufferData *alloc(BufferType type, size_t sizeInBytes, BufferAccessType access);
    static void free(BufferData *data);
    void copyFrom(void *data, size_t size);
    void attach(const void *data, size_t size);
    BufferType getBufferType() const;
    BufferAccessType getBufferAccessType() const;
    size_t getSize() const;
    c8 *getData();

private:
    BufferData();
    ~BufferData();

    OSRE_NON_COPYABLE(BufferData)
};

inline size_t BufferData::getSize() const {
    return m_buffer.size();
}

inline c8 *BufferData::getData() {
    return (c8 *)&m_buffer[0];
}

///	@brief
struct OSRE_EXPORT PrimitiveGroup {
    PrimitiveType m_primitive;
    size_t m_startIndex;
    size_t m_numIndices;
    IndexType m_indexType;

    PrimitiveGroup();
    ~PrimitiveGroup() = default;
    void init(IndexType indexType, size_t numPrimitives, PrimitiveType primType, size_t startIdx);

    OSRE_NON_COPYABLE(PrimitiveGroup)
};

///	@brief
struct OSRE_EXPORT Texture {
    String m_textureName;
    IO::Uri m_loc;
    TextureTargetType m_targetType;
    PixelFormatType mPixelFormat;
    ui32 m_size;
    uc8 *m_data;
    ui32 m_width;
    ui32 m_height;
    ui32 m_channels;
    Handle m_texHandle;

    Texture();
    ~Texture();

    OSRE_NON_COPYABLE(Texture)
};

///	@brief
class OSRE_EXPORT TextureLoader {
public:
    TextureLoader();
    ~TextureLoader();
    size_t load(const IO::Uri &uri, Texture *tex);
    bool unload(Texture *tex);
    static RenderBackend::Texture *getDefaultTexture();
};

///	@brief  This class is used to represent a texture resource.
class OSRE_EXPORT TextureResource : public Common::TResource<Texture, TextureLoader> {
public:
    TextureResource(const String &name, const IO::Uri &uri);
    ~TextureResource() override = default;
    void setTargetType(TextureTargetType targetType);
    TextureTargetType getTargetType() const;
    void setTextureStage(TextureStageType stage);
    TextureStageType setTextureStage() const;

protected:
    Common::ResourceState onLoad(const IO::Uri &uri, TextureLoader &loader) override;
    Common::ResourceState onUnload(TextureLoader &loader) override;

private:
    TextureTargetType m_targetType;
    TextureStageType m_stage;
};


///	@brief
struct OSRE_EXPORT GeoInstanceData {
    BufferData *m_data;

    GeoInstanceData();
    ~GeoInstanceData();

    OSRE_NON_COPYABLE(GeoInstanceData)
};

///	@brief
struct OSRE_EXPORT TransformState {
    glm::vec3 m_translate;
    glm::vec3 m_scale;
    glm::mat4 m_rotation;

    TransformState();
    ~TransformState();
    void setTranslation(f32 x, f32 y, f32 z);
    void setScale(f32 sx, f32 sy, f32 sz);
    void toMatrix(glm::mat4 &m) const;
    bool operator==(const TransformState &rhs) const;
    bool operator!=(const TransformState &rhs) const;
};

///	@brief
struct OSRE_EXPORT Viewport {
    i32 m_x;
    i32 m_y;
    i32 m_w;
    i32 m_h;

    Viewport();
    Viewport(i32 x, i32 y, i32 w, i32 h);
    ~Viewport();
    bool operator==(const Viewport &rhs) const;
    bool operator!=(const Viewport &rhs) const;

    OSRE_NON_COPYABLE(Viewport)
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This template class is used to store vertex data in a cache.
//-------------------------------------------------------------------------------------------------
template <class T>
struct TVertexCache {
    using CacheType = ::cppcore::TArray<T>;
    CacheType m_cache;

    /// @brief 
    /// @param size 
    TVertexCache(size_t size) :
            m_cache(size) {
        // empty
    }

    /// @brief 
    TVertexCache() :
            m_cache() {
        // empty
    }

    ///	@brief
    ~TVertexCache() {
        // empty
    }

    /// @brief 
    void clear() {
        m_cache.clear();
    }

    /// @brief 
    /// @return 
    bool isEmpty() const {
        return m_cache.isEmpty();
    }

    /// @brief 
    /// @param newSize 
    void increaseSize(size_t newSize) {
        if (0 == newSize) {
            return;
        }
        m_cache.reserve(m_cache.size() + newSize);
    }

    /// @brief 
    /// @param vertices 
    /// @param numItems 
    void add(T *vertices, size_t numItems) {
        if (nullptr == vertices || 0 == numItems) {
            return;
        }
        m_cache.add(vertices, numItems);
    }

    /// @brief 
    /// @param vertex 
    void add(const T &vertex) {
        m_cache.add(vertex);
    }

    /// @brief 
    /// @return 
    size_t numVertices() const {
        return m_cache.size();
    }

    /// @brief 
    /// @return 
    size_t sizeInBytes() const {
        return m_cache.size() * sizeof(T);
    }
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This template class is used to store index data in a cache.
//-------------------------------------------------------------------------------------------------
template <class T>
struct TIndexCache {
    using CacheType = ::cppcore::TArray<T>;

    CacheType m_cache;

    /// @brief 
    /// @param size 
    TIndexCache(size_t size) :
            m_cache(size) {
        // empty
    }

    /// @brief 
    TIndexCache() :
            m_cache() {
        // empty
    }

    ///	@brief
    ~TIndexCache() {
        // empty
    }

    ///	@brief
    void clear() {
        m_cache.clear();
    }

    ///	@brief
    /// @return 
    bool isEmpty() const {
        return m_cache.isEmpty();
    }

    ///	@brief
    /// @param newSize 
    void increaseSize(size_t newSize) {
        m_cache.reserve(m_cache.size() + newSize);
    }

    ///	@brief
    /// @param index 
    void add(const T &index) {
        m_cache.add(index);
    }

    /// @brief 
    /// @param index 
    /// @param numItems 
    void add(T *index, size_t numItems) {
        if (nullptr == index || 0 == numItems) {
            return;
        }
        m_cache.add(index, numItems);
    }

    /// @brief 
    /// @return 
    size_t numIndices() const {
        return m_cache.size();
    }

    ///	@brief
    /// @return 
    size_t sizeInBytes() const {
        return m_cache.size() * sizeof(T);
    }
};

///	@brief
struct OSRE_EXPORT Light {
    glm::vec4 m_position;
    glm::vec3 m_specular;
    glm::vec3 m_diffuse;
    glm::vec3 m_ambient;
    glm::vec4 m_direction;
    f32 m_specularExp;
    f32 mRadius;
    LightType m_type;

    Light();
    ~Light();
};

using UiVertexCache = RenderBackend::TVertexCache<RenderBackend::RenderVert>;
using UiIndexCache = RenderBackend::TIndexCache<ui16>;

struct MatrixBuffer {
    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_proj;

    MatrixBuffer() :
            m_model(1.0f),
            m_view(1.0f),
            m_proj(1.0f) {
        // empty
    }
    ~MatrixBuffer() = default;
};

struct MeshEntry {
    ui32 numInstances;
    bool m_isDirty;
    cppcore::TArray<Mesh*> mMeshArray;

    MeshEntry() = default;
    ~MeshEntry() = default;
};

struct RenderBatchData {
    enum DirtyMode {
        MatrixBufferDirty = 1,
        UniformBufferDirty = 2,
        MeshDirty = 4,
        MeshUpdateDirty = 8
    };

    const c8 *m_id;
    MatrixBuffer m_matrixBuffer;
    cppcore::TArray<UniformVar *> m_uniforms;
    cppcore::TArray<MeshEntry *> m_meshArray;
    cppcore::TArray<Mesh *> m_updateMeshArray;
    ui32 m_dirtyFlag;

    RenderBatchData(const c8 *id) :
            m_id(id),
            m_matrixBuffer(),
            m_uniforms(),
            m_meshArray(),
            m_updateMeshArray(),
            m_dirtyFlag(0) {
        osre_assert(id != nullptr);
    }

    ~RenderBatchData() = default;

    MeshEntry *getMeshEntryByName(const c8 *name);
    
    UniformVar *getVarByName(const c8 *name);
};

struct PassData {
    const c8 *m_id;
    FrameBuffer *m_renderTarget;
    cppcore::TArray<RenderBatchData *> m_geoBatches;
    glm::mat4 mView;
    glm::mat4 mProj;
    bool m_isDirty;

    PassData(const c8 *id, FrameBuffer *fb) :
            m_id(id),
            m_renderTarget(fb),
            m_geoBatches(),
            mView(1),
            mProj(1),
            m_isDirty(true) {
        // empty
    }

    ~PassData() = default;

    RenderBatchData *getBatchById(const c8 *id) const;
};

struct OSRE_EXPORT UniformDataBlob {
    void *m_data;
    size_t m_size;

    UniformDataBlob();
    ~UniformDataBlob();
    void *getData() const;
    void clear();

    static UniformDataBlob *create(ParameterType type, size_t arraySize);
};

struct OSRE_EXPORT UniformVar {
    String m_name;
    ParameterType m_type;
    ui32 m_numItems;
    UniformDataBlob m_data;
    UniformVar *m_next;

    static ui32 getParamDataSize(ParameterType type, ui32 arraySize);
    static UniformVar *create(const String &name, ParameterType type, ui32 arraySize = 1);
    static void destroy(UniformVar *param);

    size_t getSize();

private:
    UniformVar();
    ~UniformVar();
};

struct FrameSubmitCmd {
    enum Type {
        CreatePasses = 1,
        UpdateBuffer = 2,
        UpdateMatrixes = 4,
        UpdateUniforms = 8,
        AddRenderData = 16
    };

    guid m_meshId;
    const c8 *m_passId;
    const c8 *m_batchId;
    ui32 m_updateFlags;
    size_t m_size;
    c8 *m_data;
    ::cppcore::TArray<MeshEntry*> m_newMeshes;
    ::cppcore::TArray<PassData*> m_updatedPasses;

    FrameSubmitCmd() :
            m_meshId(999999),
            m_passId(nullptr),
            m_batchId(nullptr),
            m_updateFlags(0),
            m_size(0),
            m_data(nullptr),
            m_newMeshes() {
        // empty
    }

    ~FrameSubmitCmd() = default;
};

using FrameSubmitCmdAllocator = ::cppcore::TPoolAllocator<FrameSubmitCmd>;

struct UniformBuffer {
    UniformBuffer() :
            m_numvars(0),
            m_pos(0L),
            m_buffer() {
        // empty
    }

    ~UniformBuffer() = default;

    size_t getSize() const {
        return m_buffer.size();
    }

    void create(size_t size = 1024 * 1024) {
        m_buffer.resize(size);
        m_pos = 0;
    }

    void destroy() {
        m_buffer.clear();
        m_pos = 0;
    }

    void reset() {
        m_pos = 0;
    }

    static ui32 encode(ui16 nameLen, ui16 dataLen) {
        const ui32 nl_encoded = nameLen << 16;
        const ui32 dl_encoded = dataLen << 0;

        return nl_encoded | dl_encoded;
    }

    static void decode(ui32 info, ui16 &nameLen, ui16 &dataLen) {
        const ui32 i = info >> 16;
        nameLen = (ui16)i;
        dataLen = (ui16)info >> 0;
    }

    void writeVar(UniformVar *var) {
        if (nullptr == var) {
            return;
        }

        ++m_numvars;
        ui32 varInfo = encode((ui16)var->m_name.size(), (ui16)var->m_data.m_size);
        write((c8 *)&varInfo, sizeof(ui32));
        write((c8 *)var->m_data.getData(), var->m_data.m_size);
    }

    void readVar(c8 *id, size_t &size, c8 *data) {
        ui32 varInfo = 0;
        read((c8 *)&varInfo, sizeof(ui32));
        ui16 nameLen(0), dataLen(0);
        decode(varInfo, nameLen, dataLen);
        read(id, nameLen);
        size = dataLen;
        read(data, dataLen);
    }

    void read(c8 *data, size_t size) {
        if ((m_pos + size) > m_buffer.size() || 0 == size) {
            return;
        }

        ::memcpy(data, &m_buffer[m_pos], size);
        m_pos += size;
    }

    void write(c8 *data, size_t size) {
        if (0 == size || (m_pos + size) > m_buffer.size()) {
            return;
        }

        ::memcpy(&m_buffer[m_pos], data, size);
        m_pos += size;
    }

    size_t m_numvars;
    size_t m_pos;
    MemoryBuffer m_buffer;
};

struct Frame {
    cppcore::TArray<PassData *> m_newPasses;
    cppcore::TArray<FrameSubmitCmd*> m_submitCmds;
    FrameSubmitCmdAllocator m_submitCmdAllocator;
    UniformBuffer *m_uniforBuffers;
    Pipeline *m_pipeline;

    Frame();
    ~Frame();
    void init(::cppcore::TArray<PassData *> &newPasses);
    FrameSubmitCmd *enqueue();

    Frame(const Frame &) = delete;
    Frame(Frame &&) = delete;
    Frame &operator=(const Frame &) = delete;
};

struct FrameBuffer {
    i32 m_width;
    i32 m_height;
    i32 m_depth;

    FrameBuffer(i32 w, i32 h, i32 d) :
            m_width(w), m_height(h), m_depth(d) {
        // empty
    }

    ~FrameBuffer() = default;

    FrameBuffer(const FrameBuffer &) = delete;
    FrameBuffer(FrameBuffer &&) = delete;
    FrameBuffer &operator=(const FrameBuffer &) = delete;
};

enum RenderPassType : ui32 {
    StaticRenderPass,
    UiRenderPass
};

struct RenderTarget {
    // empty
};

} // Namespace RenderBackend
} // Namespace OSRE
