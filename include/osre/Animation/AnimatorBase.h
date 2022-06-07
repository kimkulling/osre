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
#include <cppcore/Container/TArray.h>

namespace OSRE {

namespace RenderBackend {
    class RenderBackendService;
}

namespace App {
    struct MouseInputState;
}

namespace Animation {

struct OSRE_EXPORT VertexWeight {
    ui32 m_vertexIdx;
    f32 m_vertexWeight;

    bool operator==(const VertexWeight &rhs) {
        return (m_vertexIdx == rhs.m_vertexIdx && m_vertexWeight == rhs.m_vertexWeight);
    }

    bool operator!=(const VertexWeight &rhs) {
        return !(*this == rhs);
    }
};

struct OSRE_EXPORT Bone {
    using VertexWeightArray = CPPCore::TArray<VertexWeight>;

    i32 mParent;
    String mName;
    VertexWeightArray m_vertexWeights;
    glm::mat4 m_offsetMatrix;

    Bone() :
            mParent(-1),
            mName(),
            m_vertexWeights(),
            m_offsetMatrix() {
        // empty
    }

    ~Bone() = default;
};

struct OSRE_EXPORT Skeleton {
    using BoneArray = CPPCore::TArray<Bone *>;

    String mName;
    i32 mRootBone;
    BoneArray mBones;

    Skeleton() {
        // empty
    }

    ~Skeleton() = default;
};

struct VectorKey {
    glm::vec3 mVector;
    d32 mTime;
};

struct RotationKey {
    glm::vec4 mQuad;
    d32 mTime;
};

using VectorKeyArray = ::CPPCore::TArray<VectorKey>;
using RotationKeyArray = ::CPPCore::TArray<RotationKey>;

struct NodeAnimation {
    VectorKeyArray mPositions;
    VectorKeyArray mScalings;
    RotationKeyArray mRotations;
};

struct AnimationTrack {
    String mName;
    double mDuration;
    double mTicksPerSecond;
    CPPCore::TArray<NodeAnimation *> mNodeAnimations;
};

template <class T>
struct AnimatorBase {
    void operator () ( T &out, const T &a, const T &b, f32 d ) const {
        out = a + ( b - a ) * d;
    }
};

enum class TransformCommandType {
    RotateXCommandPositive = 0,
    RotateXCommandNegative,
    RotateYCommandPositive,
    RotateYCommandNegative,
    RotateZCommandPositive,
    RotateZCommandNegative,
    ScaleInCommand,
    ScaleOutCommand,
    TransformCommand,
  
    MaxCommands,
    InvalidCommand
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
    
    /// @brief 
    /// @param mis 
    virtual void getMouseUpdate(const App::MouseInputState &mis) =0;

    /// @brief  The update, override this for your own proposes.
    virtual void update(TransformCommandType cmdType) = 0;

    AnimationControllerBase(const AnimationControllerBase &) = delete;
    AnimationControllerBase &operator=(const AnimationControllerBase &) = delete;

protected:
    AnimationControllerBase() = default;
};

}
}
