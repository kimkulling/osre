#pragma once

#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace RenderBackend {

struct DefaultShaderBuilder {
    String VertexShader;
    String FragmentShader;

    DefaultShaderBuilder() :
            VertexShader(), FragmentShader() {
        // empty
    }

    bool build() {
        return true;
    }
};

struct R2DShaderBuilder {
    String VertexShader;
    String FragmentShader;

    R2DShaderBuilder() :
            VertexShader(), FragmentShader() {
        // empty
    }

    bool build() {
    }
};

}
}