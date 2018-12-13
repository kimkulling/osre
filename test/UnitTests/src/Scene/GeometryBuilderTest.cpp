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
#include "osre_testcommon.h"
#include <osre/Scene/GeometryBuilder.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Geometry.h>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::Scene;
using namespace ::OSRE::RenderBackend;

class GeometryBuilderTest : public ::testing::Test {
    // empty
};

TEST_F( GeometryBuilderTest, allocEmptyGeometryTest ) {
    GeometryBuilder geoBuilder;
    geoBuilder.allocEmptyGeometry(VertexType::ColorVertex, 2);
    Geometry *geoArray = geoBuilder.getGeometry();
    EXPECT_NE( geoArray, nullptr );

    for ( ui32 i = 0; i < 2; i++ ) {
        Geometry &currentGeo( geoArray[ i ] );
        EXPECT_EQ( currentGeo.m_vertextype, VertexType::ColorVertex );
    }
    Geometry::destroy( &geoArray );
}

TEST_F( GeometryBuilderTest, allocTrianglesTest ) {
    GeometryBuilder geoBuilder;
    geoBuilder.allocTriangles(VertexType::ColorVertex, BufferAccessType::ReadOnly);
    Geometry *geo = geoBuilder.getGeometry();
    EXPECT_NE( geo, nullptr );
    EXPECT_EQ( geo->m_vertextype, VertexType::ColorVertex );
    EXPECT_NE( geo->m_vb, nullptr );
    EXPECT_NE( geo->m_ib, nullptr );
    EXPECT_NE( geo->m_material, nullptr );
    Geometry::destroy( &geo );
}

TEST_F( GeometryBuilderTest, allocLineListTest ) {
    const ui32 numLines = 2;
    glm::vec3 pos[ 3 ], col[3];
    pos[ 0 ].x = 0;
    pos[ 0 ].y = 0;
    pos[ 0 ].z = 0;

    pos[ 1 ].x = 1;
    pos[ 1 ].y = 0;
    pos[ 1 ].z = 0;

    pos[ 2 ].x = 2;
    pos[ 2 ].y = 0;
    pos[ 2 ].z = 0;

    col[ 0 ].x = 0;
    col[ 0 ].y = 0;
    col[ 0 ].z = 0;

    col[ 1 ].x = 0.5f;
    col[ 1 ].y = 0.5f;
    col[ 1 ].z = 0.5f;

    col[ 2 ].x = 0.8f;
    col[ 2 ].y = 0.8f;
    col[ 2 ].z = 0.8f;

    ui32 indices[ 4 ];
    indices[ 0 ]=0;
    indices[ 1 ]=1;
    indices[ 2 ]=1;
    indices[ 3 ]=2;

    Scene::GeometryBuilder geoBuilder;
    geoBuilder.allocLineList(VertexType::ColorVertex, BufferAccessType::ReadOnly, numLines, pos, col, indices);
    Geometry *geo = geoBuilder.getGeometry();
    EXPECT_NE( nullptr, geo );
    Geometry::destroy( &geo );
}

TEST_F( GeometryBuilderTest, allocPointsTest ) {
    const ui32 numPoints = 3;
    glm::vec3 pos[ 3 ], col[ 3 ];
    pos[ 0 ].x = 0;
    pos[ 0 ].y = 0;
    pos[ 0 ].z = 0;

    pos[ 1 ].x = 1;
    pos[ 1 ].y = 0;
    pos[ 1 ].z = 0;

    pos[ 2 ].x = 2;
    pos[ 2 ].y = 0;
    pos[ 2 ].z = 0;

    col[ 0 ].x = 0;
    col[ 0 ].y = 0;
    col[ 0 ].z = 0;

    col[ 1 ].x = 0.5f;
    col[ 1 ].y = 0.5f;
    col[ 1 ].z = 0.5f;

    col[ 2 ].x = 0.8f;
    col[ 2 ].y = 0.8f;
    col[ 2 ].z = 0.8f;
    Scene::GeometryBuilder geoBuilder;
    geoBuilder.allocPoints(VertexType::ColorVertex, BufferAccessType::ReadOnly, numPoints, pos, col);
    Geometry *geo = geoBuilder.getGeometry();
    EXPECT_NE( nullptr, geo );
    Geometry::destroy( &geo );
}

class GeometryDiagnosticUtilsTest : public ::testing::Test {
    // empty
};

TEST_F( GeometryDiagnosticUtilsTest, dumpVerticesTest_invalidInput ) {
    EXPECT_NO_THROW( GeometryDiagnosticUtils::dumpVertices( nullptr, 1 ) );
    
    CPPCore::TArray<RenderVert> vertices;
    EXPECT_NO_THROW( GeometryDiagnosticUtils::dumpVertices( vertices ) );
}

} // Namespace UnitTest
} // Namespace OSRE
