/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/RenderBackend/Pipeline.h>
#include <osre/Common/osre_common.h>

namespace OSRE {
namespace RenderBackend {

PipelinePass::PipelinePass( Shader *shader )
: m_shader( shader ) {
    // empty
}

PipelinePass::~PipelinePass() {
    // empty
}

void PipelinePass::set( RenderTarget &rt, BlendState &blendState, SamplerState &samplerState, 
                        ClearState &clearState, StencilState &stencilState ) {
    m_renderTarget = rt;
    m_blendState = blendState;
    m_samplerState = samplerState;
    m_clearState = clearState;
    m_stencilState = stencilState;
}

bool PipelinePass::operator == ( const PipelinePass &rhs ) const {
    return ( m_blendState == rhs.m_blendState 
          && m_samplerState == rhs.m_samplerState 
          && m_clearState == rhs.m_clearState
          && m_stencilState == rhs.m_stencilState );
}

bool PipelinePass::operator != ( const PipelinePass &rhs ) const {
    return ! (*this == rhs );
}

Pipeline::Pipeline() 
: m_passes()
, m_passId( -1 )
, m_inFrame( false ) {
    // empty
}

Pipeline::~Pipeline() {
    CPPCore::ContainerClear<PipelinePassArray>( m_passes );
}

void Pipeline::addPass( PipelinePass *pass ) {
    if ( nullptr == pass ) {
        return;
    }

    m_passes.add( pass );
}

ui32 Pipeline::beginFrame() {
    if ( m_inFrame ) {
        return 0;
    }

    if ( m_passes.isEmpty() ) {
        return 0;
    }

    m_inFrame = true;

    return m_passes.size();
}

bool Pipeline::beginPass( ui32 passId ) {
    if ( !m_inFrame ) {
        return false;
    }

    m_passId = passId;

    return true;
}

bool Pipeline::endPass( ui32 passId ) {
    if ( static_cast<i32>( passId ) != m_passId || !m_inFrame ) {
        return false;
    }

    m_passId = -1;
    
    return true;
}

void Pipeline::endFrame() {
    m_inFrame = false;
}

} // Namespace RenderBackend
} // Namespace OSRE
