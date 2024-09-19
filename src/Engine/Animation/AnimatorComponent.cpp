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
#include "Animation/AnimatorComponent.h"

namespace OSRE{
namespace Animation {

using namespace OSRE::App;

AnimatorComponent::AnimatorComponent(Entity *owner, ComponentType type) :
        Component(owner, type),
        mAnimationTrackArray(),
        mActiveTrack() {
    // empty
}

AnimatorComponent::~AnimatorComponent() {
    // empty
}

void AnimatorComponent::addTrack(AnimationTrack *track) {
    if (track == nullptr) {
        return;
    }

    mAnimationTrackArray.add(track);
}

AnimationTrack *AnimatorComponent::getTrackAt(size_t index) const {
    if (index >= mAnimationTrackArray.size()) {
        return nullptr;
    }

    return mAnimationTrackArray[index];
}

bool AnimatorComponent::selectTrack(size_t index) {
    if (index >= mAnimationTrackArray.size()) {
        return false;
    }

    mActiveTrack = index;

    return true;
}

size_t AnimatorComponent::getActiveTrack() const {
    return mActiveTrack;
}

bool AnimatorComponent::onUpdate(Time dt) {
    return true;
}

bool AnimatorComponent::onRender(RenderBackend::RenderBackendService *renderBackendSrv) {
    osre_assert(renderBackendSrv != nullptr);

    return true;
}

} // namespace Animation
} // namespace OSRE
