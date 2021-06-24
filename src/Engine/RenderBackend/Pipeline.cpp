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

namespace OSRE {
namespace RenderBackend {

namespace DefaultPipelines {
    const c8 *Pipeline_Default = "pipeline.default";
}

namespace Details {

static const c8 *RenderPassNames[] = {
    "RenderPass",
    "UiPass",
    "DbgPass"
};

} // Namespace Details

constexpr i32 InvalidPassIdx = -1;

PipelinePass *PipelinePass::create(ui32 id, Shader *shader) {
    return new PipelinePass(id, shader);
}

void PipelinePass::destroy(PipelinePass *plp) {
    if (nullptr != plp) {
        delete plp;
    }
}

PipelinePass::PipelinePass(ui32 id, Shader *shader) :
        mId(id),
        mRenderTarget(),
        mStates(),
        mShader(shader) {
    // empty
}

PipelinePass::~PipelinePass() {
    // empty
}

void PipelinePass::set(RenderTarget &rt, RenderStates &states) {
    mRenderTarget = rt;
    mStates = states;
}

void PipelinePass::setPolygonState(PolygonState polyState) {
    mStates.m_polygonState = polyState;
}

PolygonState PipelinePass::getPolygonState() const {
    return mStates.m_polygonState;
}

void PipelinePass::setCullState(CullState &cullstate) {
    mStates.m_cullState = cullstate;
}

CullState PipelinePass::getCullState() const {
    return mStates.m_cullState;
}

void PipelinePass::setBlendState(BlendState &blendState) {
    mStates.m_blendState = blendState;
}

const BlendState &PipelinePass::getBlendState() const {
    return mStates.m_blendState;
}

void PipelinePass::setSamplerState(SamplerState &samplerState) {
    mStates.m_samplerState = samplerState;
}

const SamplerState &PipelinePass::getSamplerState() const {
    return mStates.m_samplerState;
}

void PipelinePass::setClearState(ClearState &clearState) {
    mStates.m_clearState = clearState;
}

const ClearState &PipelinePass::getClearState() const {
    return mStates.m_clearState;
}

void PipelinePass::setStencilState(StencilState &stencilState) {
    mStates.m_stencilState = stencilState;
}

const StencilState &PipelinePass::getStencilState() const {
    return mStates.m_stencilState;
}

void PipelinePass::setShader(Shader *shader) {
    mShader = shader;
}

Shader *PipelinePass::getShader() const {
    return mShader;
}

ui32 PipelinePass::getId() const {
    return mId;
}

const c8 *PipelinePass::getPassNameById(ui32 id) {
    if (id >= MaxDbgPasses) {
        return nullptr;
    }

    return Details::RenderPassNames[id];
}

bool PipelinePass::operator==(const PipelinePass &rhs) const {
    return (mId == rhs.mId && mStates.m_polygonState == rhs.mStates.m_polygonState &&
            mStates.m_cullState == rhs.mStates.m_cullState && mStates.m_blendState == rhs.mStates.m_blendState &&
            mStates.m_samplerState == rhs.mStates.m_samplerState && mStates.m_clearState == rhs.mStates.m_clearState &&
            mStates.m_stencilState == rhs.mStates.m_stencilState);
}

bool PipelinePass::operator!=(const PipelinePass &rhs) const {
    return !(*this == rhs);
}

Pipeline *Pipeline::create(const String &pipelineName) {
    return new Pipeline(pipelineName);
}

void Pipeline::destroy(Pipeline *pl) {
    if (nullptr != pl) {
        delete pl;
    }
}

Pipeline::Pipeline(const String &pipelineName) :
        Object(pipelineName),
        mPasses(),
        mCurrentPassId(InvalidPassIdx),
        mInFrame(false) {
    // empty
}

Pipeline::~Pipeline() {
    for (ui32 i = 0; i < mPasses.size(); ++i) {
        PipelinePass::destroy(mPasses[i]);
    }
}

void Pipeline::addPass(PipelinePass *pass) {
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

PipelinePass *Pipeline::beginPass(ui32 passId) {
    if (!mInFrame) {
        return nullptr;
    }

    mCurrentPassId = passId;
    PipelinePass *pass = mPasses[passId];
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
