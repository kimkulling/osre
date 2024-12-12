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

#include "Common/osre_common.h"
#include "Platform/KeyTypes.h"

#include <cppcore/Container/TArray.h>

namespace OSRE {

namespace RenderBackend {
    class RenderBackendService;
}

namespace App {
    struct MouseInputState;
}

namespace Animation {

/// @brief
struct OSRE_EXPORT VertexWeight {
    ui32 VertexIndex;
    f32 Weight;

    bool operator==(const VertexWeight &rhs) {
        return (VertexIndex == rhs.VertexIndex && Weight == rhs.Weight);
    }

    bool operator!=(const VertexWeight &rhs) {
        return !(*this == rhs);
    }
};

/// @brief
struct OSRE_EXPORT Bone {
    using VertexWeightArray = cppcore::TArray<VertexWeight>;

    i32 mParent;
    String mName;
    VertexWeightArray m_vertexWeights;
    glm::mat4 m_offsetMatrix;

    /// @brief The default constructor.
    Bone() : mParent(-1), mName(), m_vertexWeights(), m_offsetMatrix() {}

    ///	@brief The default destructor, default implementation.
    ~Bone() = default;
};

/// @brief
struct OSRE_EXPORT Skeleton {
    using BoneArray = cppcore::TArray<Bone *>;

    String mName;
    i32 mRootBone;
    BoneArray mBones;

    /// @brief The default constructor.
    Skeleton() : mName(), mRootBone(-1), mBones(){}

    ///	@brief The default destructor, default implementation.
    ~Skeleton() = default;
};

/// @brief
struct VectorKey {
    f32 Time;
    glm::vec3 Value;

    VectorKey() : Time(1.0f), Value(1) {
        // empty
    }

    ~VectorKey() = default;
};

/// @brief  
using VectorKeyArray = ::cppcore::TArray<VectorKey>;

/// @brief
struct RotationKey {
    d32 Time;
    glm::quat Quad;

    /// @brief
    RotationKey() :
            Time(0.0), Quad() {
        // empty
    }

    /// @brief
    ~RotationKey() = default;
};

using RotationKeyArray = ::cppcore::TArray<RotationKey>;

/// @brief
struct ScalingKey {
    d32 Time;
    glm::vec3 Scale;

    /// @brief
    ScalingKey() :
            Time(0.0), Scale() {
        // empty
    }

    /// @brief
    ~ScalingKey() = default;
};

using ScalingKeyArray = ::cppcore::TArray<ScalingKey>;

/// @brief
struct AnimationChannel {
    VectorKeyArray PositionKeys;
    RotationKeyArray RotationKeys;
    ScalingKeyArray ScalingKeys;
    
    /// @brief
    AnimationChannel() :
            PositionKeys(), RotationKeys(), ScalingKeys() {
        // empty
    }

    /// @brief
    ~AnimationChannel() = default;
};

using VectorChannelArray = ::cppcore::TArray<AnimationChannel>;

/// @brief
template <class T>
struct AnimatorBase {
    void operator () ( T &out, const T &a, const T &b, f32 d ) const {
        out = a + (b - a) * d;
    }
};

/// @brief 
enum class TransformCommandType {
    Invalid = -1,
    RotateXCommandPositive = 0,
    RotateXCommandNegative,
    RotateYCommandPositive,
    RotateYCommandNegative,
    RotateZCommandPositive,
    RotateZCommandNegative,
    ScaleInCommand,
    ScaleOutCommand,
    TransformCommandXPositive,
    TransformCommandXNegative,
    TransformCommandYPositive,
    TransformCommandYNegative,
    TransformCommandZPositive,
    TransformCommandZNegative,
    Count
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief This class declares the basic interface for transformation update controllers.
///        A transform controller will perform different animations for transform blocks.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AnimationControllerBase {
public:
    /// @brief  The class destructor, virtual.
    virtual ~AnimationControllerBase() = default;

    /// @brief Will update the mouse input state.
    /// @param mis   The mouse input.
    virtual void getMouseUpdate(const App::MouseInputState &mis) = 0;

    /// @brief Will return the command code from a key binding.
    /// @param key  The key binding
    /// @return The command code.
    virtual Animation::TransformCommandType getKeyBinding(Platform::Key key) const = 0;

    /// @brief  The update, override this for your own proposes.
    /// @param cmdType   The command type.
    virtual void update(TransformCommandType cmdType) = 0;

    AnimationControllerBase(const AnimationControllerBase &) = delete;
    AnimationControllerBase &operator=(const AnimationControllerBase &) = delete;

protected:
    AnimationControllerBase() = default;
};

} // namespace Animation
} // namespace OSRE
  