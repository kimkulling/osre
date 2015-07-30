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
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace RenderBackend {

VertComponent VertexLayout::ErrorComp;

VertComponent::VertComponent()
: m_attrib( InvalidVertexAttr )
, m_format( InvalidVertexFormat ) {
    // empty

}

VertComponent::VertComponent( VertexAttribute attrib, VertexFormat format )
    : m_attrib( attrib )
    , m_format( InvalidVertexFormat ) {
    // empty
}

VertComponent::~VertComponent() {
    // empty
}

VertexLayout::VertexLayout()
    : m_components()
    , m_offsets()
    , m_currentOffset( 0 ) {
    // empty
}

VertexLayout::~VertexLayout() {
    // empty
}

void VertexLayout::clear() {
    if( !m_components.isEmpty() ) {
        for( ui32 i = 0; i < m_components.size(); ++i ) {
            delete m_components[ i ];
        }
        m_components.clear();
    }

    m_offsets.clear();
    m_currentOffset = 0;
}

ui32 VertexLayout::size() const {
    return m_components.size();
}

VertexLayout &VertexLayout::add( VertComponent *comp ) {
    if( nullptr == comp ) {
        return *this;
    }
    m_components.add( comp );
    const ui32 offset( getVertexFormatSize( comp->m_format ) );
    m_offsets.add( m_currentOffset );
    m_currentOffset += offset;

    return *this;
}

VertComponent &VertexLayout::getAt( ui32 idx ) const {
    if( idx >= m_components.size() ) {
        return ErrorComp;
    }

    return *m_components[ idx ];
}

BufferData::BufferData()
: m_type( EmptyBuffer )
, m_pData( nullptr )
, m_size( 0 )
, m_access( ReadOnly ) {
    // empty
}

BufferData::~BufferData() {
    delete[] m_pData;
    m_pData = nullptr;
    m_size = 0;
}

BufferData *BufferData::alloc( BufferType type, ui32 m_size, BufferAccessType access ) {
    BufferData *buffer( new BufferData );
    buffer->m_size = m_size;
    buffer->m_access = access;
    buffer->m_type = type;
    buffer->m_pData = new uc8[ buffer->m_size ];

    return buffer;
}

PrimitiveGroup::PrimitiveGroup()
: m_primitive( LineList )
, m_startIndex( 0 )
, m_numPrimitives( 0 )
, m_indexType( UnsignedShort ) {
    // empty
}

PrimitiveGroup::~PrimitiveGroup() {
    // empty
}


Texture::Texture()
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

Texture::~Texture() {
    delete[] m_pData;
    m_pData = nullptr;
}

Shader::Shader() {
    // empty
}

Shader::~Shader() {
    // empty
}

Material::Material()
    : m_type( FlatShadingMaterial )
    , m_numTextures( 0 )
    , m_pTextures( nullptr )
    , m_pShader( nullptr ) {
    // empty
}

Material::~Material() {
    m_pShader = nullptr;

    delete[] m_pTextures;
    m_pTextures = nullptr;
}

Transform::Transform() {
    for( ui32 i = 0; i < 3; ++i ) {
        m_translate[ i ] = 0.0f;
        m_scale[ i ] = 1.0f;
    }
}

Transform::~Transform() {
    // empty
}

Geometry::Geometry()
    : m_material( nullptr )
    , m_numParameter( 0 )
    , m_parameter( nullptr )
    , m_vb( nullptr )
    , m_ib( nullptr )
    , m_numPrimGroups( 0 )
    , m_pPrimGroups( nullptr ) {
    // empty
}

Geometry::~Geometry() {
    delete m_material;
    m_material = nullptr;

    delete m_vb;
    m_vb = nullptr;

    delete m_ib;
    m_ib = nullptr;

    delete[] m_pPrimGroups;
    m_pPrimGroups = nullptr;
}

GeoInstanceData::GeoInstanceData()
    : m_data( nullptr ) {
    // empty
}

GeoInstanceData::~GeoInstanceData() {
    delete m_data;
    m_data = nullptr;
}

TransformBlock::TransformBlock() {
    // empty
}

TransformBlock::~TransformBlock() {
    // empty
}

TransformMatrixBlock::TransformMatrixBlock()
    : m_projection()
    , m_model()
    , m_view() {
    // empty
}

TransformMatrixBlock::~TransformMatrixBlock() {
    // empty
}

void TransformMatrixBlock::init() {
    m_projection = glm::mat4( 1.0f );
    m_model = glm::mat4( 1.0f );
    m_view = glm::mat4( 1.0f );
}


} // Namespace RenderBackend
} // Namespace OSRE
