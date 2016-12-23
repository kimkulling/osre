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
        Geometry *geo = myBuilder.allocTriangles( VertexType::ColorVertex, BufferAccessType::ReadOnly );

        attachGeoEvData->m_numGeo = 1;
        attachGeoEvData->m_geo = new Geometry*[ attachGeoEvData->m_numGeo ];
        attachGeoEvData->m_geo[0] = geo;

        // use a default material
        geo->m_material = AbstractRenderTest::createMaterial( VsSrc, FsSrc );
        if( nullptr != geo->m_material->m_pShader ) {
            geo->m_material->m_pShader->m_attributes.add( "position" );
            geo->m_material->m_pShader->m_attributes.add( "normal" );
            geo->m_material->m_pShader->m_attributes.add( "color0" );
            geo->m_material->m_pShader->m_parameters.add( "M" );
            geo->m_material->m_pShader->m_parameters.add( "VP" );
        }

        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
        m_transformMatrix.update();

        static const ui32 NumInstances = 25;
        attachGeoEvData->m_numInstances = NumInstances;
        glm::mat4 mat[NumInstances];
        glm::mat4 scale = glm::scale( glm::mat4( 1.0f ), glm::vec3( 0.1f ) );
        
		auto idx(0);
		auto x(-2.0f), y(-2.0f);
		for (auto i = 0; i < 5; i++) {
			x = -2.0f;
			for (auto j = 0; j < 5; j++) {
				mat[ idx ] = glm::translate( scale, glm::vec3( x, y, 0.f ) );
				x += 2.0f;
				++idx;
			}
			y += 2.0f;
		}
        
        Parameter *parameterMVP = Parameter::create( "VP", ParameterType::PT_Mat4 );
        ::memcpy( parameterMVP->m_data.m_data, m_transformMatrix.getMVP(), sizeof( glm::mat4 ) );

        Parameter *parameterM = Parameter::create( "M", ParameterType::PT_Mat4Array, NumInstances);
        ::memcpy( parameterM->m_data.m_data, glm::value_ptr( mat[ 0 ] ), sizeof( glm::mat4 ) * NumInstances);
        parameterMVP->m_next = parameterM;

        geo->m_material->m_parameters = parameterMVP;
        geo->m_material->m_numParameters += 2;
        
        rbSrv->sendEvent( &OnAttachSceneEvent, attachGeoEvData );

        return true;
    }

    virtual bool onDestroy( RenderBackend::RenderBackendService *pRenderBackendSrv ) {
        return true;
    }
};

ATTACH_RENDERTEST( GeoInstanceRenderTest )

} // Namespace RenderTest
} // Namespace OSRE
