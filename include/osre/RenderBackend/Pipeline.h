/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/RenderBackend/BlendState.h>
#include <osre/RenderBackend/SamplerState.h>
#include <osre/RenderBackend/ClearState.h>
#include <osre/RenderBackend/StencilState.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace RenderBackend {

struct Shader;

using CPPCore::TArray;

struct RenderTarget {
    // empty
};

class PipelinePass {
public:
    PipelinePass( Shader *shader );
    ~PipelinePass();
    void set( RenderTarget &rt, BlendState &belndState, SamplerState &samplerState, ClearState &clearState, StencilState &stencilState );
    bool operator == ( const PipelinePass &rhs ) const;
    bool operator != ( const PipelinePass &rhs ) const;

private:
    RenderTarget m_renderTarget;
    BlendState m_blendState;
    SamplerState m_samplerState;
    ClearState m_clearState;
    StencilState m_stencilState;
    Shader *m_shader;
};

class Pipeline {
public:
    Pipeline();
    ~Pipeline();
    void addPass( PipelinePass *pass );
    ui32 beginFrame();
    bool beginPass( ui32 passId );
    bool endPass( ui32 passId );
    void endFrame();

private:
    typedef TArray<PipelinePass*> PipelinePassArray;
    PipelinePassArray m_passes;
    i32 m_passId;
    bool m_inFrame;
};

}
}
