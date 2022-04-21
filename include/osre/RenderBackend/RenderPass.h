#pragma once

#include <osre/RenderBackend/RenderStates.h>
#include <cppcore/Container/TArray.h>


namespace OSRE {
namespace RenderBackend {

class RenderPass;


class OSRE_EXPORT RenderPassFactory {
public:
    static RenderPass *create(ui32 id);
    static void registerPass(ui32 id, RenderPass *renderPass);
    static void unregisterPass(ui32 id);

private:
    static CPPCore::TArray<RenderPass*> sPasses;
};


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
    RenderPass(ui32 id, Shader *shader);
    ~RenderPass() = default;
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
    ui32 mId;
    RenderTarget mRenderTarget;
    RenderStates mStates;
    Shader *mShader;
};

inline ui32 RenderPass::getId() const {
    return mId;
}

} // namespace RenderBackend
} // namespace OSRE
