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
#pragma once

#include <osre/RenderBackend/RenderStates.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

// Forward declarations
class Shader;

using CPPCore::TArray;

enum RenderPassType : ui32 {
    StaticRenderPass,
    UiRenderPass
};

struct RenderTarget {
    // empty
};

static const ui32 RenderPassId = 0;
static const ui32 UiPassId = 1;
static const ui32 DbgPassId = 2;
static const ui32 MaxDbgPasses = 3;

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


namespace DefaultPipelines {
    static const c8 *Pipeline_Default = "pipeline.default";
}

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief This class is used to describes a pipeline used for rendering. A pipeline contains 
/// passes. Each frame which will be rendered by the pipeline walks through all the passes. So
/// a pipeline could look like:
/// MyPipeline:
///   1.) ClearPass ( will clear all the buffers)
///   2.) Render all static meshes
///   3.) Render special tiles for lightning
///   4.) UI-Rendering 
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Pipeline : public Common::Object {
public:
    /// @brief
    Pipeline(const String &pipelineName);
    ~Pipeline();

    /// @brief
    void addPass(RenderPass *pass);

    /// @brief
    size_t getNumPasses() const;

    /// @brief
    size_t beginFrame();

    /// @brief
    RenderPass *beginPass(ui32 passId);

    /// @brief
    bool endPass(ui32 passId);

    /// @brief
    void endFrame();

    /// @brief
    void clear();

private:

private:
    using PipelinePassArray = TArray<RenderPass*>;
    PipelinePassArray mPasses;
    i32 mCurrentPassId;
    bool mInFrame;
};

} // Namespace RenderBackend
} // Namespace OSRE
