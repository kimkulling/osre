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
#include "AbstractRenderTest.h"
#include "RenderTestUtils.h"

#include <osre/Common/Logger.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Mesh.h>
#include <src/Engine/RenderBackend/OGLRenderer/OGLShader.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/MaterialBuilder.h>
#include <cppcore/Random/RandomGenerator.h>
#include <iostream>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Test
///
///	@brief
//-------------------------------------------------------------------------------------------------
class RenderBufferAccessTest : public AbstractRenderTest {
    TransformMatrixBlock m_transformMatrix;
    static const ui32 NumPts = 1000;
    glm::vec3 m_col[NumPts];
    glm::vec3 m_pos[NumPts];
    Mesh *m_pointMesh;

public:
    RenderBufferAccessTest()
        : AbstractRenderTest( "rendertest/RenderBufferAccessTest" ) {
        // empty
    }

    virtual ~RenderBufferAccessTest() {
        // empty
    }

    bool onCreate( RenderBackendService *rbSrv ) override {
        rbSrv->sendEvent( &OnAttachViewEvent, nullptr );

        CPPCore::RandomGenerator generator;
        for ( ui32 i = 0; i < NumPts; i++ ) {
            const f32 r = static_cast< f32 >( generator.get( 1, 100 ) ) / 100.0f;
            const f32 g = static_cast< f32 >( generator.get( 1, 100 ) ) / 100.0f;
            const f32 b = static_cast< f32 >( generator.get( 1, 100 ) ) / 100.0f;
            m_col[ i ] = glm::vec3( r, g, b );

            const f32 x = static_cast< f32 >( generator.get( 0, 400 )-200 ) / 100.0f;
            const f32 y = static_cast< f32 >( generator.get( 0, 400 )-200 ) / 100.0f;
            const f32 z = static_cast< f32 >( generator.get( 0, 400 )-200 ) / 100.0f;
            m_pos[ i ] = glm::vec3( x, y, z );
        }

        GLushort pt_indices[ NumPts ];
        for ( ui32 i = 0; i < NumPts; i++ ) {
            pt_indices[ i ] = static_cast<GLushort>( i );
        }

        Scene::MeshBuilder meshBuilder;
        meshBuilder.allocEmptyMesh( VertexType::ColorVertex, 1 );
        m_pointMesh = meshBuilder.getMesh();

        rbSrv->beginPass( PipelinePass::getPassNameById( RenderPassId ) );
        rbSrv->beginRenderBatch("particle");

        rbSrv->addMesh( m_pointMesh, 0 );
        m_pointMesh->m_vb = Scene::MeshBuilder::allocVertices( VertexType::ColorVertex, NumPts, m_pos, m_col, nullptr, BufferAccessType::ReadOnly );
        m_pointMesh->m_indextype = IndexType::UnsignedShort;
        ui32 pt_size = sizeof( GLushort ) * NumPts;
        m_pointMesh->m_ib = BufferData::alloc( BufferType::IndexBuffer, pt_size, BufferAccessType::ReadOnly );
        m_pointMesh->m_ib->copyFrom( pt_indices, pt_size );

        // setup primitives
        m_pointMesh->m_numPrimGroups = 1;
        m_pointMesh->m_primGroups = new PrimitiveGroup[ m_pointMesh->m_numPrimGroups ];
        m_pointMesh->m_primGroups[ 0 ].init( IndexType::UnsignedShort, NumPts, PrimitiveType::PointList, 0 );

        // setup material
        Material *mat = Scene::MaterialBuilder::createBuildinMaterial( VertexType::ColorVertex );
        m_pointMesh->m_material = mat;

        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
        m_transformMatrix.m_model = glm::scale( m_transformMatrix.m_model, glm::vec3( .5, .5, .5 ) );
        UniformVar *parameter = UniformVar::create( "MVP", ParameterType::PT_Mat4 );
        ::memcpy( parameter->m_data.m_data, glm::value_ptr( m_transformMatrix.m_projection*m_transformMatrix.m_view*m_transformMatrix.m_model ), sizeof( glm::mat4 ) );

        m_transformMatrix.update();
        rbSrv->setMatrix("MVP", m_transformMatrix.m_mvp);

        rbSrv->endRenderBatch();
        rbSrv->endPass();

        return true;
    }

    bool onRender( RenderBackend::RenderBackendService* ) override {
        CPPCore::RandomGenerator generator;
        for (ui32 i = 0; i < NumPts; i++) {
            const f32 x = static_cast<f32>(generator.get(-10, 10) ) / 100.0f;
            const f32 y = static_cast<f32>(generator.get(-10, 10) ) / 100.0f;
            const f32 z = static_cast<f32>(generator.get(-10, 10) ) / 100.0f;
            m_pos[ i ] += glm::vec3(x, y, z);
        }

        ui32 offset( 0 );
        for (ui32 i = 0; i < NumPts; i++) {
            uc8 *ptr = (uc8*) m_pointMesh->m_vb->getData();
            ::memcpy( &ptr[offset], &m_pos[ i ], sizeof(glm::vec3));
            offset += sizeof( ColorVert );
        }
        
        return true;
    }
};

ATTACH_RENDERTEST( RenderBufferAccessTest )

} // Namespace RenderTest
} // Namespace OSRE
