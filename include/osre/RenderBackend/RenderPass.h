#pragma once

#include <osre/RenderBackend/RenderStates.h>
#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

static constexpr ui32 RenderPassId = 0;
static constexpr ui32 UiPassId = 1;
static constexpr ui32 DbgPassId = 2;
static constexpr ui32 MaxDbgPasses = 3;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class is used to describes one pass in a render pipeline.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT RenderPass {
public:
    static RenderPass *create(ui32 id, Shader *shader);
    static void destroy(RenderPass *plp);
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
    ui32 getId() const;
    static const c8 *getPassNameById(ui32 id);
    bool operator==(const RenderPass &rhs) const;
    bool operator!=(const RenderPass &rhs) const;

private:
    RenderPass(ui32 id, Shader *shader);
    ~RenderPass();

private:
    ui32 mId;
    RenderTarget mRenderTarget;
    RenderStates mStates;
    Shader *mShader;
};

} // namespace RenderBackend
} // namespace OSRE
