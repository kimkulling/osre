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

#include <osre/RenderBackend/RenderStates.h>
#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

class RenderPass;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class is used to describes one pass in a render pipeline.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT RenderPassFactory {
public:
    static RenderPass *create(guid id);
    static void registerPass(guid id, RenderPass *renderPass);
    static void unregisterPass(guid id);

private:
    static cppcore::TArray<RenderPass*> sPasses;
};


static constexpr ui32 RenderPassId = 0;
static constexpr ui32 UiPassId = 1;
static constexpr ui32 DbgPassId = 2;
static constexpr ui32 MaxDbgPasses = 3;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class is used to describe a dedicated render pass.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT RenderPass {
public:
    RenderPass() = default;
    RenderPass(guid id, Shader *shader);
    ~RenderPass() = default;
    void setViewProjection(const glm::mat4 &view, const glm::mat4 &projection);
    const glm::mat4 &getView() const;
    const glm::mat4 &getProjection() const;
    void setViewport(const Viewport &viewport);
    const Viewport &getViewport() const;
    RenderPass &set(RenderTarget &rt, RenderStates &states);
    RenderPass &setPolygonState(PolygonState polyState);
    PolygonState getPolygonState() const;
    RenderPass &setCullState(CullState &cullstate);
    CullState getCullState() const;
    RenderPass &setBlendState(BlendState &blendState);
    const BlendState &getBlendState() const;
    RenderPass &setSamplerState(SamplerState &samplerState);
    const SamplerState &getSamplerState() const;
    RenderPass &setClearState(ClearState &clearState);
    const ClearState &getClearState() const;
    RenderPass &setStencilState(StencilState &stencilState);
    const StencilState &getStencilState() const;
    RenderPass &setShader(Shader *shader);
    Shader *getShader() const;
    guid getId() const;
    static const c8 *getPassNameById(guid id);
    bool operator == (const RenderPass &rhs) const;
    bool operator != (const RenderPass &rhs) const;

private:
    guid mId;
    glm::mat4 mProjection;
    glm::mat4 mView;
    Viewport mViewport;
    RenderTarget mRenderTarget;
    RenderStates mStates;
    Shader *mShader;
};

inline void RenderPass::setViewProjection(const glm::mat4& view, const glm::mat4& projection) {
    mView = view;
    mProjection = projection;
}

inline const glm::mat4& RenderPass::getView() const {
    return mView;
}

inline const glm::mat4& RenderPass::getProjection() const {
    return mProjection;
}

inline guid RenderPass::getId() const {
    return mId;
}

} // namespace RenderBackend
} // namespace OSRE
