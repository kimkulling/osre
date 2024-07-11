#pragma once

#include "Common/osre_common.h"
#include "RenderBackend/RenderCommon.h"

namespace OSRE {
namespace RenderBackend {

String getGLSLVersionString_330();
String  getGLSLVersionString_400();
String getNewLine();
String getGLSLRenderVertexLayout();
String getGLSLColorVertexLayout();
String getGLSLCombinedMVPUniformSrc();

struct DefaultShader {
    String VertexShader;
    String FragmentShader;

    DefaultShader(): VertexShader(), FragmentShader() {
        // eempty
    }

    bool build()  {
        return true;
    }
};

}
}
