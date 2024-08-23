/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "RenderBackend/MaterialBuilder.h"
#include "RenderBackend/Shader/DefaultShader.h"
#include "RenderBackend/Shader.h"
#include "RenderBackend/Material.h"
#include "Debugging/osre_debugging.h"
#include "IO/Uri.h"

#include <cstdio>

namespace OSRE {
namespace RenderBackend {

MaterialBuilder::Data *MaterialBuilder::sData = nullptr;

static void addMaterialParameter(Material *mat) {
    if (mat == nullptr) {
        osre_assert(false);
        return;
    }

    Shader *shader = mat->m_shader;
    if (shader == nullptr) {
        osre_assert(false);
        return;
    }

    shader->addUniformBuffer("Model");
    shader->addUniformBuffer("View");
    shader->addUniformBuffer("Projection");
}

// see https://jasonliang.js.org/batch-renderer.html
 const String vertex_2d =
        getGLSLVersionString_400() + 
        getGLSLRenderVertexLayout() +
        "out vec2 v_texindex;\n"
        "uniform mat4 Model;\n"
        "uniform mat4 View;\n"
        "uniform mat4 Projection;\n"
        "void main() {\n"
        "    mat4 u_mvp = Projection * View * Model;\n"
        "    gl_Position = u_mvp * vec4(position, 1.0);\n"
        "    v_texindex = texcoord0;\n"
        "}\n";

const String fragment_2d = "#version 330 core\n"
                       "in vec2 v_texindex;\n"
                       "out vec4 f_color;\n"
                       "uniform sampler2D u_texture;\n"
                       "void main() {\n"
                       "    f_color = texture(u_texture, v_texindex);\n"
                       "}\n";

static constexpr c8 Render2DMat[] = "2d_mat";

Material *MaterialBuilder::create2DMaterial() {

    MaterialBuilder::MaterialCache *materialCache = sData->mMaterialCache;
    Material *mat = materialCache->find(Render2DMat);
    if (nullptr != mat) {
        return mat;
    }

    mat = materialCache->create(Render2DMat);

    ShaderSourceArray shArray;
    shArray[static_cast<ui32>(ShaderType::SH_VertexShaderType)] = vertex_2d;
    shArray[static_cast<ui32>(ShaderType::SH_FragmentShaderType)] = fragment_2d;
    mat->createShader(shArray);

        // Setup shader attributes and variables
    if (nullptr != mat->m_shader) {
        mat->m_shader->addVertexAttributes(ColorVert::getAttributes(), ColorVert::getNumAttributes());
        addMaterialParameter(mat);
    }

    return mat;
}

static const String GLSLVsSrc =
        getGLSLVersionString_400() +
        getNewLine() +
        getGLSLColorVertexLayout() +
        getNewLine() +
        "// output from the vertex shader\n"
        "smooth out vec4 vSmoothColor;		//smooth colour to fragment shader\n" +
        getNewLine() +
        getGLSLCombinedMVPUniformSrc() +
        getNewLine() +
        "void main() {\n"
        "    mat4 ModelView = View * Model;\n"
        "    mat4 MVP = Projection * ModelView;\n"
        "    // assign the per-vertex color to vSmoothColor varying\n"
        "    vSmoothColor = vec4(color0,1);\n"
        "    // get the clip space position by multiplying the combined MVP matrix with the object space\n"
        "    // vertex position\n"
        "    gl_Position = MVP*vec4(position,1);\n"
        "}\n";

const String GLSLFsSrc =
        getGLSLVersionString_400() +
        getNewLine() +
        "layout(location=0) out vec4 vFragColor; //fragment shader output\n" +
        getNewLine() +
        "//input form the vertex shader\n"
        "smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader\n" +
        getNewLine() +
        "void main() {\n"
        "    // set the interpolated color as the shader output\n"
        "    vFragColor = vSmoothColor;\n"
        "}\n";

const String GLSLVertexShaderSrcRV =
        getGLSLVersionString_400() +
        "\n" + getGLSLRenderVertexLayout() +
        getNewLine() +
        "out vec3 position_eye, normal_eye;\n"
        "// output from the vertex shader\n"
        "smooth out vec4 vSmoothColor;		//smooth colour to fragment shader\n"
        "smooth out vec2 vUV;\n" +
        getNewLine() +
        "vec3 light_pos = vec3(0.0, 0.0, 2.0);\n"
        "vec3 Ls        = vec3(1.0, 1.0, 1.0);\n"
        "vec3 Ld        = vec3(0.7, 0.7, 0.7);\n"
        "vec3 La        = vec3(0.7, 0.7, 0.7);\n"
        "float radius   = 100.0; // todo: set this via uniform\n" +
        getNewLine() +
        "vec3 Ks = vec3(1.0, 1.0, 1.0);\n"
        "vec3 Kd = vec3(1.0, 0.5, 0.0);\n"
        "vec3 Ka = vec3(1.0, 1.0, 1.0);\n"
        "float specular_exponent = 100.0;\n" +
        getNewLine() +
        getGLSLCombinedMVPUniformSrc() +
        getNewLine() +
        "void main()\n"
        "{\n" 
        "    position_eye = vec3(View * Model * vec4(position, 1.0));\n"
        "    normal_eye = vec3(View * Model * vec4(normal, 0.0));\n"
        "    vec3 Ia = La * Ka;\n"
        "    // get the clip space position by multiplying the combined MVP matrix with the object space\n" 
        "    vec3 light_position_eye = vec3(View * vec4(light_pos, 1.0));\n"
        "    vec3 distance_to_light_eye = light_position_eye - position_eye;\n" 
        "    float distance = length(distance_to_light_eye);\n" 
        "    float intensity = cos(90 / (radius * distance));\n"
        "    vec3 direction_to_light_eye = normalize(distance_to_light_eye);\n"
        "    vec3 reflection_eye = reflect(-direction_to_light_eye, normal_eye);\n"
        "    vec3 surface_to_viewer_eye = normalize(-position_eye);\n"
        "    float dot_prod_specular = dot(reflection_eye, surface_to_viewer_eye);\n" 
        "    dot_prod_specular = max(dot_prod_specular, 0.0);\n" +
        getNewLine() +
        "    float dot_prod = dot (direction_to_light_eye, normal_eye);\n"
        "    dot_prod = max (dot_prod, 0.0); \n" +
        getNewLine() +
        "    float specular_factor = pow(dot_prod_specular, specular_exponent);\n"
        "    vec3 Is = Ls * Ks * specular_factor; // final specular intensity\n"
        "    vec3 Id = Ld * Kd * dot_prod;\n" +
        getNewLine() +
        "    //vertex position\n" 
        "    gl_Position = Projection * vec4(position_eye, 1.0);\n"
        "    vSmoothColor = vec4(Is + Id + Ia, 1.0) * intensity;\n"
        "    vUV = texcoord0;\n"
        "}\n";

const String GLSLFragmentShaderSrcRV =
        getGLSLVersionString_400() +
        getNewLine() +
        "in vec3 position_eye, normal_eye;\n"
        "layout(location=0) out vec4 frag_color; //fragment shader output\n" +
        getNewLine() +
        "//input form the vertex shader\n"
        "smooth in vec4 vSmoothColor;		//interpolated colour to fragment shader\n"
        "smooth in vec2 vUV;\n"
        "uniform sampler2D tex0;\n" +
        getNewLine() +
        "void main()\n"
        "{\n"
        "    // set the interpolated color as the shader output\n"
        "    vec4 tex_color = texture(tex0, vUV);\n"
        "    if (tex_color.r >0.0 || tex_color.g >0.0 || tex_color.b>0.0)\n"
        "        frag_color = texture(tex0, vUV) * vSmoothColor;\n"
        "    else\n"
        "        frag_color = vSmoothColor;\n"
        "}\n";

void MaterialBuilder::create(GLSLVersion glslVersion) {
    if (nullptr == sData) {
        sData = new MaterialBuilder::Data;
        sData->mMaterialCache = new MaterialBuilder::MaterialCache;
        sData->mVersion = glslVersion;
    }
}

void MaterialBuilder::destroy() {
    delete sData;
    sData = nullptr;
}

Material *MaterialBuilder::createBuildinMaterial(VertexType type) {
    MaterialBuilder::MaterialCache *materialCache = sData->mMaterialCache;
    Material *mat = materialCache->find("buildinShaderMaterial");
    if (nullptr != mat) {
        return mat;
    }

    mat = materialCache->create("buildinShaderMaterial", IO::Uri());
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
    Shader *shader = mat->m_shader;
    if (shader != nullptr) {
        if (type == VertexType::ColorVertex) {
            mat->m_shader->addVertexAttributes(ColorVert::getAttributes(), ColorVert::getNumAttributes());
        } else if (type == VertexType::RenderVertex) {
            mat->m_shader->addVertexAttributes(RenderVert::getAttributes(),RenderVert::getNumAttributes());
        }

        addMaterialParameter(mat);
    }

    return mat;
}

RenderBackend::Material *MaterialBuilder::createTexturedMaterial(const String &matName, TextureResourceArray &texResArray,
        RenderBackend::VertexType type) {
    if (matName.empty()) {
        return nullptr;
    }
    MaterialBuilder::MaterialCache *materialCache = sData->mMaterialCache;
    Material *mat = materialCache->find(matName);
    if (mat != nullptr) {
        return mat;
    }

    mat = materialCache->create(matName);
    if (mat == nullptr) {
        return nullptr;
    }

    mat->m_numTextures = texResArray.size();
    mat->m_textures = new Texture *[texResArray.size()];
    for (size_t i = 0; i < texResArray.size(); ++i) {
        TextureResource *texRes = texResArray[i];
        if (texRes == nullptr) {
            continue;
        }
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
            mat->m_shader->addVertexAttributes(ColorVert::getAttributes(), ColorVert::getNumAttributes());
        } else if (type == VertexType::RenderVertex) {
            mat->m_shader->addVertexAttributes(RenderVert::getAttributes(), RenderVert::getNumAttributes());
        }

        addMaterialParameter(mat);
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

    MaterialBuilder::MaterialCache *materialCache = sData->mMaterialCache;
    Material *mat = materialCache->find(matName);
    if (nullptr != mat) {
        return mat;
    }

    mat = materialCache->create(matName);
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

static constexpr c8 DefaultDebugTestMat[] = "debug_text_mat";

Material *MaterialBuilder::createDebugRenderTextMaterial() {
    MaterialBuilder::MaterialCache *materialCache = sData->mMaterialCache;
    Material *mat = materialCache->find(DefaultDebugTestMat);
    if (nullptr != mat) {
        return mat;
    }
    mat = materialCache->create(DefaultDebugTestMat);
    ShaderSourceArray shArray;
    shArray[static_cast<ui32>(ShaderType::SH_VertexShaderType)] = 
            "\n";
    shArray[static_cast<ui32>(ShaderType::SH_FragmentShaderType)] =
            "\n";
    mat->createShader(shArray);

    return mat;
}

} // Namespace RenderBackend
} // namespace OSRE
