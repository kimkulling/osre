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

enum RenderPass : ui32 {
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
///	@brief  Describes one pass in a render pipeline.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT PipelinePass {
public:
    PipelinePass(ui32 id, Shader *shader);
    ~PipelinePass();
    void set(RenderTarget &rt, RenderStates &states);
    void setPolygonState(PolygonState polyState);
    PolygonState getPolygonState() const;
    void setCullState(CullState &cullstate);
    CullState getCullState() const;
    void setBlendState(BlendState &blendState);
    const BlendState &getBlendState() const;
    void setSamplerState(SamplerState &samplerState);
    const SamplerState &getSamplerState() const;
    void setClearState(ClearState &clearState);
    const ClearState &getClearState() const;
    void setStencilState(StencilState &stencilState);
    const StencilState &getStencilState() const;
    void setShader(Shader *shader);
    Shader *getShader() const;
    ui32 getId() const;
    static const c8 *getPassNameById(ui32 id);
    bool operator==(const PipelinePass &rhs) const;
    bool operator!=(const PipelinePass &rhs) const;

private:
    ui32 m_id;
    RenderTarget m_renderTarget;
    RenderStates m_states;
    Shader *m_shader;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief Describes a pipeline used for rendering.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Pipeline {
public:
    Pipeline();
    ~Pipeline();
    void addPass(PipelinePass *pass);
    size_t getNumPasses() const;
    size_t beginFrame();
    PipelinePass *beginPass(ui32 passId);
    bool endPass(ui32 passId);
    void endFrame();
    void clear();

private:
    using PipelinePassArray = TArray<PipelinePass *>;
    PipelinePassArray m_passes;
    i32 m_currentPassId;
    bool m_inFrame;
};

} // Namespace RenderBackend
} // Namespace OSRE
