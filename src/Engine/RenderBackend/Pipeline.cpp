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
#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/Pipeline.h>
#include <osre/RenderBackend/RenderBackendService.h>

namespace OSRE {
namespace RenderBackend {

namespace Details {

static const c8 *RenderPassNames[] = {
    "RenderPass",
    "UiPass",
    "DbgPass"
};

} // Namespace Details

constexpr i32 InvalidPassIdx = -1;

RenderPass *RenderPass::create(ui32 id, Shader *shader) {
    return new RenderPass(id, shader);
}

void RenderPass::destroy(RenderPass *plp) {
    if (nullptr != plp) {
        delete plp;
    }
}

RenderPass::RenderPass(ui32 id, Shader *shader) :
        mId(id),
        mRenderTarget(),
        mStates(),
        mShader(shader) {
    // empty
}

RenderPass::~RenderPass() {
    // empty
}

RenderPass &RenderPass::set(RenderTarget &rt, RenderStates &states) {
    mRenderTarget = rt;
    mStates = states;

    return *this;
}

RenderPass &RenderPass::setPolygonState(PolygonState polyState) {
    mStates.m_polygonState = polyState;

    return *this;
}

PolygonState RenderPass::getPolygonState() const {
    return mStates.m_polygonState;
}

RenderPass &RenderPass::setCullState(CullState &cullstate) {
    mStates.m_cullState = cullstate;

    return *this;
}

CullState RenderPass::getCullState() const {
    return mStates.m_cullState;
}

RenderPass &RenderPass::setBlendState(BlendState &blendState) {
    mStates.m_blendState = blendState;

    return *this;
}

const BlendState &RenderPass::getBlendState() const {
    return mStates.m_blendState;
}

RenderPass &RenderPass::setSamplerState(SamplerState &samplerState) {
    mStates.m_samplerState = samplerState;

    return *this;
}

const SamplerState &RenderPass::getSamplerState() const {
    return mStates.m_samplerState;
}

RenderPass &RenderPass::setClearState(ClearState &clearState) {
    mStates.m_clearState = clearState;

    return *this;
}

const ClearState &RenderPass::getClearState() const {
    return mStates.m_clearState;
}

RenderPass &RenderPass::setStencilState(StencilState &stencilState) {
    mStates.m_stencilState = stencilState;

    return *this;
}

const StencilState &RenderPass::getStencilState() const {
    return mStates.m_stencilState;
}

RenderPass &RenderPass::setShader(Shader *shader) {
    mShader = shader;

    return *this;
}

Shader *RenderPass::getShader() const {
    return mShader;
}

ui32 RenderPass::getId() const {
    return mId;
}

const c8 *RenderPass::getPassNameById(ui32 id) {
    if (id >= MaxDbgPasses) {
        return nullptr;
    }

    return Details::RenderPassNames[id];
}

bool RenderPass::operator==(const RenderPass &rhs) const {
    return (mId == rhs.mId && mStates.m_polygonState == rhs.mStates.m_polygonState &&
            mStates.m_cullState == rhs.mStates.m_cullState && mStates.m_blendState == rhs.mStates.m_blendState &&
            mStates.m_samplerState == rhs.mStates.m_samplerState && mStates.m_clearState == rhs.mStates.m_clearState &&
            mStates.m_stencilState == rhs.mStates.m_stencilState);
}

bool RenderPass::operator!=(const RenderPass &rhs) const {
    return !(*this == rhs);
}

Pipeline::Pipeline(const String &pipelineName) :
        Object(pipelineName),
        mPasses(),
        mRbService(),
        mCurrentPassId(InvalidPassIdx),
        mInFrame(false) {
    // empty
}

Pipeline::~Pipeline() {
    for (ui32 i = 0; i < mPasses.size(); ++i) {
        RenderPass::destroy(mPasses[i]);
    }
}

void Pipeline::addPass(RenderPass *pass) {
    if (nullptr == pass) {
        return;
    }

    mPasses.add(pass);
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

RenderPass *Pipeline::beginPass(ui32 passId) {
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

bool Pipeline::endPass(ui32 passId) {
    if (static_cast<i32>(passId) != mCurrentPassId || !mInFrame) {
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

} // Namespace RenderBackend
} // Namespace OSRE
