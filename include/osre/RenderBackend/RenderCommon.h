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

#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/Parameter.h>

#include <cppcore/Container/TArray.h>

#include <glm/glm.hpp>

namespace OSRE {
namespace RenderBackend {

struct Parameter;

static const i32 UnsetHandle = -1;

///	@brief  This enum describes the usage of a buffer object.
enum BufferType {
    EmptyBuffer,    ///< Empty buffer, no special use.
    VertexBuffer,   ///< Vertex buffer, stores vertex data inside.
    IndexBuffer,    ///< Index buffer, stores indices inside.
    InstanceBuffer  ///< Instance buffer, will store instance-specific data.
};

/// @brief  This enum describes the supported access types for render buffers.
enum BufferAccessType {
    ReadOnly,       ///< Read only access.
    WriteOnly,      ///< Write only access.
    ReadWrite       ///< Read and write access.
};

enum RenderCommandType {
    NoOpCmd,
    RenderView3DCmd
};

enum VertexType {
    ColorVertex,
    RenderVertex
};

enum TextureTargetType {
    Texture1D,
    Texture2D,
    Texture3D
};

enum TextureStageType {
    TextureStage0,
    TextureStage1,
    TextureStage2,
    TextureStage3
};

enum TextureParameterName {
    TextureParamMinFilter = 0,
    TextureParamMagFilter,
    TextureParamWrapS,
    TextureParamWrapT
};

enum TextureParameterType {
    TexturePTNearest = 0,
    TexturePTLinear,
    TexturePTClamp,
    TexturePTMirroredRepeat,
    TexturePTRepeat
};

///	@brief
enum IndexType {
    UnsignedByte,   ///<
    UnsignedShort   ///<
};

///	@brief
enum PrimitiveType {
    PointList,		///< A list of points, one index per point.
    LineList,		///< A list of separate lines, 2 indices per line.
    LineStrip,		///< A line strip, Start and end-index and all indices between.
    TriangleList,	///< A list of triangles, 3 indices per triangle.
    TriangelStrip,	///< A strip of triangles
    TriangleFan		///< A triangle fan.
};

enum ClearBitType {
    ColorBit    = 1 << 0,
    DepthBit    = 1 << 1,
    StencilBit  = 1 << 2
};

struct ColorVert {
    glm::vec3 position;
    glm::vec3 color;
};

struct RenderVert {
    glm::vec3 position;
    glm::vec2 tex0;
};

enum VertexAttribute {
    Position = 0,   ///> "position"
    Normal,         ///> "normal"
    TexCoord0,      ///> "texcoord0"
    TexCoord1,      ///> "texcoord1"
    TexCoord2,      ///> "texcoord2"
    TexCoord3,      ///> "texcoord3"
    Tangent,        ///> "tangent
    Binormal,       ///> "binormal"
    Weights,        ///> "weights" (skin weights)
    Indices,        ///> "indices" (skin indices)
    Color0,         ///> "color0"
    Color1,         ///> "color1"
    Instance0,      ///> "instance0"
    Instance1,      ///> "instance1"
    Instance2,      ///> "instance2"
    Instance3,      ///> "instance3"
    NumVertexAttrs,
    InvalidVertexAttr
};

enum VertexFormat {
    Float,          ///< single component float, expanded to (x, 0, 0, 1)
    Float2,         ///< 2-component float, expanded to (x, y, 0, 1)
    Float3,         ///< 3-component float, expanded to (x, y, z, 1)
    Float4,         ///< 4-component float
    Byte4,          ///< 4-component float (-128.0f..+127.0f) mapped to byte (-128..+127)
    UByte4,         ///< 4-component float (0.0f..255.0f) mapped to byte (0..255)
    Short2,         ///< 2-component float (-32768.0f..+32767.0f) mapped to short (-32768..+32768)
    Short4,         ///< 4-component float (-32768.0f..+32767.0f) mapped to short (-32768..+32768)
    NumVertexFormats,       
    InvalidVertexFormat, 
};

inline 
ui32 getVertexFormatSize( VertexFormat format ) {
    ui32 size( 0 );
    switch( format ) {
        case Float:
            size = sizeof( f32 );
            break;
        case Float2:
            size = sizeof( f32 )*2;
            break;
        case Float3:
            size = sizeof( f32 )*3;
            break;
        case Float4:
            size = sizeof( f32 )*4;
            break;
        case Byte4:
            size = sizeof( c8 ) * 4;
            break;
        case UByte4:
            size = sizeof( uc8 ) * 4;
            break;
        case Short2:
            size = sizeof( ui16 ) * 2;
            break;
        case Short4:
            size = sizeof( ui16 ) * 4;
            break;
        case NumVertexFormats:
        case InvalidVertexFormat:
            break;
    }

    return size;
}

struct OSRE_EXPORT VertComponent {
    VertexAttribute m_attrib;
    VertexFormat    m_format;

    VertComponent()
    : m_attrib( InvalidVertexAttr )
    , m_format( InvalidVertexFormat ) {
        // empty

    }
    
    VertComponent( VertexAttribute attrib, VertexFormat format );
    ~VertComponent();
};

struct OSRE_EXPORT VertexLayout {
    static VertComponent           ErrorComp;
    CPPCore::TArray<VertComponent*> m_components;
    CPPCore::TArray<ui32>          m_offsets;
    ui32                           m_currentOffset;

    VertexLayout();     
    ~VertexLayout();
    void clear();
    VertexLayout &add( VertComponent *comp );
    VertComponent &getAt( ui32 idx ) const;
};

struct BufferData {
    BufferType       m_type;
    void            *m_pData;
    ui32             m_size;
    BufferAccessType m_access;

    BufferData()
    : m_type( EmptyBuffer )
    , m_pData( nullptr )
    , m_size( 0 )
    , m_access( ReadOnly ) {
        // empty
    }

    ~BufferData() {
        delete [] m_pData;
        m_pData = nullptr;
        m_size = 0;
    }

    static BufferData *alloc( BufferType type, ui32 m_size, BufferAccessType access ) {
        BufferData *buffer( new BufferData );
        buffer->m_size = m_size;
        buffer->m_access = access;
        buffer->m_type = type;
        buffer->m_pData = new uc8[ buffer->m_size ];

        return buffer;
    }
};

struct PrimitiveGroup {
    PrimitiveType m_primitive;
    ui32          m_startIndex;
    ui32          m_numPrimitives;
    IndexType     m_indexType;

    PrimitiveGroup()
    : m_primitive( LineList )
    , m_startIndex( 0 )
    , m_numPrimitives( 0 )
    , m_indexType( UnsignedShort ) {
        // empty
    }

    ~PrimitiveGroup() {
        // empty
    }

    OSRE_NON_COPYABLE( PrimitiveGroup )
};


enum MaterialType {
    FlatShadingMaterial,
    ShaderMaterial
};

struct Texture {
    String            m_textureName;
    TextureTargetType m_targetType;
    ui32              m_size;
    uc8              *m_pData;
    ui32              m_width;
    ui32              m_height;
    ui32              m_channels;
    Handle            m_texHandle;

    Texture()
    : m_textureName( "" )
    , m_targetType( Texture2D )
    , m_size( 0 )
    , m_pData( nullptr )
    , m_width( 0 )
    , m_height( 0 )
    , m_channels( 0 )
    , m_texHandle() {
        // empty
    }

    ~Texture() {
        delete[] m_pData;
        m_pData = nullptr;
    }

    OSRE_NON_COPYABLE( Texture )
};

enum ShaderType {
    SH_VertexShaderType = 0,
    SH_FragmentShaderType,
    SH_GeometryShaderType
};

static const ui32 MaxShaderTypes = 3;

struct Shader {
    CPPCore::TArray<String> m_parameters;
    CPPCore::TArray<String> m_attributes;
    String                  m_src[ MaxShaderTypes ];

    Shader() {
        // empty
    }

    ~Shader() {
        // empty
    }

    OSRE_NON_COPYABLE( Shader )
};

struct Material {
    MaterialType m_type;
    ui32         m_numTextures;
    Texture     *m_pTextures;
    Shader      *m_pShader;

    Material()
    : m_type( FlatShadingMaterial )
    , m_numTextures( 0 )
    , m_pTextures( nullptr )
    , m_pShader( nullptr ) {
        // empty
    }

    ~Material() {
        m_pShader = nullptr;

        delete [] m_pTextures;
        m_pTextures = nullptr;
    }

    OSRE_NON_COPYABLE( Material )
};

struct Transform {
    f32 m_translate[ 3 ];
    f32 m_scale[ 3 ];

    Transform() {
        for( ui32 i = 0; i < 3; ++i ) {
            m_translate[ i ] = 0.0f;
            m_scale[ i ] = 1.0f;
        }
    }

    ~Transform() {
        // empty
    }

    OSRE_NON_COPYABLE( Transform )
};

struct Geometry {
    Material       *m_material;
    ui32            m_numParameter;
    Parameter      *m_parameter;
    VertexType      m_vertextype;
    BufferData     *m_vb;
    IndexType       m_indextype;
    BufferData     *m_ib;
    ui32            m_numPrimGroups;
    PrimitiveGroup *m_pPrimGroups;

    Geometry()
    : m_material( nullptr )
    , m_numParameter( 0 )
    , m_parameter( nullptr )
    , m_vb( nullptr )
    , m_ib( nullptr )
    , m_numPrimGroups( 0 )
    , m_pPrimGroups( nullptr ) {
        // empty
    }

    ~Geometry() {
        delete m_material;
        m_material = nullptr;

        delete m_vb;
        m_vb = nullptr;

        delete m_ib;
        m_ib = nullptr;
        
        delete [] m_pPrimGroups;
        m_pPrimGroups = nullptr;
    }

    OSRE_NON_COPYABLE( Geometry )
};

struct GeoInstanceData {
    BufferData *m_data;

    GeoInstanceData()
        : m_data( nullptr ) {
        // empty
    }

    ~GeoInstanceData() {
        delete m_data;
        m_data = nullptr;
    }

    OSRE_NON_COPYABLE( GeoInstanceData )
};

struct RenderCommand {
    RenderCommandType m_commandType;
    void             *m_pImpl;

    OSRE_NON_COPYABLE( RenderCommand )
};


struct TransformBlock {
    glm::vec4 m_transform;
    glm::vec4 m_scale;
    glm::vec4 m_rotation;

    TransformBlock() {
        // empty
    }

    ~TransformBlock() {
        // empty
    }

    OSRE_NON_COPYABLE( TransformBlock )
};

struct TransformMatrixBlock {
    glm::mat4 m_projection;
    glm::mat4 m_model;
    glm::mat4 m_view;

    TransformMatrixBlock() 
    : m_projection()
    , m_model()
    , m_view() {
        // empty
    }

    ~TransformMatrixBlock() {
        // empty
    }

    void init() {
        m_projection = glm::mat4( 1.0f );
        m_model      = glm::mat4( 1.0f );
        m_view       = glm::mat4( 1.0f );
    }

    OSRE_NON_COPYABLE( TransformMatrixBlock )
};

} // Namespace RenderBackend
} // Namespace OSRE
