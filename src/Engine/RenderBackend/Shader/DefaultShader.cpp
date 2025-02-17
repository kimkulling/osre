/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
