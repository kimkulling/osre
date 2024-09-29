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
#include "Animation/AnimatorBase.h"
#include "App/Component.h"

#include <vector>
#include <tuple>

namespace OSRE {
namespace Animation {

struct AnimationTrack {
    f32 Duration;
    f32 mTicksPerSecond;
    size_t NumVectorChannels;
    AnimationChannel *AnimationChannels;

    AnimationTrack() :
            Duration(1.0f), mTicksPerSecond(1.0f), NumVectorChannels(0l), AnimationChannels(nullptr) {
        // empty
    }

    ~AnimationTrack() {
        delete [] AnimationChannels;
        AnimationChannels = nullptr;
    }
};

using AnimationTrackArray = cppcore::TArray<AnimationTrack *>;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief 
//-------------------------------------------------------------------------------------------------
class AnimatorComponent : public App::Component {
public:
    AnimatorComponent(App::Entity *owner);
    ~AnimatorComponent() override;
    void setAnimationTrackArray(AnimationTrackArray &animationTrackArray);
    void addTrack(AnimationTrack *track);
    AnimationTrack *getTrackAt(size_t index) const;
    bool selectTrack(size_t index);
    size_t getActiveTrack() const;

protected:
    bool onUpdate(Time dt) override;
    bool onRender(RenderBackend::RenderBackendService *renderBackendSrv) override;
    void initTransforms();

private:
    AnimationTrackArray mAnimationTrackArray;
    size_t mActiveTrack;
    using TransformArray = cppcore::TArray<glm::mat4>;
    TransformArray mTransformArray;
    std::vector<std::tuple<size_t, size_t, size_t>> mLastPositions;
    double mLastTime;
};

} // namespace Animation
} // namespace OSRE