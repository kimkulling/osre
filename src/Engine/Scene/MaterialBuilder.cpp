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
#include <osre/Scene/MaterialBuilder.h>

namespace OSRE {
namespace Scene {
        
using namespace ::OSRE::RenderBackend;

static const String GLSLVersionString_400 = 
    "#version 400 core\n";

static const String GLSLRenderVertexLayout =
	"// RenderVertex layout\n"
	"layout(location = 0) in vec3 position;	  // object space vertex position\n"
	"layout(location = 1) in vec3 normal;	  // object space vertex normal\n"
	"layout(location = 2) in vec3 color0;     // per-vertex colour\n"
	"layout(location = 3) in vec2 texcoord0;  // per-vertex colour\n"
	"\n";


static const String CombinedMVPUniformSrc =
    "// uniform\n"
    "uniform mat4 MVP;	//combined modelview projection matrix\n";

static const String VsSrc =
    GLSLVersionString_400 +
    "\n"
    "layout(location = 0) in vec3 position;	      // object space vertex position\n"
    "layout(location = 1) in vec3 normal;	            // object space vertex normal\n"
    "layout(location = 2) in vec3 color0;  // per-vertex colour\n"
    "\n"
    "// output from the vertex shader\n"
    "smooth out vec4 vSmoothColor;		//smooth colour to fragment shader\n"
    "\n"
    + CombinedMVPUniformSrc +
    "\n"
    "void main() {\n"
    "    // assign the per-vertex color to vSmoothColor varying\n"
    "    vSmoothColor = vec4(color0,1);\n"
    "    // get the clip space position by multiplying the combined MVP matrix with the object space\n"
    "    // vertex position\n"
    "    gl_Position = MVP*vec4(position,1);\n"
    "}\n";

const String FsSrc =
    GLSLVersionString_400 +
    "\n"
    "layout(location=0) out vec4 vFragColor; //fragment shader output\n"
    "\n"
    "//input form the vertex shader\n"
    "smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader\n"
    "\n"
    "void main() {\n"
    "    // set the interpolated color as the shader output\n"
    "    vFragColor = vSmoothColor;\n"
    "}\n";

const String VsSrcRV =
    GLSLVersionString_400 +
    "\n"
	+ GLSLRenderVertexLayout +
    "\n"
    "// output from the vertex shader\n"
    "smooth out vec4 vSmoothColor;		//smooth colour to fragment shader\n"
    "smooth out vec2 vUV;\n"
    "\n"
    + CombinedMVPUniformSrc +
    "\n"
    "void main()\n"
    "{\n"
    "    //assign the per-vertex color to vSmoothColor varying\n"
    "    vSmoothColor = vec4(color0,1);\n"
    "\n"
    "    //get the clip space position by multiplying the combined MVP matrix with the object space\n"
    "    //vertex position\n"
    "    gl_Position = MVP*vec4(position,1);\n"
    "    vSmoothColor = vec4( color0, 1 );\n"
    "    vUV = texcoord0;\n"
    "}\n";

const String FsSrcRV =
    GLSLVersionString_400 +
    "\n"
    "layout(location=0) out vec4 vFragColor; //fragment shader output\n"
    "\n"
    "//input form the vertex shader\n"
    "smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader\n"
    "smooth in vec2 vUV;\n"
    "uniform sampler2D tex0;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    // set the interpolated color as the shader output\n"
	"    vec4 texColor = texture( tex0, vUV );\n"
	"    vFragColor = texColor + vSmoothColor;\n"
    "}\n";

const String VsSrcUI =
    GLSLVersionString_400 +
    "\n"
    + GLSLRenderVertexLayout +
    "\n"
    "// fixed point light properties\n"
    "vec3 light_position_world = vec3 (0.0, 0.0, 2.0);\n"
    "vec3 Ls = vec3 (1.0, 1.0, 1.0); // white specular colour\n"
    "vec3 Ld = vec3 (0.7, 0.7, 0.7); // dull white diffuse light colour\n"
    "vec3 La = vec3 (0.2, 0.2, 0.2); // grey ambient colour\n"
    "\n"
    "// surface reflectance\n"
    "vec3 Ks = vec3 (1.0, 1.0, 1.0); // fully reflect specular light\n"
    "vec3 Kd = vec3 (1.0, 0.5, 0.0); // orange diffuse surface reflectance\n"
    "vec3 Ka = vec3 (1.0, 1.0, 1.0); // fully reflect ambient light\n"
    "float specular_exponent = 100.0; // specular 'power'\n"
    "\n"
    "// output from the vertex shader\n"
    "smooth out vec4 vSmoothColor;		//smooth colour to fragment shader\n"
    "smooth out vec2 vUV;\n"
    "\n"
    + CombinedMVPUniformSrc +
    "\n"
    "void main()\n"
    "{\n"
    "    //assign the per-vertex color to vSmoothColor varying\n"
    "    vSmoothColor = vec4(color0,1);\n"
    "\n"
    "    //get the clip space position by multiplying the combined MVP matrix with the object space\n"
    "    //vertex position\n"
    "    gl_Position = MVP*vec4(position,1);\n"
    "    vSmoothColor = vec4( color0, 1 );\n"
    "    vUV = texcoord0;\n"
    "}\n";

const String FsSrcUI =
    GLSLVersionString_400 +
    "\n"
    "layout(location=0) out vec4 vFragColor; //fragment shader output\n"
    "\n"
    "//input form the vertex shader\n"
    "smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader\n"
    "smooth in vec2 vUV;\n"
    "uniform sampler2D tex0;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    // set the interpolated color as the shader output\n"
    "    vFragColor = vSmoothColor;\n"
    "}\n";

static const String VSLightRenderVertexSrc = "";
static const String FSLightRenderVertexSrc = "";

MaterialBuilder::MaterialBuilder() {
    // empty
}

MaterialBuilder::~MaterialBuilder() {
    // empty
}

 Material *MaterialBuilder::createBuildinMaterial(VertexType type) {
    Material *mat = new Material( "shadermaterial", MaterialType::ShaderMaterial);

    String vs, fs;
    if ( type == VertexType::ColorVertex ) {
        vs = VsSrc;
        fs = FsSrc;
    } else if ( type == VertexType::RenderVertex ) {
        vs = VsSrcRV;
        fs = FsSrcRV;
    }

    if ( vs.empty() || fs.empty() ) {
        delete mat;
        return nullptr;
    }

    mat->createShader( vs, fs );

    // Setup shader attributes and variables
    if ( nullptr != mat->m_shader ) {
        if ( type == VertexType::ColorVertex ) {
            ui32 numAttribs( ColorVert::getNumAttributes() );
            const String *attribs( ColorVert::getAttributes() );
            mat->m_shader->m_attributes.add( attribs, numAttribs );
        } else if ( type == VertexType::RenderVertex ) {
            ui32 numAttribs( RenderVert::getNumAttributes() );
            const String *attribs( RenderVert::getAttributes() );
            mat->m_shader->m_attributes.add( attribs, numAttribs );
        }

        mat->m_shader->m_parameters.add( "MVP" );
    }

    return mat;
}

Material *MaterialBuilder::createBuildinUiMaterial() {
    Material *mat = new Material( "shadermaterial", MaterialType::ShaderMaterial );
    mat->createShader( VsSrcUI, FsSrcUI );

    // setup shader attributes and variables
    if ( nullptr != mat->m_shader ) {
        ui32 numAttribs( RenderVert::getNumAttributes() );
        const String *attribs( RenderVert::getAttributes() );
        mat->m_shader->m_attributes.add( attribs, numAttribs );
        mat->m_shader->m_parameters.add( "MVP" );
    }

    return mat;
}

} // Namespace Scene
} // namespace OSRE
