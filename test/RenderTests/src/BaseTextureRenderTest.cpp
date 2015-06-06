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
    "layout(location = 0) in vec3 vVertex;	            // object space vertex position\n"
    "layout(location = 1) in vec2 TexCoord0Attribute;	// texture coordinate\n"
    "\n"
    "//output from the vertex shader\n"
    "smooth out vec4 vSmoothColor\n;		            //smooth colour to fragment shader\n"
    "smooth out vec2 vUV;\n"
    "\n"
    "//uniform\n"
    "uniform mat4 MVP;	                                //combined modelview projection matrix\n"
    "\n"
    "void main() {\n"
    "    // assign the per-vertex colour to vSmoothColor varying\n"
    "    vSmoothColor = vec4( 1,0,0,0);\n"
    "\n"
    "    // get the clip space position by multiplying the combined MVP matrix with the object space\n"
    "    // vertex position\n"
    "    gl_Position = MVP*vec4(vVertex,1);\n"
    "    vUV = TexCoord0Attribute;\n"
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
    "    //vFragColor = vSmoothColor;\n"
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
    , m_angle( 0.5f )
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

        Geometry *pGeometry = new Geometry[ 1 ];
        pGeometry->m_vertextype = RenderVertex;
        pGeometry->m_indextype = UnsignedShort;

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

        pGeometry->m_vb = new BufferData;
        pGeometry->m_vb->m_size = sizeof( RenderVert ) * 4;
        pGeometry->m_vb->m_access = ReadOnly;
        pGeometry->m_vb->m_type = VertexBuffer;

        pGeometry->m_vb->m_pData = new uc8[ pGeometry->m_vb->m_size ];
        ::memcpy( pGeometry->m_vb->m_pData, vertices, pGeometry->m_vb->m_size );

        // setup triangle indices
        indices[ 0 ] = 0;
        indices[ 1 ] = 1;
        indices[ 2 ] = 2;

        indices[ 3 ] = 1;
        indices[ 4 ] = 2;
        indices[ 5 ] = 3;
        
        pGeometry->m_ib = new BufferData;
        pGeometry->m_ib->m_size = sizeof( GLushort ) * 6;
        pGeometry->m_ib->m_access = ReadOnly;
        pGeometry->m_ib->m_type = IndexBuffer;
        
        pGeometry->m_ib->m_pData = new uc8[ pGeometry->m_ib->m_size ];
        ::memcpy( pGeometry->m_ib->m_pData, indices, pGeometry->m_ib->m_size );

        attachGeoEvData->m_numGeo = 1;
        attachGeoEvData->m_pGeometry = pGeometry;

        // use default material
        pGeometry->m_material = AbstractRenderTest::createMaterial( VsSrc, FsSrc );
        if( nullptr != pGeometry->m_material->m_pShader ) {
            pGeometry->m_material->m_pShader->m_attributes.add( "vVertex" );
            pGeometry->m_material->m_pShader->m_attributes.add( "TexCoord0Attribute" );
            pGeometry->m_material->m_pShader->m_parameters.add( "MVP" );
        }

        pGeometry->m_material->m_numTextures = 1;
        pGeometry->m_material->m_pTextures = new Texture[ 1 ];
#ifdef _WIN32
        pGeometry->m_material->m_pTextures[ 0 ].m_textureName = "../../media/Models/Obj/SpiderTex.jpg";
#else
        pGeometry->m_material->m_pTextures[ 0 ].m_textureName = "../media/Models/Obj/SpiderTex.jpg";
#endif
        pGeometry->m_material->m_pTextures[ 0 ].m_targetType = Texture2D;
        pGeometry->m_material->m_pTextures[ 0 ].m_width = 0;
        pGeometry->m_material->m_pTextures[ 0 ].m_height = 0;
        pGeometry->m_material->m_pTextures[ 0 ].m_channels = 0;
        pGeometry->m_material->m_pTextures[ 0 ].m_pData = nullptr;
        pGeometry->m_material->m_pTextures[ 0 ].m_size = 0;

        pGeometry->m_numPrimGroups = 1;
        pGeometry->m_pPrimGroups = new PrimitiveGroup[ 1 ];
        pGeometry->m_pPrimGroups[ 0 ].m_indexType = UnsignedShort;
        pGeometry->m_pPrimGroups[ 0 ].m_numPrimitives = 6;
        pGeometry->m_pPrimGroups[ 0 ].m_primitive = TriangleList;
        pGeometry->m_pPrimGroups[ 0 ].m_startIndex = 0;

        pRenderBackendSrv->sendEvent( &OnAttachSceneEvent, attachGeoEvData );

        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, m_angle, glm::vec3( 1, 1, 0 ) );
        
        Parameter *parameter = Parameter::create( "MVP", PT_Mat4 );
        ::memcpy( parameter->m_data.m_data, glm::value_ptr( m_transformMatrix.m_projection*m_transformMatrix.m_view*m_transformMatrix.m_model ), sizeof( glm::mat4 ) );
        
        pGeometry->m_parameter = parameter;
        pGeometry->m_numParameter++;

        return true;
    }

    virtual bool onDestroy( RenderBackend::RenderBackendService *pRenderBackendSrv ) {
        osre_debug( Tag, "BaseTextureRenderTest::onDestroy" );

        return true;
    }

    virtual bool onRender( d32 timediff, RenderBackend::RenderBackendService *pRenderBackendSrv ) {
        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, m_angle, glm::vec3( 1, 1, 0 ) );
       
        UpdateParameterEventData *data( new UpdateParameterEventData );
        data->m_numParam = 1;
        data->m_param = Parameter::create( "MVP", PT_Mat4 );
        ::memcpy( data->m_param->m_data.m_data, glm::value_ptr( m_transformMatrix.m_projection*m_transformMatrix.m_view*m_transformMatrix.m_model ), sizeof( glm::mat4 ) );

        pRenderBackendSrv->sendEvent( &OnUpdateParameterEvent, data );

        return true;
    }
};

ATTACH_RENDERTEST( BaseTextureRenderTest )

} // Namespace RenderTest
} // Namespace OSRE
