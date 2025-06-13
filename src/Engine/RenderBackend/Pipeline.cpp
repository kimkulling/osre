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
#include "RenderBackend/Pipeline.h"
#include "Common/osre_common.h"
#include "RenderBackend/RenderBackendService.h"

namespace OSRE::RenderBackend {

namespace DefaultPipelines {

const c8 *get3DPipelineDefault() {
    static constexpr c8 Name[] = "pipeline.default.3d";
    return Name;
}

const c8* get2DPipelineDefault() {
    static constexpr c8 Name[] = "pipeline.default.2d";
    return Name;
}

}

constexpr guid InvalidPassIdx = 99999999u;

Pipeline::Pipeline(const String &pipelineName) :
        Object(pipelineName),
        mRbService(),
        mCurrentPassId(InvalidPassIdx),
        mInFrame(false) {
    // empty
}

Pipeline::~Pipeline() {
    clear();
}

void Pipeline::addPass(RenderPass *pass) {
    if (nullptr == pass) {
        return;
    }

    mPasses.add(pass);
}

RenderPass *Pipeline::getPassById(guid passId) const {
    for (size_t i=0; i<mPasses.size(); i++) {
        if (passId == mPasses[i]->getId()) {
            return mPasses[i];
        }
    }
    
    return nullptr;
}

size_t Pipeline::getNumPasses() const {
    return mPasses.size();
}

size_t Pipeline::beginFrame() {
    if (mInFrame) {
        return 0L;
    }

    if (mPasses.isEmpty()) {
        return 0L;
    }

    mInFrame = true;

    return mPasses.size();
}

RenderPass *Pipeline::beginPass(guid passId) {
    if (!mInFrame) {
        return nullptr;
    }

    mCurrentPassId = passId;
    RenderPass *pass = mPasses[passId];
    if (nullptr == pass) {
        return nullptr;
    }

    return pass;
}

bool Pipeline::endPass(guid passId) {
    if (static_cast<ui32>(passId) != mCurrentPassId || !mInFrame) {
        return false;
    }

    mCurrentPassId = InvalidPassIdx;

    return true;
}

void Pipeline::endFrame() {
    mInFrame = false;
}

void Pipeline::clear() {
    mCurrentPassId = InvalidPassIdx;
    mInFrame = false;
    mPasses.resize(0);
}

void Pipeline::resizeRenderTargets(guid id, ui32 x, ui32 y, ui32 w, ui32 h) {
    for (size_t passId=0; passId<mPasses.size(); passId++) {
        RenderPass *pass = mPasses[passId];
        if (pass == nullptr) {
            continue;
        }

        if (pass->getFrameBufferId() == id) {
            pass->setViewport(x, y, w, h);
        }
    }
}

} // Namespace OSRE::RenderBackend
