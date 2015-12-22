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
#include "AbstractRenderTest.h"
#include "RenderTestSuite.h"
#include "RenderTestUtils.h"

#include <osre/Common/Logger.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <src/RenderBackend/OGLRenderer/OGLShader.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "SOIL.h"

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

static const String Tag = "BasetexturerenderTest";

const String VsSrc =
    "#version 400 core\n"
    "\n"
    "layout(location = 0) in vec3 position;	            // object space vertex position\n"
    "layout(location = 1) in vec3 normal;	            // object space vertex normal\n"
    "layout(location = 2) in vec3 color0;	            // object space vertex normal\n"
    "layout(location = 3) in vec2 texcoord0;	        // texture coordinate\n"
    "\n"
    "//output from the vertex shader\n"
    "smooth out vec4 vSmoothColor\n;		            //smooth colour to fragment shader\n"
    "smooth out vec2 vUV;\n"
    "\n"
    "//uniform\n"
    "uniform mat4 MVP;	                                //combined modelview projection matrix\n"
    "\n"
    "void main() {\n"
    "    // assign the per-vertex color to vSmoothColor varying\n"
    "    vSmoothColor = vec4(color0,1);\n"
    "\n"
    "    // get the clip space position by multiplying the combined MVP matrix with the object space\n"
    "    // vertex position\n"
    "    gl_Position = MVP*vec4(position,1);\n"
    "    vUV = texcoord0;\n"
    "}\n";

const String FsSrc =
    "#version 400 core\n"
    "\n"
    "layout(location=0) out vec4 vFragColor; //fragment shader output\n"
    "\n"
    "//input form the vertex shader\n"
    "smooth in vec4 vSmoothColor;		    //interpolated colour to fragment shader\n"
    "smooth in vec2 vUV;\n"
    "uniform sampler2D tex0;\n"
    "\n"
    "void main() {\n"
    "    //set the interpolated color as the shader output\n"
    "    vFragColor = texture( tex0, vUV );\n"
    "}\n";


//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::RenderTest::BaseTextureRenderTest
///	@ingroup	Test
///
///	@brief  This class implements a simple texture render test.
//-------------------------------------------------------------------------------------------------
class BaseTextureRenderTest : public AbstractRenderTest {
    f32 m_angle;
    TransformMatrixBlock m_transformMatrix;
    Parameter *m_mvpParam;

public:
    BaseTextureRenderTest()
    : AbstractRenderTest( "rendertest/basetexturerendertest" )
    , m_angle( 0.02f )
    , m_transformMatrix()
    , m_mvpParam( nullptr ){
        // empty
    }

    virtual ~BaseTextureRenderTest( ) {
        delete m_mvpParam;
        m_mvpParam = nullptr;
    }

    virtual bool onCreate( RenderBackend::RenderBackendService *pRenderBackendSrv ) {
        osre_debug( Tag, "BaseTextureRenderTest::onCreate" );

        pRenderBackendSrv->sendEvent( &OnAttachViewEvent, nullptr );
        AttachGeoEventData *attachGeoEvData = new AttachGeoEventData;

        StaticGeometry *geo = StaticGeometry::create();
        geo->m_vertextype = RenderVertex;
        geo->m_indextype = UnsignedShort;

        RenderVert vertices[ 4 ];
        GLushort indices[ 6 ];

        // setup triangle vertices        
        vertices[ 0 ].position = glm::vec3( -1, -1, 0 );
        vertices[ 1 ].position = glm::vec3( -1,  1, 0 );
        vertices[ 2 ].position = glm::vec3(  1, -1, 0 );
        vertices[ 3 ].position = glm::vec3(  1,  1, 0 );

        vertices[ 0 ].tex0 = glm::vec2( 0, 0 );
        vertices[ 1 ].tex0 = glm::vec2( 0, 1 );
        vertices[ 2 ].tex0 = glm::vec2( 1, 0 );
        vertices[ 3 ].tex0 = glm::vec2( 1, 1 );

        geo->m_vb = new BufferData;
        geo->m_vb->m_size = sizeof( RenderVert ) * 4;
        geo->m_vb->m_access = ReadOnly;
        geo->m_vb->m_type = VertexBuffer;

        geo->m_vb->m_pData = new uc8[ geo->m_vb->m_size ];
        ::memcpy( geo->m_vb->m_pData, vertices, geo->m_vb->m_size );

        // setup triangle indices
        indices[ 0 ] = 0;
        indices[ 1 ] = 1;
        indices[ 2 ] = 2;

        indices[ 3 ] = 1;
        indices[ 4 ] = 2;
        indices[ 5 ] = 3;
        
        geo->m_ib = new BufferData;
        geo->m_ib->m_size = sizeof( GLushort ) * 6;
        geo->m_ib->m_access = ReadOnly;
        geo->m_ib->m_type = IndexBuffer;
        
        geo->m_ib->m_pData = new uc8[ geo->m_ib->m_size ];
        ::memcpy( geo->m_ib->m_pData, indices, geo->m_ib->m_size );

        attachGeoEvData->m_numGeo = 1;
        attachGeoEvData->m_geo = geo;

        // use default material
        geo->m_material = AbstractRenderTest::createMaterial( VsSrc, FsSrc );
        if( nullptr != geo->m_material->m_pShader ) {
            geo->m_material->m_pShader->m_attributes.add( "position" );
            geo->m_material->m_pShader->m_attributes.add( "normal" );
            geo->m_material->m_pShader->m_attributes.add( "color0" );
            geo->m_material->m_pShader->m_attributes.add( "texcoord0" );
            geo->m_material->m_pShader->m_parameters.add( "MVP" );
        }

        geo->m_material->m_numTextures = 1;
        geo->m_material->m_pTextures = new Texture[ 1 ];

#ifdef OSRE_WINDOWS
        geo->m_material->m_pTextures[ 0 ].m_textureName = "../../media/Models/Obj/SpiderTex.jpg";
#else
        geo->m_material->m_pTextures[ 0 ].m_textureName = "../media/Models/Obj/SpiderTex.jpg";
#endif
        geo->m_material->m_pTextures[ 0 ].m_targetType = Texture2D;
        geo->m_material->m_pTextures[ 0 ].m_width = 0;
        geo->m_material->m_pTextures[ 0 ].m_height = 0;
        geo->m_material->m_pTextures[ 0 ].m_channels = 0;
        geo->m_material->m_pTextures[ 0 ].m_data = nullptr;
        geo->m_material->m_pTextures[ 0 ].m_size = 0;

        geo->m_numPrimGroups = 1;
        geo->m_pPrimGroups = new PrimitiveGroup[ 1 ];
        geo->m_pPrimGroups[ 0 ].m_indexType = UnsignedShort;
        geo->m_pPrimGroups[ 0 ].m_numPrimitives = 6;
        geo->m_pPrimGroups[ 0 ].m_primitive = TriangleList;
        geo->m_pPrimGroups[ 0 ].m_startIndex = 0;

        pRenderBackendSrv->sendEvent( &OnAttachSceneEvent, attachGeoEvData );

        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, m_angle, glm::vec3( 1, 1, 0 ) );
        
        Parameter *parameter = Parameter::create( "MVP", PT_Mat4 );
        ::memcpy( parameter->m_data.m_data, glm::value_ptr( m_transformMatrix.m_projection*m_transformMatrix.m_view*m_transformMatrix.m_model ), sizeof( glm::mat4 ) );
        
        geo->m_parameter = parameter;
        geo->m_numParameter++;

        return true;
    }

    virtual bool onDestroy( RenderBackend::RenderBackendService *pRenderBackendSrv ) {
        osre_debug( Tag, "BaseTextureRenderTest::onDestroy" );

        return true;
    }

    virtual bool onRender( d32 timediff, RenderBackend::RenderBackendService *pRenderBackendSrv ) {
        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, m_angle, glm::vec3( 1, 1, 0 ) );
       
        if( nullptr == m_mvpParam ) {
            m_mvpParam = Parameter::create( "MVP", PT_Mat4 );
        }

        UpdateParameterEventData *data( new UpdateParameterEventData );
        data->m_numParam = 1;
        data->m_param = m_mvpParam;
        ::memcpy( data->m_param->m_data.m_data, glm::value_ptr( m_transformMatrix.m_projection*m_transformMatrix.m_view*m_transformMatrix.m_model ), sizeof( glm::mat4 ) );

        pRenderBackendSrv->sendEvent( &OnUpdateParameterEvent, data );

        return true;
    }
};

ATTACH_RENDERTEST( BaseTextureRenderTest )

} // Namespace RenderTest
} // Namespace OSRE
