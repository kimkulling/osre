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
#include "RenderBackend/RenderPass.h"
#include "RenderBackend/2D/RenderPass2D.h"
namespace OSRE {
namespace RenderBackend {


namespace Details {

static const c8 *RenderPassNames[] = {
    "RenderPass",
    "2D",
    "DbgPass"
};

static void initRenderPasses() {
    RenderPassFactory::registerPass(RenderPassId, new RenderPass(RenderPassId, nullptr));
    RenderPassFactory::registerPass(UiPassId, RenderPass2D::build(UiPassId));
    RenderPassFactory::registerPass(DbgPassId, new RenderPass(DbgPassId, nullptr));
}

} // Namespace Details

cppcore::TArray<RenderPass*> RenderPassFactory::sPasses;

RenderPass *RenderPassFactory::create(guid id) {
    if (sPasses.isEmpty()) {
        Details::initRenderPasses();
    }
    for (size_t i = 0; i < sPasses.size(); ++i) {
        if (sPasses[i]->getId() == id) {
            return sPasses[i];
        }
    }

    return nullptr;
}

void RenderPassFactory::registerPass(guid id, RenderPass *renderPass) {
    if (renderPass == nullptr) {
        return;
    }

    for (size_t i = 0; i < sPasses.size(); ++i) {
        if (sPasses[i]->getId() == id) {
            return;
        }
    }
    sPasses.add(renderPass);
}

void RenderPassFactory::unregisterPass(guid id ) {
    for (size_t i = 0; i < sPasses.size(); ++i) {
        if (sPasses[i]->getId() == id) {
            sPasses.remove(i);
            break;
        }
    }
}

RenderPass::RenderPass(guid id, Shader *shader) :
        mId(id),
        mViewport(),
        mRenderTarget(),
        mStates(),
        mShader(shader) {
    // empty
}

void RenderPass::setViewport(const Viewport& viewport) {
    mViewport = viewport;
}

const Viewport& RenderPass::getViewport() const {
    return mViewport;
}

RenderPass &RenderPass::set(RenderTarget &rt, RenderStates &states) {
    mRenderTarget = rt;
    mStates = states;

    return *this;
}

RenderPass &RenderPass::setPolygonState(PolygonState polyState) {
    mStates.polygonState = polyState;

    return *this;
}

PolygonState RenderPass::getPolygonState() const {
    return mStates.polygonState;
}

RenderPass &RenderPass::setCullState(CullState &cullstate) {
    mStates.cullState = cullstate;

    return *this;
}

CullState RenderPass::getCullState() const {
    return mStates.cullState;
}

RenderPass &RenderPass::setBlendState(BlendState &blendState) {
    mStates.blendState = blendState;

    return *this;
}

const BlendState &RenderPass::getBlendState() const {
    return mStates.blendState;
}

RenderPass &RenderPass::setSamplerState(SamplerState &samplerState) {
    mStates.samplerState = samplerState;

    return *this;
}

const SamplerState &RenderPass::getSamplerState() const {
    return mStates.samplerState;
}

RenderPass &RenderPass::setClearState(ClearState &clearState) {
    mStates.clearState = clearState;

    return *this;
}

const ClearState &RenderPass::getClearState() const {
    return mStates.clearState;
}

RenderPass &RenderPass::setStencilState(StencilState &stencilState) {
    mStates.stencilState = stencilState;

    return *this;
}

const StencilState &RenderPass::getStencilState() const {
    return mStates.stencilState;
}

RenderPass &RenderPass::setShader(Shader *shader) {
    mShader = shader;

    return *this;
}

Shader *RenderPass::getShader() const {
    return mShader;
}

const c8 *RenderPass::getPassNameById(guid id) {
    if (id >= MaxDbgPasses) {
        return nullptr;
    }

    return Details::RenderPassNames[id];
}

bool RenderPass::operator == (const RenderPass &rhs) const {
    return (mId == rhs.mId && mStates.polygonState == rhs.mStates.polygonState &&
            mStates.cullState == rhs.mStates.cullState && mStates.blendState == rhs.mStates.blendState &&
            mStates.samplerState == rhs.mStates.samplerState && mStates.clearState == rhs.mStates.clearState &&
            mStates.stencilState == rhs.mStates.stencilState);
}

bool RenderPass::operator != (const RenderPass &rhs) const {
    return !(*this == rhs);
}

} // namespace RenderBackend
} // namespace OSRE
