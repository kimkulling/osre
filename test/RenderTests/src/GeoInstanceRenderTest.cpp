#include "AbstractRenderTest.h"
#include "RenderTestUtils.h"

#include <osre/Common/Logger.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <src/RenderBackend/OGLRenderer/OGLShader.h>
#include <osre/Scene/GeometryBuilder.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef _WIN32
#   include "GL/wglew.h"
#endif

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

const String VsSrc =
    "#version 400 core\n"
    "\n"
    "layout(location = 0) in vec3 vVertex;	      // object space vertex position\n"
    "layout(location = 1) in vec3 vDiffuseColor;  // per-vertex colour\n"
    "\n"
    "// output from the vertex shader\n"
    "smooth out vec4 vSmoothColor;		//smooth colour to fragment shader\n"
    "\n"
    "// uniform\n"
    "uniform mat4 M[4];	// model matrix per instance\n"
    "uniform mat4 VP;	// combined modelview projection matrix\n"
    "\n"
    "void main() {\n"
    "    //assign the per-vertex color to vSmoothColor varying\n"
    "    vSmoothColor = vec4(vDiffuseColor,1);\n"
    "\n"
    "    //get the clip space position by multiplying the combined MVP matrix with the object space\n"
    "    //vertex position\n"
    "    gl_Position = VP*M[ gl_InstanceID ]*vec4(vVertex,1);\n"
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
///	@class		::ZFXCE2::RenderTest::GeoInstanceRenderTest
///	@ingroup	Test
///
///	@brief
//-------------------------------------------------------------------------------------------------
class GeoInstanceRenderTest : public AbstractRenderTest {
    TransformMatrixBlock m_transformMatrix;

public:
    GeoInstanceRenderTest()
        : AbstractRenderTest( "rendertest/geoinstancerendertest" ) {
        // empty
    } 

    virtual ~GeoInstanceRenderTest() {
        // empty
    }

    virtual bool onCreate( RenderBackendService *rbSrv ) {
        rbSrv->sendEvent( &OnAttachViewEvent, nullptr );
        AttachGeoEventData *attachGeoEvData = new AttachGeoEventData;

        Scene::GeometryBuilder myBuilder;
        Geometry *pGeometry = myBuilder.createTriangle();

        attachGeoEvData->m_numGeo = 1;
        attachGeoEvData->m_pGeometry = pGeometry;

        // use a default material
        pGeometry->m_pMaterial = AbstractRenderTest::createMaterial( VsSrc, FsSrc );
        if( nullptr != pGeometry->m_pMaterial->m_pShader ) {
            pGeometry->m_pMaterial->m_pShader->m_attributes.add( "vVertex" );
            pGeometry->m_pMaterial->m_pShader->m_attributes.add( "vDiffuseColor" );
            pGeometry->m_pMaterial->m_pShader->m_parameters.add( "M" );
            pGeometry->m_pMaterial->m_pShader->m_parameters.add( "VP" );
        }

        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );

        static const ui32 NumInstances = 4;
        attachGeoEvData->m_numInstances = NumInstances;
        glm::mat4 mat[ 4 ];
        glm::mat4 scale = glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.1f ) );
        
        mat[ 0 ] = glm::translate( scale, glm::vec3( -1.0f, -1.0f, 0.f ) );
        mat[ 1 ] = glm::translate( scale, glm::vec3(  1.0f, -1.0f, 0.f ) );
        mat[ 2 ] = glm::translate( scale, glm::vec3( -1.0f,  1.0f, 0.f ) );
        mat[ 3 ] = glm::translate( scale, glm::vec3(  1.0f,  1.0f, 0.f ) );
        
        Parameter *parameterMVP = Parameter::create( "VP", PT_Mat4 );
        ::memcpy( parameterMVP->m_data.m_data, glm::value_ptr( m_transformMatrix.m_projection*m_transformMatrix.m_view ), sizeof( glm::mat4 ) );

        Parameter *parameterM = Parameter::create( "M", PT_Mat4Array, 4 );
        ::memcpy( parameterM->m_data.m_data, glm::value_ptr( mat[ 0 ] ), sizeof( glm::mat4 ) * 4 );
        parameterMVP->m_next = parameterM;

        pGeometry->m_pParameter = parameterMVP;
        pGeometry->m_numParameter += 2;
        
        rbSrv->sendEvent( &OnAttachSceneEvent, attachGeoEvData );

        return true;
    }

    virtual bool onDestroy( RenderBackend::RenderBackendService *pRenderBackendSrv ) {
        return true;
    }

    virtual bool onRender( d32 timediff, RenderBackend::RenderBackendService *pRenderBackendSrv ) {
        return true;
    }
};

ATTACH_RENDERTEST( GeoInstanceRenderTest )

//-------------------------------------------------------------------------------------------------

} // Namespace RenderTest
} // Namespace OSRE
