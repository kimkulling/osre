#include "RenderBackend/Shader/DefaultShader.h"

namespace OSRE {
namespace RenderBackend {

String getGLSLVersionString_330()  {
    static const String GLSLVersionString_330 =
            "#version 330 core\n";
    return GLSLVersionString_330;
}

String  getGLSLVersionString_400() {
    static const String GLSLVersionString_400 =
            "#version 400 core\n";
    return GLSLVersionString_400;
}

String getNewLine() {
    static String NewLine = "\n";
    return NewLine;
}

String getGLSLRenderVertexLayout() {
    static const String GLSLRenderVertexLayout =
            "// RenderVertex layout\n"
            "layout(location = 0) in vec3 position;	  // object space vertex position\n"
            "layout(location = 1) in vec3 normal;	  // object space vertex normal\n"
            "layout(location = 2) in vec3 color0;     // per-vertex diffuse colour\n"
            "layout(location = 3) in vec2 texcoord0;  // per-vertex tex coord, stage 0\n" +
            getNewLine();
    return GLSLRenderVertexLayout;
}

String getGLSLColorVertexLayout() {
    static const String GLSLColorVertexLayout =
        "// Colorvertex layout\n"
        "layout(location = 0) in vec3 position;	 // object space vertex position\n"
        "layout(location = 1) in vec3 normal;    // object space vertex normal\n"
        "layout(location = 2) in vec3 color0;    // per-vertex colour\n" +
        getNewLine();
    return GLSLColorVertexLayout;
}

String getGLSLCombinedMVPUniformSrc() {
    static const String GLSLCombinedMVPUniformSrc =
            "// uniforms\n"
            "uniform mat4 Model;\n"
            "uniform mat4 View;\n"
            "uniform mat4 Projection;\n";
    return GLSLCombinedMVPUniformSrc;
}

}
}
