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
#pragma once

#include "Animation/AnimatorBase.h"
#include "Platform/PlatformCommon.h"
#include "Animation/AnimatorBase.h"
#include <cppcore/Container/TStaticArray.h>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace RenderBackend {
    struct TransformMatrixBlock;
}

namespace App {

/// @brief The class to manage the keyboard mappings for transform commands.
struct KeyboardMap {
    cppcore::TStaticArray<Animation::TransformCommandType, Platform::KEY_LAST> KeyArray;

    /// @brief The class constructor.
    KeyboardMap() {
        init();
    }

    /// @brief The class destructor.
    ~KeyboardMap() = default;

    /// @brief Will initialize the array, no mapping is stored.
    void init() {
        for (size_t i=0; i<Platform::KEY_LAST; ++i) {
            KeyArray[i] = Animation::TransformCommandType::Invalid;
        }
    }

    /// @brief Enables the default mapping.
    void setDefault() {
        KeyArray[Platform::KEY_A] = Animation::TransformCommandType::RotateXCommandPositive;
        KeyArray[Platform::KEY_a] = Animation::TransformCommandType::RotateXCommandPositive;
        KeyArray[Platform::KEY_D] = Animation::TransformCommandType::RotateXCommandNegative;
        KeyArray[Platform::KEY_d] = Animation::TransformCommandType::RotateXCommandNegative;
        KeyArray[Platform::KEY_W] = Animation::TransformCommandType::RotateYCommandPositive;
        KeyArray[Platform::KEY_w] = Animation::TransformCommandType::RotateYCommandPositive;
        KeyArray[Platform::KEY_S] = Animation::TransformCommandType::RotateYCommandNegative;
        KeyArray[Platform::KEY_s] = Animation::TransformCommandType::RotateYCommandNegative;
        KeyArray[Platform::KEY_Q] = Animation::TransformCommandType::RotateZCommandPositive;
        KeyArray[Platform::KEY_q] = Animation::TransformCommandType::RotateZCommandPositive;
        KeyArray[Platform::KEY_E] = Animation::TransformCommandType::RotateZCommandNegative;
        KeyArray[Platform::KEY_e] = Animation::TransformCommandType::RotateZCommandNegative;
        KeyArray[Platform::KEY_PLUS] = Animation::TransformCommandType::ScaleInCommand;
        KeyArray[Platform::KEY_MINUS] = Animation::TransformCommandType::ScaleOutCommand;
    }

    /// @brief Set a new mapping.
    /// @param[in] key   The key.
    /// @param[in] type  The mapped transform command.
    void set(Platform::Key key, Animation::TransformCommandType type) {
        KeyArray[key] = type; 
    }

    /// @brief Get the actieve mapping.
    /// @param[in] key   The key.
    /// @return The mapped command.
    Animation::TransformCommandType get(Platform::Key key) const {
        return KeyArray[key];
    }
};

// Forward declarations ---------------------------------------------------------------------------
struct MouseInputState;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the default keyboard controlling.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT TransformController : public Animation::AnimationControllerBase {
public:
    /// @brief The class constructor.
    /// @param tmb  The transform matrix block to control.
    TransformController(RenderBackend::TransformMatrixBlock &tmb);

    ///	@brief The class destructor.
    ~TransformController() override = default;

    /// @brief Will return the command code from a key binding.
    /// @param key  The key binding
    /// @return The command code.
    Animation::TransformCommandType getKeyBinding(Platform::Key key) const override;

    /// @brief Will perform the mouse update.
    /// @param mis  The mouse input state.
    void getMouseUpdate(const MouseInputState &mis) override;

    /// @brief Will update the transformation.
    /// @param cmdType  The command.
    void update(Animation::TransformCommandType cmdType) override;

private:
    KeyboardMap mKeyboardMap;
    RenderBackend::TransformMatrixBlock &mTransform;
};

} // namespace App
} // namespace OSRE
