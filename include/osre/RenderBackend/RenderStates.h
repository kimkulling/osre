/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/ClearState.h>
#include <osre/RenderBackend/BlendState.h>
#include <osre/RenderBackend/CullState.h>
#include <osre/RenderBackend/SamplerState.h>
#include <osre/RenderBackend/StencilState.h>

namespace OSRE {
namespace RenderBackend {

struct RenderStates {
    ClearState     m_clearState;
    TransformState m_transformState;
    PolygonState   m_polygonState;
    BlendState     m_blendState;
    CullState      m_cullState;
    SamplerState   m_samplerState;
    StencilState   m_stencilState;
    bool           m_applied;

    RenderStates()
    : m_clearState()
    , m_transformState()
    , m_polygonState()
    , m_blendState()
    , m_cullState()
    , m_samplerState()
    , m_stencilState()
    , m_applied( false ) {
        // empty
    }

    bool isEqual( const ClearState &ClearState, const TransformState &transformState, const PolygonState &polygonState, const CullState &cullstate, const BlendState &blendState,
        const SamplerState &samplerState, const StencilState &stencilState ) const {
        return ( ClearState == m_clearState
            && transformState == m_transformState
            && polygonState == m_polygonState
            && blendState == m_blendState
            && cullstate == m_cullState
            && samplerState == m_samplerState
            && stencilState == m_stencilState );
    }
};

} // Namespace RenderBackend
} // Namespace OSRE

