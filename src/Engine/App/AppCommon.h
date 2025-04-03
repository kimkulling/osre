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
#pragma once

#include "Common/osre_common.h"
#include "Platform/KeyTypes.h"

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
class CameraComponent;
class TransformComponent;

template<class T> class TAbstractCtrlBase;

/// @brief  Describes the requested render API for the backend.
enum class RenderBackendType {
    Invalid = -1,               ///< Invalid render API.
    OpenGLRenderBackend = 0,    ///< OpenGL render API.
    VulkanRenderBackend,        ///< Vulkan render API.
    Count                       ///< Number of render APIs.
};

/// @brief A container struct to store all the mouse input states.
struct MouseInputState {
    i32 RelX, RelY, AbsX, AbsY;
    ui32 LastX, LastY;
    cppcore::TBitField<ui32> MouseButtonState;

    MouseInputState();
};

inline MouseInputState::MouseInputState() :
        RelX(0), RelY(0), AbsX(0), AbsY(0), LastX(0), LastY(0), MouseButtonState() {
    // empty
}

/// @brief A container struct to store all the keyboard input states.
struct KeyboardInputState {
    Platform::Key mLast = Platform::KEY_UNKNOWN;
    char mKeymap[Platform::KEY_LAST];

    KeyboardInputState() {
        clear();
    }

    void clear() {
        memset(mKeymap, 0, sizeof(char) * Platform::KEY_LAST);
    }
};

} // Namespace App
} // Namespace OSRE
