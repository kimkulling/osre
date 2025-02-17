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
#include "Animation/AnimatorComponent.h"
#include "Common/Logger.h"

namespace OSRE{
namespace Animation {

using namespace OSRE::App;

static constexpr c8 Tag[] = "AnimatorComponent";

AnimatorComponent::AnimatorComponent(Entity *owner) :
        Component(owner, ComponentType::AnimationComponentType),
        mAnimationTrackArray(),
        mActiveTrack(),
        mTransformArray(),
        mLastPositions(),
        mLastRotations(),
        mLastScales(),
        mLastTime(0.0) {
    // empty
}

void AnimatorComponent::addTrack(AnimationTrack *track) {
    if (track == nullptr) {
        osre_error(Tag, "Invalid animation track instance.");
        return;
    }

    const size_t lastTrack = mAnimationTrackArray.size();
    mAnimationTrackArray.add(track);
    mTransformArray.add(glm::mat4(1.0f));
    selectTrack(lastTrack);
}

AnimationTrack *AnimatorComponent::createAnimation() {
    AnimationTrack *track = new AnimationTrack;
    addTrack(track);

    return track;    
}

AnimationTrack *AnimatorComponent::getTrackAt(size_t index) const {
    if (index >= mAnimationTrackArray.size()) {
        osre_error(Tag, "Invalid animation track index.");
        return nullptr;
    }

    return mAnimationTrackArray[index];
}

bool AnimatorComponent::selectTrack(size_t index) {
    if (index >= mAnimationTrackArray.size()) {
        osre_error(Tag, "Invalid animation track index.");
        return false;
    }

    mActiveTrack = index;

    return true;
}

size_t AnimatorComponent::getActiveTrack() const {
    return mActiveTrack;
}

bool AnimatorComponent::onUpdate(Time dt) {
    AnimationTrack *track = getTrackAt(mActiveTrack);
    if (track == nullptr) {
        return true;
    }

    // calculate the time
    double time = static_cast<d32>(dt.asMilliSeconds()) / 1000.0f;
    const double ticksPerSecond = track->mTicksPerSecond != 0.0 ? track->mTicksPerSecond : 25.0;
    
    // every following time calculation happens in ticks
    time *= ticksPerSecond;

    // map into animation track duration
    if (track->Duration > 0.0) {
        time = fmod(time, track->Duration);
    }

    const size_t currentAnimationTrack = getActiveTrack();

    AnimationChannel &animChannel = track->AnimationChannels[currentAnimationTrack];
    glm::vec3 presentPosition(0, 0, 0);
    glm::quat q(0, 0, 0, 1);
    glm::vec3 presenceScale(1, 1, 1);
    if (!animChannel.PositionKeys.isEmpty()) {
        if (mLastPositions.empty()) {
            mLastPositions.resize(animChannel.PositionKeys.size());
        }

        for (size_t keyIndex = 0; keyIndex < animChannel.PositionKeys.size(); ++keyIndex) {
                
            size_t animFrameIndex = (time > mLastTime) ? std::get<0>(mLastPositions[keyIndex]) : 0;
            while (animFrameIndex < animChannel.PositionKeys.size() - 1) {
                if (time < animChannel.PositionKeys[animFrameIndex + 1].Time) {
                    break;
                }
                ++animFrameIndex;
            }
            size_t nextFrame = (animFrameIndex + 1) % animChannel.PositionKeys.size();
            VectorKey &key = animChannel.PositionKeys[keyIndex];
            VectorKey &nextKey = animChannel.PositionKeys[nextFrame];
            double diffTime = nextKey.Time - key.Time;
            if (diffTime < 0.0) {
                diffTime += track->Duration;
            }
            if (diffTime > 0) {
                float factor = float((time - key.Time) / diffTime);
                presentPosition = key.Value + (nextKey.Value - key.Value) * factor;
            } else {
                presentPosition = key.Value;
            }

            std::get<0>(mLastPositions[keyIndex]) = animFrameIndex;
        }   
    }

    if (!animChannel.RotationKeys.isEmpty()) {
        if (mLastRotations.empty()) {
            mLastRotations.resize(animChannel.RotationKeys.size());
        }

        for (size_t keyIndex = 0; keyIndex < animChannel.RotationKeys.size(); ++keyIndex) {
            size_t animFrameIndex = (time > mLastTime) ? std::get<0>(mLastRotations[keyIndex]) : 0;
            while (animFrameIndex < animChannel.RotationKeys.size() - 1) {
                if (time < animChannel.RotationKeys[animFrameIndex + 1].Time) {
                    break;
                }
                ++animFrameIndex;
            }
            size_t nextFrame = (animFrameIndex + 1) % animChannel.RotationKeys.size();
            RotationKey &key = animChannel.RotationKeys[keyIndex];
            RotationKey &nextKey = animChannel.RotationKeys[nextFrame];
            double diffTime = nextKey.Time - key.Time;
            if (diffTime < 0.0) {
                diffTime += track->Duration;
            }
            if (diffTime > 0) {
                f32 factor = f32((time - key.Time) / diffTime);
                q = key.Quad + (nextKey.Quad - key.Quad) * factor;
            } else {
                q = key.Quad;
            }
            std::get<0>(mLastRotations[keyIndex]) = animFrameIndex;
        }
    }

    if (!animChannel.ScalingKeys.isEmpty()) {
        if (mLastScales.empty()) {
            mLastScales.resize(animChannel.ScalingKeys.size());
        }

        for (size_t keyIndex = 0; keyIndex < animChannel.ScalingKeys.size(); ++keyIndex) {
            size_t animFrameIndex = (time > mLastTime) ? std::get<0>(mLastScales[keyIndex]) : 0;
            while (animFrameIndex < animChannel.ScalingKeys.size() - 1) {
                if (time < animChannel.ScalingKeys[animFrameIndex + 1].Time) {
                    break;
                }
                ++animFrameIndex;
            }
            size_t nextFrame = (animFrameIndex + 1) % animChannel.RotationKeys.size();
            ScalingKey &key = animChannel.ScalingKeys[keyIndex];
            ScalingKey &nextKey = animChannel.ScalingKeys[nextFrame];
            double diffTime = nextKey.Time - key.Time;
            if (diffTime < 0.0) {
                diffTime += track->Duration;
            }
            if (diffTime > 0) {
                f32 factor = f32((time - key.Time) / diffTime);
                presenceScale = key.Scale + (nextKey.Scale - key.Scale) * factor;
            } else {
                presenceScale = key.Scale;
            }
            std::get<0>(mLastScales[keyIndex]) = animFrameIndex;
        }
    }

    glm::mat4 &transform = mTransformArray[currentAnimationTrack];
    transform = glm::toMat4(q);
    transform = glm::scale(transform, presenceScale);
    transform = glm::translate(transform, presentPosition);

    return true;
}

bool AnimatorComponent::onRender(RenderBackend::RenderBackendService *renderBackendSrv) {
    osre_assert(renderBackendSrv != nullptr);

    return true;
}

void AnimatorComponent::initAnimations() {
    for (size_t index = 0; index < mAnimationTrackArray.size(); ++index) {
        mTransformArray[index] = glm::mat4(1.0f);
    }
}

} // namespace Animation
} // namespace OSRE
