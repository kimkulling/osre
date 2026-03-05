/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2026 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "RenderBackend/MeshBuilder.h"
#include "RenderBackend/RenderCommon.h"
#include "Debugging/MeshDiagnostic.h"
#include "RenderBackend/MaterialBuilder.h"
#include "RenderBackend/Mesh.h"

#include <memory>

namespace OSRE::UnitTest {

using namespace ::OSRE::Debugging;
using namespace ::OSRE::RenderBackend;

class MeshBuilderTest final : public ::testing::Test {
protected:
    void SetUp() override {
        MaterialBuilder::create(GLSLVersion::GLSL_400);
    }

    void TearDown() override {
        MaterialBuilder::destroy();
    }
};

TEST_F( MeshBuilderTest, allocTrianglesTest ) {
    MeshBuilder meshBuilder;
    meshBuilder.createTriangle(VertexType::ColorVertex, BufferAccessType::ReadOnly);
    std::unique_ptr<Mesh> mesh(meshBuilder.getMesh());
    ASSERT_NE(mesh, nullptr);
    EXPECT_EQ(mesh->getVertexType(), VertexType::ColorVertex);
    EXPECT_NE(mesh->getVertexBuffer(), nullptr);
    EXPECT_NE(mesh->getIndexBuffer(), nullptr);
    EXPECT_NE(mesh->getMaterial(), nullptr);
}

TEST_F( MeshBuilderTest, allocLineListTest ) {
    constexpr ui32 numLines = 2;
    glm::vec3 pos[3] = {}, col[3] = {};
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

    ui32 indices[4] = {};
    indices[ 0 ]=0;
    indices[ 1 ]=1;
    indices[ 2 ]=1;
    indices[ 3 ]=2;

    MeshBuilder meshBuilder;
    meshBuilder.allocLineList(VertexType::ColorVertex, BufferAccessType::ReadOnly, numLines, pos, col, indices);
    std::unique_ptr<Mesh> mesh(meshBuilder.getMesh());
    EXPECT_NE( nullptr, mesh );    
}

TEST_F( MeshBuilderTest, allocPointsTest ) {
    constexpr ui32 numPoints = 3;
    glm::vec3 pos[3] = {}, col[3] = {};
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
    MeshBuilder meshBuilder;
    meshBuilder.allocPoints(VertexType::ColorVertex, BufferAccessType::ReadOnly, numPoints, pos, col);
    std::unique_ptr<Mesh> mesh(meshBuilder.getMesh());
    EXPECT_NE( nullptr, mesh );
}

TEST_F(MeshBuilderTest, createCubeTest) { 
    MeshBuilder meshBuilder;
    meshBuilder.createCube(VertexType::ColorVertex, 1.0f, BufferAccessType::ReadOnly);
    std::unique_ptr<Mesh> mesh(meshBuilder.getMesh());
    ASSERT_NE(mesh, nullptr);
    EXPECT_EQ(mesh->getVertexType(), VertexType::ColorVertex);
    EXPECT_NE(mesh->getVertexBuffer(), nullptr);
    const BufferData *indexBuffer = mesh->getIndexBuffer();
    ASSERT_NE(indexBuffer, nullptr);
    EXPECT_EQ(indexBuffer->getSize(), sizeof(ui16) * 36);
    const ui16 expectedIndices[] = {
        0, 1, 2, 0, 2, 3,
        4, 6, 5, 4, 7, 6,
        8, 9, 10, 8, 10, 11,
        12, 14, 13, 12, 15, 14,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };
    const ui16 *indices = reinterpret_cast<const ui16 *>(indexBuffer->getData());
    ASSERT_NE(indices, nullptr);
    for (size_t i = 0; i < sizeof(expectedIndices) / sizeof(expectedIndices[0]); ++i) {
        EXPECT_EQ(indices[i], expectedIndices[i]) << "Mismatch at index " << i;
    }
    EXPECT_EQ(mesh->getNumberOfPrimitiveGroups(), 1u);
}

class GeometryDiagnosticUtilsTest : public ::testing::Test {};

TEST_F( GeometryDiagnosticUtilsTest, dumpVerticesTest_invalidInput ) {
    EXPECT_NO_THROW(MeshDiagnostic::dumpVertices( nullptr, 1));
    
    cppcore::TArray<RenderVert> vertices;
    EXPECT_NO_THROW(MeshDiagnostic::dumpVertices(vertices));
}

} // Namespace OSRE::UnitTest

