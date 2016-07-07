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
#include <gtest/gtest.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <glm/gtc/matrix_transform.hpp>

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::RenderBackend;

class RenderCommonTest : public ::testing::Test {
    // empty
};

TEST_F( RenderCommonTest, createVertComponentTest ) {
    bool ok( true );
    try {
        VertComponent comp1;
        VertComponent comp2( Position, Float3 );
        EXPECT_EQ( Position, comp2.m_attrib );
        EXPECT_EQ( Float3,   comp2.m_format );
    }
    catch( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );

}

TEST_F( RenderCommonTest, createVertexLayoutTest ) {
    bool ok( true );
    try {
        VertexLayout layout;
    } catch( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( RenderCommonTest, addCompVertexLayoutTest ) {
    VertComponent *comp = new VertComponent;
    VertexLayout layout;
    EXPECT_EQ( 0, layout.numComponents() );
    layout.add( comp );
    EXPECT_EQ( 1, layout.numComponents() );
}

TEST_F( RenderCommonTest, clearCompVertexLayoutTest ) {
    VertComponent *comp = new VertComponent;
    VertexLayout layout;
    layout.add( comp );
    EXPECT_EQ( 1, layout.numComponents() );
    layout.clear();
    EXPECT_EQ( 0, layout.numComponents() );
}

TEST_F( RenderCommonTest, sizeInBytesTest ) {
    VertexLayout layout;
    ui32 size( layout.sizeInBytes() );
    EXPECT_EQ( 0, size );

    VertComponent *comp1 = new VertComponent;
    layout.add( comp1 );
    size = layout.sizeInBytes();
    EXPECT_EQ( 0, size );

    layout.clear();
    size = layout.sizeInBytes();
    EXPECT_EQ( 0, size );

    VertComponent *comp2 = new VertComponent( Position, Float3 );
    layout.add( comp2 );
    size = layout.sizeInBytes();
    EXPECT_EQ( 12, size );
}

TEST_F( RenderCommonTest, getVertCompNameTest ) {
    String name;
    name = getVertCompName( Position );
    EXPECT_NE( static_cast<ui32>( 0 ), name.size() );

    name = getVertCompName( InvalidVertexAttr );
    EXPECT_NE( static_cast<ui32>( 0 ), name.size() );
    EXPECT_EQ( "Error", name );
}

TEST_F(RenderCommonTest, viewportTest) {
	Viewport vp;
	EXPECT_EQ( -1, vp.m_x );
	EXPECT_EQ( -1, vp.m_y );
	EXPECT_EQ( -1, vp.m_w );
	EXPECT_EQ( -1, vp.m_h );

	Viewport vp_set( 1, 2, 3, 4 );
	EXPECT_EQ( 1, vp_set.m_x );
	EXPECT_EQ( 2, vp_set.m_y );
	EXPECT_EQ( 3, vp_set.m_w );
	EXPECT_EQ( 4, vp_set.m_h );

	Viewport vp_set2(1, 2, 3, 4);
	EXPECT_EQ( vp_set, vp_set2 );
}

TEST_F( RenderCommonTest, allocBufferDataTest ) {
    BufferData *data( BufferData::alloc( VertexBuffer, 100, ReadWrite ) );
    EXPECT_NE( data, nullptr );
    EXPECT_EQ( data->m_access, ReadWrite );
    EXPECT_EQ( data->m_size, 100 );
    EXPECT_EQ( data->m_type, VertexBuffer );

    BufferData::free( data );
}

TEST_F( RenderCommonTest, copyBufferDataTest ) {
    BufferData *data( BufferData::alloc( VertexBuffer, 100, ReadWrite ) );

    static const ui32 size = 100;
    static const unsigned char Value = 9;
    void *buffer( new unsigned char[ size ] );
    ::memset( buffer, Value, size );
    data->copyFrom( buffer, size );
    BufferData::free( data );
}

TEST_F( RenderCommonTest, accessStaticGeometryTest ) {
    StaticGeometry *geo( nullptr );
    geo = StaticGeometry::create( 0 );
    EXPECT_EQ( geo, nullptr );
    geo = StaticGeometry::create( 1 );
    EXPECT_NE( geo, nullptr );
    StaticGeometry::destroy( &geo );
    EXPECT_EQ( geo, nullptr );
}

TEST_F( RenderCommonTest, accessTransformMatrixBlockTest ) {
    TransformMatrixBlock block;
    block.m_model = glm::translate( block.m_model, glm::vec3( 1, 2, 3 ) );
    block.m_projection = glm::translate( block.m_model, glm::vec3( 1, 2, 3 ) );
    block.m_view = glm::translate( block.m_model, glm::vec3( 1, 2, 3 ) );

    block.init();
}

TEST_F( RenderCommonTest, accessMaterialTest ) {
    bool ok = true;
    try {
        Material *mat( new Material );
        EXPECT_EQ( FlatShadingMaterial, mat->m_type );
        EXPECT_EQ( mat->m_parameters, nullptr );
        EXPECT_EQ( mat->m_numParameters, 0 );
    } catch ( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

} // Namespace UnitTest
} // Namespace OSRE
