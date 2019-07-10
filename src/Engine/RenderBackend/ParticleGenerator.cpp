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
#include <osre/RenderBackend/ParticleGenerator.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/MaterialBuilder.h>

#include <cppcore/Random/RandomGenerator.h>

namespace OSRE {
namespace RenderBackend {

ParticleGenerator::ParticleGenerator( RenderBackendService *rbSrv )
: m_rbSrv( rbSrv )
, m_numPoints( 0 )
, m_col( nullptr )
, m_pos( nullptr )
, m_pt_indices( nullptr )
, m_ptGeo( nullptr ) {
    // empty
}

ParticleGenerator::~ParticleGenerator() {
    delete[] m_col;
    delete[] m_pos;
}

void ParticleGenerator::init( ui32 numPoints ) {
    m_numPoints = numPoints;
    m_col = new glm::vec3[ numPoints ];
    m_pos = new glm::vec3[ numPoints ];

    CPPCore::RandomGenerator generator;
    for ( ui32 i = 0; i < m_numPoints; i++ ) {
        const f32 r = static_cast< f32 >( generator.get( 1, 100 ) ) / 100.0f;
        const f32 g = static_cast< f32 >( generator.get( 1, 100 ) ) / 100.0f;
        const f32 b = static_cast< f32 >( generator.get( 1, 100 ) ) / 100.0f;
        m_col[ i ] = glm::vec3( r, g, b );

        const f32 x = static_cast< f32 >( generator.get( 0, 400 ) - 200 ) / 100.0f;
        const f32 y = static_cast< f32 >( generator.get( 0, 400 ) - 200 ) / 100.0f;
        const f32 z = static_cast< f32 >( generator.get( 0, 400 ) - 200 ) / 100.0f;
        m_pos[ i ] = glm::vec3( x, y, z );
    }

    m_pt_indices = new GLushort[ m_numPoints ];
    for ( ui32 i = 0; i < m_numPoints; i++ ) {
        m_pt_indices[ i ] = static_cast< GLushort >( i );
    }

    Scene::MeshBuilder meshBuilder;
    meshBuilder.allocEmptyMesh(VertexType::ColorVertex, 1 );
    m_ptGeo = meshBuilder.getMesh();
    m_rbSrv->addMesh( m_ptGeo, 0 );
    m_ptGeo->m_vb = Scene::MeshBuilder::allocVertices( VertexType::ColorVertex, m_numPoints, m_pos, m_col, nullptr, BufferAccessType::ReadOnly );
    m_ptGeo->m_indextype = IndexType::UnsignedShort;
    ui32 pt_size = sizeof( GLushort ) * m_numPoints;
    m_ptGeo->m_ib = BufferData::alloc( BufferType::IndexBuffer, pt_size, BufferAccessType::ReadOnly );
    m_ptGeo->m_ib->copyFrom( m_pt_indices, pt_size );

    // setup primitives
    m_ptGeo->m_numPrimGroups = 1;
    m_ptGeo->m_primGroups = new PrimitiveGroup[ m_ptGeo->m_numPrimGroups ];
    m_ptGeo->m_primGroups[ 0 ].init( IndexType::UnsignedShort, m_numPoints, PrimitiveType::PointList, 0 );

    m_ptGeo->m_localMatrix = true;
    m_ptGeo->m_model = glm::mat4(1.0f);

    // setup material
    Material *mat = Scene::MaterialBuilder::createBuildinMaterial( VertexType::ColorVertex );
    m_ptGeo->m_material = mat;
}

void ParticleGenerator::update( d32 tick ) {
    CPPCore::RandomGenerator generator;
    for ( ui32 i = 0; i < m_numPoints; i++ ) {
        const f32 x = static_cast< f32 >( generator.get( -10, 10 ) ) / 100.0f;
        const f32 y = static_cast< f32 >( generator.get( -10, 10 ) ) / 100.0f;
        const f32 z = static_cast< f32 >( generator.get( -10, 10 ) ) / 100.0f;
        m_pos[ i ] += glm::vec3( x, y, z );
    }

    ui32 offset( 0 );
    for ( ui32 i = 0; i < m_numPoints; i++ ) {
        uc8 *ptr = ( uc8* )m_ptGeo->m_vb->getData();
        ::memcpy( &ptr[ offset ], &m_pos[ i ], sizeof( glm::vec3 ) );
        offset += sizeof( ColorVert );
    }
}

Mesh* ParticleGenerator::getMesh() const {
    return m_ptGeo;
}

} // Namespace RenderBackend
} // Namespace OSRE
