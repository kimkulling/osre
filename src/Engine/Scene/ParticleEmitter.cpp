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
#include <osre/Scene/ParticleEmitter.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/Scene/MeshBuilder.h>
#include <osre/Scene/MaterialBuilder.h>

#include <cppcore/Random/RandomGenerator.h>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::RenderBackend;

ParticleEmitter::ParticleEmitter( RenderBackendService *rbSrv )
: m_rbSrv( rbSrv )
, m_numPoints( 0 )
, m_col( nullptr )
, m_pos( nullptr )
, m_pt_indices( nullptr )
, m_ptGeo( nullptr )
, mUseBounds( false )
, mBounds() {
    // empty
}

ParticleEmitter::~ParticleEmitter() {
    delete[] m_col;
    delete[] m_pos;
}

void ParticleEmitter::init( ui32 numPoints ) {
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
    meshBuilder.allocEmptyMesh("", VertexType::ColorVertex);
    m_ptGeo = meshBuilder.getMesh();
    m_rbSrv->addMesh( m_ptGeo, 0 );
    Scene::MeshBuilder::allocVertices(m_ptGeo, VertexType::ColorVertex, m_numPoints, m_pos, m_col, nullptr, BufferAccessType::ReadOnly);
    ui32 pt_size = sizeof( GLushort ) * m_numPoints;
    m_ptGeo->createIndexBuffer(m_pt_indices, pt_size, IndexType::UnsignedShort, BufferAccessType::ReadOnly);

    // setup primitives
    m_ptGeo->addPrimitiveGroup(m_numPoints, PrimitiveType::PointList, 0);
    m_ptGeo->setModelMatrix(true, glm::mat4(1.0f));

    // setup material
    Material *mat = Scene::MaterialBuilder::createBuildinMaterial( VertexType::ColorVertex );
    m_ptGeo->setMaterial(mat);
}

void ParticleEmitter::update( d32 /*tick*/ ) {
    CPPCore::RandomGenerator generator;
    for ( ui32 i = 0; i < m_numPoints; i++ ) {
        const f32 x = static_cast< f32 >( generator.get( -10, 10 ) ) / 100.0f;
        const f32 y = static_cast< f32 >( generator.get( -10, 10 ) ) / 100.0f;
        const f32 z = static_cast< f32 >( generator.get( -10, 10 ) ) / 100.0f;
        m_pos[ i ] += glm::vec3( x, y, z );
        if (mUseBounds) {
            glm::vec3 pt(m_pos[i].x, m_pos[i].y, m_pos[i].z);
            if (!mBounds.isIn(pt)) {
                m_pos[i] -= m_pos[i];
                m_pos[i] -= m_pos[i];
            }
        }
    }

    ui32 offset( 0 );
    for ( ui32 i = 0; i < m_numPoints; i++ ) {
        uc8 *ptr = ( uc8* )m_ptGeo->getVertexBuffer()->getData();
        ::memcpy( &ptr[ offset ], &m_pos[ i ], sizeof( glm::vec3 ) );
        offset += sizeof( ColorVert );
    }
}

void ParticleEmitter::setBounds(const AABB& bounds) {
    mUseBounds = true;
    mBounds = bounds;
}

Mesh* ParticleEmitter::getMesh() const {
    return m_ptGeo;
}

} // Namespace RenderBackend
} // Namespace OSRE
