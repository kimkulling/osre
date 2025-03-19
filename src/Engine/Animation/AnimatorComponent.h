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
#include "Animation/AnimatorBase.h"
#include "App/Component.h"

#include <vector>
#include <tuple>

namespace OSRE {
namespace Animation {

/// @brief  
struct AnimationTrack {
    f32 duration;
    f32 ticksPerSecond;
    size_t numVectorChannels;
    AnimationChannel *animationChannels;

    AnimationTrack() :
            duration(1.0f), ticksPerSecond(1.0f), numVectorChannels(0l), animationChannels(nullptr) {
        // empty
    }

    ~AnimationTrack() {
        delete [] animationChannels;
    }
};

using AnimationTrackArray = cppcore::TArray<AnimationTrack *>;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief This class implements the animation component. 
/// 
/// As a user you can add single animation tracks to the component. When applying an animation you
/// first need to choose the track. This active animation track will be selected and updated.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AnimatorComponent : public App::Component {
    using TransformArray = cppcore::TArray<glm::mat4>;

public:
    AnimatorComponent(App::Entity *owner);
    ~AnimatorComponent() override = default;
    void addTrack(AnimationTrack *track);
    AnimationTrack *createAnimation();
    AnimationTrack *getTrackAt(size_t index) const;
    bool selectTrack(size_t index);
    size_t getActiveTrack() const;

protected:
    bool onUpdate(Time dt) override;
    bool onRender(RenderBackend::RenderBackendService *renderBackendSrv) override;
    void initAnimations();

private:
    AnimationTrackArray mAnimationTrackArray;
    size_t mActiveTrack;
    TransformArray mTransformArray;
    std::vector<std::tuple<size_t, size_t, size_t>> mLastPositions;
    std::vector<std::tuple<size_t, size_t, size_t>> mLastRotations;
    std::vector<std::tuple<size_t, size_t, size_t>> mLastScales;
    double mLastTime;
};

} // namespace Animation
} // namespace OSRE
