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
#pragma once

#include <osre/Common/osre_common.h>
#include <cppcore/Common/TBitField.h>

// Forward declarations ---------------------------------------------------------------------------
struct aiScene;
struct aiMesh;
struct aiNode;
struct aiMaterial;
struct aiAnimation;

namespace OSRE {

namespace RenderBackend {
    class Pipeline;
    class RenderBackendService;
}

namespace IO {
    class IOService;
}

namespace App {

class MouseEventListener;
class KeyboardEventListener;

template<class T> class TAbstractCtrlBase;

/// @brief  Describes the requested render API for the backend.
enum class RenderBackendType {
    OpenGLRenderBackend = 0,    ///< OpenGL render API.
    VulkanRenderBackend         ///< Vulkan render API.
};

struct MouseInputState {
    i32 mRelX, mRelY, mAbsX, mAbsY;
    ui32 mLastX, mLastY;
    CPPCore::TBitField<ui32> mMouseButtonState;

    MouseInputState();
};

inline MouseInputState::MouseInputState() :
        mRelX(0), mRelY(0), mAbsX(0), mAbsY(0), mLastX(0), mLastY(0), mMouseButtonState() {
    // empty
}

} // Namespace App
} // Namespace OSRE
