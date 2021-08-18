/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/IO/Uri.h>
#include <osre/RenderBackend/Shader.h>
#include <osre/Scene/MaterialBuilder.h>

#include <cstdio>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::RenderBackend;

MaterialBuilder::MaterialCache *MaterialBuilder::s_materialCache = nullptr;

static const String GLSLVersionString_330 =
        "#version 330 core\n";

static const String GLSLVersionString_400 =
        "#version 400 core\n";

static const String GLSLRenderVertexLayout =
        "// RenderVertex layout\n"
        "layout(location = 0) in vec3 position;	  // object space vertex position\n"
        "layout(location = 1) in vec3 normal;	  // object space vertex normal\n"
        "layout(location = 2) in vec3 color0;     // per-vertex diffuse colour\n"
        "layout(location = 3) in vec2 texcoord0;  // per-vertex tex coord, stage 0\n"
        "\n";

static const String GLSLCombinedMVPUniformSrc =
        "// uniforms\n"
        "uniform mat4 Model;\n"
        "uniform mat4 View;\n"
        "uniform mat4 Projection;\n"
        "uniform mat4 MVP;	//combined modelview projection matrix\n";

static const String GLSLVsSrc =
        GLSLVersionString_400 +
        "\n"
        "layout(location = 0) in vec3 position;	 // object space vertex position\n"
        "layout(location = 1) in vec3 normal;    // object space vertex normal\n"
        "layout(location = 2) in vec3 color0;    // per-vertex colour\n"
        "\n"
        "// output from the vertex shader\n"
        "smooth out vec4 vSmoothColor;		//smooth colour to fragment shader\n"
        "\n" +
        GLSLCombinedMVPUniformSrc +
        "\n"
        "void main() {\n"
        "    // assign the per-vertex color to vSmoothColor varying\n"
        "    vSmoothColor = vec4(color0,1);\n"
        "    // get the clip space position by multiplying the combined MVP matrix with the object space\n"
        "    // vertex position\n"
        "    gl_Position = MVP*vec4(position,1);\n"
        "}\n";

const String GLSLFsSrc =
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

const String GLSLVertexShaderSrcRV =
        GLSLVersionString_400 +
        "\n" + GLSLRenderVertexLayout +
        "\n"
        "out vec3 position_eye, normal_eye;\n"
        "// output from the vertex shader\n"
        "smooth out vec4 vSmoothColor;		//smooth colour to fragment shader\n"
        "smooth out vec2 vUV;\n"
        "\n"
        "vec3 light_pos = vec3(0.0, 0.0, 2.0);\n"
        "vec3 Ls        = vec3(1.0, 1.0, 1.0);\n"
        "vec3 Ld        = vec3(0.7, 0.7, 0.7);\n"
        "vec3 La        = vec3(0.7, 0.7, 0.7);\n"
        "\n"
        "vec3 Ks = vec3(1.0, 1.0, 1.0);\n"
        "vec3 Kd = vec3(1.0, 0.5, 0.0);\n"
        "vec3 Ka = vec3(1.0, 1.0, 1.0);\n"
        "float specular_exponent = 100.0;\n"
        "\n" +
        GLSLCombinedMVPUniformSrc +
        "\n"
        "void main()\n"
        "{\n" 
        "    position_eye = vec3(View * Model * vec4(position, 1.0));\n"
        "    normal_eye = vec3(View * Model * vec4(normal, 0.0));\n"
        "    vec3 Ia = La * Ka;\n"
        "    vec3 Is = vec3(0.0, 0.0, 0.0);\n"
        "    //get the clip space position by multiplying the combined MVP matrix with the object space\n" 
        "    vec3 light_position_eye = vec3(View * vec4(light_pos, 1.0));\n"
        "    vec3 distance_to_light_eye = light_position_eye - position_eye;\n" 
        "    vec3 direction_to_light_eye = normalize (distance_to_light_eye);\n"
        "    float dot_prod = dot (direction_to_light_eye, normal_eye);\n"
        "    dot_prod = max (dot_prod, 0.0); "
        "    vec3 Id = Ld * Kd * dot_prod;\n"
        "\n"
        "    //vertex position\n"
        "    gl_Position = MVP*vec4(position,1);\n"
        "    vSmoothColor = vec4(Is + Id + Ia, 1.0);\n"
        "    vUV = texcoord0;\n"
        "}\n";

const String GLSLFragmentShaderSrcRV =
        GLSLVersionString_400 +
        "\n"
        "in vec3 position_eye, normal_eye;\n"
        "layout(location=0) out vec4 frag_volor; //fragment shader output\n"
        "\n"
        "//input form the vertex shader\n"
        "smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader\n"
        "smooth in vec2 vUV;\n"
        "uniform sampler2D tex0;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    // set the interpolated color as the shader output\n"
        "    frag_volor = texture(tex0, vUV) * vSmoothColor;\n"
        "}\n";

const String GLSLVsSrcRV_Editor =
        GLSLVersionString_400 +
        "\n" + GLSLRenderVertexLayout +
        "\n"
        "// output from the vertex shader\n"
        "smooth out vec4 vSmoothColor;		//smooth colour to fragment shader\n"
        "smooth out vec2 TexCoord;\n"
        "out vec3 Position;\n"
        "out vec3 Normal;\n"
        "\n" +
        GLSLCombinedMVPUniformSrc +
        "\n"
        "void main()\n"
        "{\n"
        "    //assign the per-vertex color to vSmoothColor varying\n"
        "    vSmoothColor = vec4(color0,1);\n"
        "\n"
        "    //get the clip space position by multiplying the combined MVP matrix with the object space\n"
        "    //vertex position\n"
        "    Position = (MVP*vec4(position,1)).xyz;\n"
        "    gl_Position = MVP*vec4(position,1);\n"
        "    vSmoothColor = vec4( color0, 1 );\n"
        "    TexCoord = texcoord0;\n"
        "}\n";

const String GLSLFsSrcRV_Editor =
        GLSLVersionString_400 +
        "\n"
        "layout(location=0) out vec4 FragColor; //fragment shader output\n"
        "\n"
        "//input form the vertex shader\n"
        "smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader\n"
        "smooth in vec2 TexCoord;\n"
        "\n"
        "in vec3 Position;\n"
        "in vec3 Normal;\n"
        "uniform sampler2D tex0;\n"
        "\n"
        "struct LightInfo {\n"
        "    vec4 Position;\n"
        "    vec3 La;\n"
        "    vec3 Ld;\n"
        "    vec3 Ls;\n"
        "};\n"
        "uniform LightInfo Light;\n"
        "\n"
        "struct MaterialInfo {\n"
        "    vec3 Ka;\n"
        "    vec3 Kd;\n"
        "    vec3 Ks;\n"
        "    float shineness;\n"
        "};\n"
        "uniform MaterialInfo Material;\n"
        "\n"
        "vec3 phongModel(vec3 position, vec3 normal) {\n"
        "    vec3 s = normalize(Light.Position.xyz - position);\n"
        "    vec3 v = normalize(-position);\n"
        "    vec3 r = reflect(-s, normal);\n"
        "    vec3 ambient = Light.La * Material.Ka;\n"
        "    float sDotN = max(dot(s, normal),0.0);\n"
        "    vec3 diffuse = Light.Ld * Material.Kd * sDotN;\n"
        "    vec3 spec = vec3(0.0);\n"
        "    if ( sDotN > 0.0)\n"
        "        spec = Light.Ls * Material.Ks * pow(max(dot(r,v),0.0), Material.shineness);\n"
        "    return ( ambient + diffuse + spec );\n"
        "}\n"
        "\n"
        "void main()\n"
        "{\n"
        "    // set the interpolated color as the shader output\n"
        "    vec3 ambAndDiff;\n"
        "    vec4 texColor = texture( tex0, TexCoord );\n"

        "    ambAndDiff = phongModel(Position, Normal);"
        "    FragColor = vec4(ambAndDiff, 1.0) * texColor;\n"
        "}\n";

const String GLSLVsSrcUI =
        GLSLVersionString_400 +
        "\n" + GLSLRenderVertexLayout +
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
        "\n" +
        GLSLCombinedMVPUniformSrc +
        "\n"
        "void main() {\n"
        "    //assign the per-vertex color to vSmoothColor varying\n"
        "    vSmoothColor = vec4(color0,1);\n"
        "\n"
        "    //get the clip space position by multiplying the combined MVP matrix with the object space\n"
        "    //vertex position\n"
        "    gl_Position = MVP*vec4(position,1);\n"
        "    vSmoothColor = vec4( color0, 1 );\n"
        "    vUV = texcoord0;\n"
        "}\n";

const String GLSLFsSrcUI =
        GLSLVersionString_400 +
        "\n"
        "layout(location=0) out vec4 vFragColor; //fragment shader output\n"
        "\n"
        "//input form the vertex shader\n"
        "smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader\n"
        "smooth in vec2 vUV;\n"
        "uniform sampler2D tex0;\n"
        "\n"
        "void main() {\n"
        "    // set the interpolated color as the shader output\n"
        "    vFragColor = vSmoothColor;\n"
        "}\n";

static const String GLSLVSLightRenderVertexSrc =
        "";

static const String GLSLFSLightRenderVertexSrc =
        "";

static const String GLSLMaterialParameterStruct =
        "struct MaterialParameter {\n"
        "    vec3 ka;\n"
        "    vec3 kd;\n"
        "    vec3 ks;\n"
        "}\n";

MaterialBuilder::MaterialBuilder() {
    // empty
}

MaterialBuilder::~MaterialBuilder() {
    // empty
}

void MaterialBuilder::create() {
    if (nullptr == s_materialCache) {
        s_materialCache = new MaterialBuilder::MaterialCache;
    }
}

void MaterialBuilder::destroy() {
    delete s_materialCache;
    s_materialCache = nullptr;
}

Material *MaterialBuilder::createBuildinMaterial(VertexType type) {
    Material *mat = s_materialCache->find("buildinShaderMaterial");
    if (nullptr != mat) {
        return mat;
    }
    mat = s_materialCache->create("buildinShaderMaterial", IO::Uri());
    String vs, fs;
    if (type == VertexType::ColorVertex) {
        vs = GLSLVsSrc;
        fs = GLSLFsSrc;
    } else if (type == VertexType::RenderVertex) {
        vs = GLSLVertexShaderSrcRV;
        fs = GLSLFragmentShaderSrcRV;
    }
    if (vs.empty() || fs.empty()) {
        delete mat;
        return nullptr;
    }

    ShaderSourceArray arr;
    arr[static_cast<size_t>(ShaderType::SH_VertexShaderType)] = vs;
    arr[static_cast<size_t>(ShaderType::SH_FragmentShaderType)] = fs;
    mat->createShader(arr);

    // Setup shader attributes and variables
    if (nullptr != mat->m_shader) {
        if (type == VertexType::ColorVertex) {
            mat->m_shader->m_attributes.add(ColorVert::getAttributes(),
                    ColorVert::getNumAttributes());
        } else if (type == VertexType::RenderVertex) {
            mat->m_shader->m_attributes.add(RenderVert::getAttributes(),
                    RenderVert::getNumAttributes());
        }

        mat->m_shader->m_parameters.add("MVP");
    }

    return mat;
}

Material *MaterialBuilder::createBuildinUiMaterial() {
    Material *mat = s_materialCache->find("buildinUiShaderMaterial");
    if (nullptr != mat) {
        return mat;
    }

    mat = s_materialCache->create("buildinUiShaderMaterial", IO::Uri());
    ShaderSourceArray arr;
    arr[static_cast<ui32>(ShaderType::SH_VertexShaderType)] = GLSLVsSrcUI;
    arr[static_cast<ui32>(ShaderType::SH_FragmentShaderType)] = GLSLFsSrcUI;
    mat->createShader(arr);

    // setup shader attributes and variables
    if (nullptr != mat->m_shader) {
        size_t numAttribs(RenderVert::getNumAttributes());
        const String *attribs(RenderVert::getAttributes());
        if (numAttribs > 0) {
            mat->m_shader->m_attributes.add(attribs, numAttribs);
            mat->m_shader->m_parameters.add("MVP");
        }
    }

    return mat;
}

RenderBackend::Material *MaterialBuilder::createTexturedMaterial(const String &matName, TextureResourceArray &texResArray,
        RenderBackend::VertexType type) {
    if (matName.empty()) {
        return nullptr;
    }

    Material *mat = s_materialCache->find(matName);
    if (nullptr != mat) {
        return mat;
    }

    mat = s_materialCache->create(matName);
    mat->m_numTextures = texResArray.size();
    mat->m_textures = new Texture *[texResArray.size()];
    for (size_t i = 0; i < texResArray.size(); ++i) {
        TextureResource *texRes = texResArray[i];
        TextureLoader loader;
        texRes->load(loader);
        mat->m_textures[i] = texRes->get();
    }

    String vs, fs;
    if (type == VertexType::ColorVertex) {
        vs = GLSLVsSrc;
        fs = GLSLFsSrc;
    } else if (type == VertexType::RenderVertex) {
        vs = GLSLVertexShaderSrcRV;
        fs = GLSLFragmentShaderSrcRV;
    }

    if (vs.empty() || fs.empty()) {
        delete mat;
        return nullptr;
    }

    ShaderSourceArray arr;
    arr[static_cast<ui32>(ShaderType::SH_VertexShaderType)] = vs;
    arr[static_cast<ui32>(ShaderType::SH_FragmentShaderType)] = fs;
    mat->createShader(arr);

    // Setup shader attributes and variables
    if (nullptr != mat->m_shader) {
        if (type == VertexType::ColorVertex) {
            mat->m_shader->m_attributes.add(ColorVert::getAttributes(), ColorVert::getNumAttributes());
        } else if (type == VertexType::RenderVertex) {
            mat->m_shader->m_attributes.add(RenderVert::getAttributes(), RenderVert::getNumAttributes());
        }

        mat->m_shader->m_parameters.add("MVP");
    }

    return mat;
}

RenderBackend::Material *MaterialBuilder::createTexturedMaterial(const String &matName, TextureResourceArray &texResArray,
        const String &VsSrc, const String &FsSrc) {
    if (matName.empty()) {
        return nullptr;
    }

    if (VsSrc.empty() || FsSrc.empty()) {
        return nullptr;
    }

    Material *mat = s_materialCache->find(matName);
    if (nullptr != mat) {
        return mat;
    }

    mat = s_materialCache->create(matName);
    mat->m_numTextures = texResArray.size();
    mat->m_textures = new Texture *[texResArray.size()];
    for (size_t i = 0; i < texResArray.size(); ++i) {
        TextureResource *texRes = texResArray[i];
        IO::Uri uri = texRes->getUri();
        TextureLoader loader;
        texRes->load(loader);
        mat->m_textures[i] = texRes->get();
    }

    ShaderSourceArray shArray;
    shArray[static_cast<ui32>(ShaderType::SH_VertexShaderType)] = VsSrc;
    shArray[static_cast<ui32>(ShaderType::SH_FragmentShaderType)] = FsSrc;
    mat->createShader(shArray);

    return mat;
}

} // Namespace Scene
} // namespace OSRE
