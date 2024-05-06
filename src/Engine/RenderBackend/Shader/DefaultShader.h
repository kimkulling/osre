#pragma once

#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
    namespace RenderBackend {

        struct DefaultShader {
            String VertexShader;
            String FragmentShader;

            DefaultShader(): VertexShader(), FragmentShader() {
                // eempty
            }

            bool build()  {
                return true;
            }
        }
    }
}
