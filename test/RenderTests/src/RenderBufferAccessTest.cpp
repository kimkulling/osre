/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <src/Engine/RenderBackend/OGLRenderer/OGLShader.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/MaterialBuilder.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef OSRE_WINDOWS
#   include "GL/wglew.h"
#endif

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

const String VsSrc =
    "#version 400 core\n"
    "\n"
    "layout(location = 0) in vec3 position;	     // object space vertex position\n"
    "layout(location = 1) in vec3 normal;	     // object space vertex normal\n"
    "layout(location = 2) in vec3 color0;        // per-vertex colour\n"
    "\n"
    "// output from the vertex shader\n"
    "smooth out vec4 vSmoothColor;		//smooth colour to fragment shader\n"
    "\n"
    "// uniform\n"
    "uniform mat4 M[25];	// model matrix per instance\n"
    "uniform mat4 VP;	// combined modelview projection matrix\n"
    "\n"
    "void main() {\n"
    "    //assign the per-vertex color to vSmoothColor varying\n"
    "    vSmoothColor = vec4(color0,1);\n"
    "\n"
    "    //get the clip space position by multiplying the combined MVP matrix with the object space\n"
    "    //vertex position\n"
    "    gl_Position = VP*M[ gl_InstanceID ]*vec4(position,1);\n"
    "}\n";

const String FsSrc =
    "#version 400 core\n"
    "\n"
    "layout(location=0) out vec4 vFragColor; //fragment shader output\n"
    "\n"
    "//input form the vertex shader\n"
    "smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader\n"
    "\n"
    "void main() {\n"
    "    //set the interpolated color as the shader output\n"
    "    vFragColor = vSmoothColor;\n"
    "}\n";

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::RenderTest::GeoInstanceRenderTest
///	@ingroup	Test
///
///	@brief
//-------------------------------------------------------------------------------------------------
class RenderBufferAccessTest : public AbstractRenderTest {
    TransformMatrixBlock m_transformMatrix;

public:
    RenderBufferAccessTest()
        : AbstractRenderTest( "rendertest/RenderBufferAccessTest" ) {
        // empty
    }

    virtual ~RenderBufferAccessTest() {
        // empty
    }

    virtual bool onCreate( RenderBackendService *rbSrv ) {
        rbSrv->sendEvent( &OnAttachViewEvent, nullptr );
        AttachGeoEventData *attachGeoEvData = new AttachGeoEventData;

        // colors
        glm::vec3 col[ 3 ];
        col[ 0 ] = glm::vec3( 1, 0, 0 );
        col[ 1 ] = glm::vec3( 0, 1, 0 );
        col[ 2 ] = glm::vec3( 0, 0, 1 );

        // point coordinates
        glm::vec3 points[ 3 ];
        points[ 0 ] = glm::vec3( -0.5, -0.5, 0 );
        points[ 1 ] = glm::vec3( 0, 0.5, 0 );
        points[ 2 ] = glm::vec3( 0.5, -0.5, 0 );
        static const ui32 PtNumIndices = 3;
        GLushort pt_indices[ PtNumIndices ];
        pt_indices[ 0 ] = 0;
        pt_indices[ 1 ] = 1;
        pt_indices[ 2 ] = 2;

        static ui32 NumGeo( 1 );
        attachGeoEvData->m_numGeo = NumGeo;
        attachGeoEvData->m_geo = Scene::GeometryBuilder::allocEmptyGeometry( VertexType::ColorVertex, NumGeo );

        Geometry *ptGeo = &attachGeoEvData->m_geo[ 0 ];
        ptGeo->m_vb = Scene::GeometryBuilder::allocVertices( VertexType::ColorVertex, 3, points, col, nullptr, BufferAccessType::ReadOnly );
        ptGeo->m_indextype = IndexType::UnsignedShort;
        ui32 pt_size = sizeof( GLushort ) * PtNumIndices;
        ptGeo->m_ib = BufferData::alloc( BufferType::IndexBuffer, pt_size, BufferAccessType::ReadOnly );
        ptGeo->m_ib->copyFrom( pt_indices, pt_size );

        // setup primitives
        ptGeo->m_numPrimGroups = 1;
        ptGeo->m_pPrimGroups = new PrimitiveGroup[ ptGeo->m_numPrimGroups ];
        ptGeo->m_pPrimGroups[ 0 ].init( IndexType::UnsignedShort, 3, PrimitiveType::PointList, 0 );

        // setup material
        Material *mat = Scene::MaterialBuilder::createBuildinMaterial( VertexType::ColorVertex );
        ptGeo->m_material = mat;

        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
        m_transformMatrix.m_model = glm::scale( m_transformMatrix.m_model, glm::vec3( .5, .5, .5 ) );
        Parameter *parameter = Parameter::create( "MVP", PT_Mat4 );
        ::memcpy( parameter->m_data.m_data, glm::value_ptr( m_transformMatrix.m_projection*m_transformMatrix.m_view*m_transformMatrix.m_model ), sizeof( glm::mat4 ) );

        mat->m_numParameters = 1;
        mat->m_parameters = parameter;

        rbSrv->sendEvent( &OnAttachSceneEvent, attachGeoEvData );

        return true;
    }

    virtual bool onDestroy( RenderBackend::RenderBackendService *pRenderBackendSrv ) {
        return true;
    }
};

ATTACH_RENDERTEST( RenderBufferAccessTest )

} // Namespace RenderTest
} // Namespace OSRE
