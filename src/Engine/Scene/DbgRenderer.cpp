/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Scene/DbgRenderer.h>
#include <osre/RenderBackend/Pipeline.h>
#include <osre/Scene/MaterialBuilder.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/UI/Widget.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::RenderBackend;

DbgRenderer *DbgRenderer::s_instance = nullptr;

DbgRenderer::DbgRenderer( RenderBackend::RenderBackendService *rbSrv )
: m_rbSrv( rbSrv )
, m_textBoxes()
, m_tbArray()
, m_debugMesh( nullptr )
, m_lastIndex( 0 ) {
    OSRE_ASSERT( nullptr != m_rbSrv );
}

DbgRenderer::~DbgRenderer() {
    clearDbgTextCache();
    Mesh::destroy(&m_debugMesh);
}

bool DbgRenderer::create( RenderBackend::RenderBackendService *rbSrv ) {
    if ( nullptr != s_instance ) {
        return false;
    }

    s_instance = new DbgRenderer( rbSrv );
    return true;
}

bool DbgRenderer::destroy() {
    if ( nullptr == s_instance ) {
        return false;
    }
    delete s_instance;
    s_instance = nullptr;
    return true;
}

DbgRenderer *DbgRenderer::getInstance() {
    return s_instance;
}

static void insertTextEntry( ui32 id, Mesh *geo, const String &text, DbgRenderer::TextBoxHashMap &textBoxes ) {
    DbgRenderer::DbgTextEntry *entry( new DbgRenderer::DbgTextEntry );
    entry->m_geo = geo;
    entry->m_text = text;
    textBoxes.insert( id, entry );    
}

void DbgRenderer::renderDbgText( ui32 x, ui32 y, ui32 id, const String &text ) {
    if ( text.empty() ) {
        return;
    }

    f32 scale = 0.2f;
    glm::vec3 translate(0.f, 0.f, 0);
    m_transformMatrix.init();
    m_transformMatrix.m_model = glm::translate(m_transformMatrix.m_model, translate);
    m_transformMatrix.m_model = glm::scale(m_transformMatrix.m_model, glm::vec3(scale, scale, scale));
    m_transformMatrix.update();

    m_rbSrv->beginPass( PipelinePass::getPassNameById( DbgPassId ) );
    m_rbSrv->beginRenderBatch("dbgFontBatch");
    
    f32 xTrans(0), yTrans(0);
    UI::WidgetCoordMapping::mapPosToWorld(x , y, xTrans, yTrans);
    if ( !m_textBoxes.hasKey( id ) ) {
        MeshBuilder geoBuilder;
        geoBuilder.allocTextBox(xTrans, yTrans, scale, text, BufferAccessType::ReadWrite);
        Mesh *mesh = geoBuilder.getMesh();
        m_rbSrv->addMesh(mesh, 0 );
        insertTextEntry( id, mesh, text, m_textBoxes );
        mesh->m_localMatrix = true;
        mesh->m_model = m_transformMatrix.m_model;
    } else {
        DbgTextEntry *entry( nullptr );
        if ( m_textBoxes.getValue( id, entry ) ) {
            OSRE_ASSERT( nullptr != entry );
			if (entry->m_text != text) {
				Mesh* geo(nullptr);
				if (text.size() > entry->m_text.size()) {
					MeshBuilder geoBuilder;
                    geoBuilder.allocTextBox(xTrans, yTrans, 0.1f, text, BufferAccessType::ReadWrite);
					geo = geoBuilder.getMesh();
					entry->m_geo = geo;
				} else {
					MeshBuilder::updateTextBox(entry->m_geo, 0.1f, text);
					geo = entry->m_geo;
				}
				m_rbSrv->updateMesh(geo);
			}
        }
    }
    
    m_rbSrv->endRenderBatch();
    m_rbSrv->endPass();
}

static const ui32 NumIndices = 24;

static ui16 indices[ NumIndices ] = {
    0, 1,
    1, 2,
    2, 3,
    3, 0,
    4, 5,
    5, 6,
    6, 7,
    7, 4,
    0, 4,
    1, 5,
    2, 6,
    3, 7
};

void DbgRenderer::renderAABB( const glm::mat4 &transform, const TAABB<f32> &aabb ) {
    MeshBuilder meshBuilder;
    meshBuilder.allocEmptyMesh(VertexType::ColorVertex, 1);
    Mesh *mesh = meshBuilder.getMesh();

    static const ui32 NumVertices = 8;
    ColorVert vertices[ NumVertices ];
    const OSRE::Vec3f &min( aabb.getMin() );
    const OSRE::Vec3f &max( aabb.getMax() );
    f32 x0( min.getX() ), y0( min.getY() ), z0( min.getZ() ), x1( max.getX() ), y1( max.getY() ), z1 ( max.getZ() );
    vertices[ 0 ].position.x = x0;
    vertices[ 0 ].position.y = y0;
    vertices[ 0 ].position.z = z0;

    vertices[ 1 ].position.x = x1;
    vertices[ 1 ].position.y = y0;
    vertices[ 1 ].position.z = z0;

    vertices[ 2 ].position.x = x1;
    vertices[ 2 ].position.y = y1;
    vertices[ 2 ].position.z = z0;

    vertices[ 3 ].position.x = x0;
    vertices[ 3 ].position.y = y1;
    vertices[ 3 ].position.z = z0;

    vertices[ 4 ].position.x = x0;
    vertices[ 4 ].position.y = y0;
    vertices[ 4 ].position.z = z1;

    vertices[ 5 ].position.x = x1;
    vertices[ 5 ].position.y = y0;
    vertices[ 5 ].position.z = z1;

    vertices[ 6 ].position.x = x1;
    vertices[ 6 ].position.y = y1;
    vertices[ 6 ].position.z = z1;

    vertices[ 7 ].position.x = x0;
    vertices[ 7 ].position.y = y1;
    vertices[ 7 ].position.z = z1;

    const size_t vertexSize( sizeof( ColorVert )*NumVertices );
    mesh->m_vb = BufferData::alloc( BufferType::VertexBuffer, vertexSize, BufferAccessType::ReadOnly );
    mesh->m_vb->copyFrom( &vertices[ 0 ], vertexSize );
    const size_t indexSize( sizeof( ui16 )*NumIndices );
    mesh->m_ib = BufferData::alloc( BufferType::IndexBuffer, indexSize, BufferAccessType::ReadOnly );
    mesh->m_indextype = IndexType::UnsignedShort;
    mesh->m_ib->copyFrom( &indices[ 0 ], indexSize );

    // setup primitives
    mesh->m_model = transform;
    mesh->m_numPrimGroups = 1;
    
    mesh->m_primGroups = new PrimitiveGroup[ 1 ];
    mesh->m_primGroups[ 0 ].init( IndexType::UnsignedShort, NumIndices, PrimitiveType::LineList, 0 );

    // setup material
    mesh->m_material = MaterialBuilder::createBuildinMaterial( VertexType::ColorVertex );

    mesh->m_model = transform;

    m_rbSrv->beginPass( PipelinePass::getPassNameById( DbgPassId ) );
    m_rbSrv->beginRenderBatch("dbgFontBatch");

    m_rbSrv->setMatrix(MatrixType::Model, transform);
    m_rbSrv->addMesh( mesh, 0 );

    m_rbSrv->endRenderBatch();
    m_rbSrv->endPass();
}

void DbgRenderer::clearDbgTextCache() {
    if ( m_textBoxes.isEmpty() ) {
        return;
    }

    for ( ui32 i = 0; i < m_tbArray.size(); i++ ) {
        if ( nullptr != m_tbArray[ i ] ) {
            delete m_tbArray[ i ];
        }
    }
    m_textBoxes.clear();
    m_textBoxes.init(100);
    m_tbArray.resize( 0 );
    m_textBoxes.init( 100 );
}

void DbgRenderer::clearDbgRenderPass() {

}

size_t DbgRenderer::numDbgTexts() const {
    return m_tbArray.size();
}

void DbgRenderer::addLine( const ColorVert &v0, const ColorVert &v1 ) {
    if (nullptr == m_debugMesh) {
        m_debugMesh = Mesh::create(1);
    }
    
    ColorVert vertices[2];
    vertices[0] = v0;
    vertices[1] = v1;
    
    m_debugMesh->m_vb->attach(&vertices[0], sizeof(ColorVert) * 2);
    ui16 lineIndices[2];
    lineIndices[0] = m_lastIndex;
    m_lastIndex++;
    lineIndices[1] = m_lastIndex;
    m_lastIndex++;

    m_debugMesh->m_ib->attach(&lineIndices[0], sizeof(ui16) * 2);

    m_debugMesh->m_primGroups = new PrimitiveGroup[1];
    m_debugMesh->m_primGroups[0].init(IndexType::UnsignedShort, NumIndices, PrimitiveType::LineList, 0);
}

void DbgRenderer::addTriangle( const ColorVert &v0, const ColorVert &v1, const ColorVert &v2 ) {
}

} // Namespace Scene
} // namespace OSRE
