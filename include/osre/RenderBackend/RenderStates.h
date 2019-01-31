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

struct ClearState {
    enum class ClearBitType : int {
        ColorBit = 1 << 0,    ///< Clear the color buffer.
        DepthBit = 1 << 1,    ///< Clear the depth buffer.
        StencilBit = 1 << 2     ///< Clear the stencil buffer.
    };

    ui32 m_state;

    /// @brief  The default class constructor.
    ClearState();

    /// @brief  The class constructor with the requested clear states.
    /// @param  states      [in] The requested clear states.
    explicit ClearState(ui32 states);
    bool operator == (const SamplerState &rhs) const;
    bool operator != (const SamplerState &rhs) const;

};

inline
ClearState::ClearState()
: m_state(0) {
    // empty
}

inline
ClearState::ClearState(ui32 state)
: m_state(state) {
    // empty
}

inline
bool ClearState::operator == (const ClearState &rhs) const {
    return m_state == rhs.m_state;
}

inline
bool ClearState::operator != (const ClearState &rhs) const {
    return !(*this == rhs);
}

struct SamplerState {
    TextureTargetType m_targetType;
    TextureStageType  m_stageType;

    SamplerState();
    SamplerState(TextureTargetType targetType, TextureStageType stageType);
    bool operator == (const SamplerState &rhs) const;
    bool operator != (const SamplerState &rhs) const;

};

inline
SamplerState::SamplerState()
: m_targetType(TextureTargetType::Texture2D)
, m_stageType(TextureStageType::TextureStage0) {
    // empty
}

inline
SamplerState::SamplerState(TextureTargetType targetType, TextureStageType stageType)
: m_targetType(targetType)
, m_stageType(stageType) {

    // empty
}

inline
bool SamplerState::operator == (const SamplerState &rhs) const {
    return (m_targetType == rhs.m_targetType
        &&  m_stageType == rhs.m_stageType);
}

inline
bool SamplerState::operator != (const SamplerState &rhs) const {
    return !(*this == rhs);
}

struct StencilState {
public:
    /// @brief  
    enum class StencilFunc {
        Never,
        Always,
        Equal,
        NotEqual,
        Less,
        LEqual,
        GEqual,
        Greater,
        Off
    };

    /// @brief  
    enum class StencilOp {
        Keep,
        Zero,
        Replace,
        Incr,
        IncrWrap,
        Decr,
        DecrWrap,
        Invert
    };

    StencilState();
    ~StencilState();
    void setStencilFunc(StencilFunc, i32 ref, c8 mask);
    StencilFunc getStencilFunc() const;
    i32 getStencilFuncRef() const;
    uc8 getStencilFuncMask() const;
    void setStencilOp(StencilOp sFail, StencilOp dpFail, StencilOp dppass);
    StencilOp getStencilOpSFail() const;
    StencilOp getStencilOpDPFail() const;
    StencilOp getStencilOpDPPass() const;
    bool operator == (const StencilState &rhs) const;
    bool operator != (const StencilState &rhs) const;

private:
    StencilFunc m_stencilFunc;
    i32 m_stencilFuncRef;
    uc8 m_stencilFuncMask;
    StencilOp m_sFail;
    StencilOp m_dpFail;
    StencilOp m_dpPass;
};

inline
StencilState::StencilState()
: m_stencilFunc(StencilFunc::Never)
, m_stencilFuncRef(1)
, m_stencilFuncMask(0xFF)
, m_sFail(StencilOp::Keep)
, m_dpFail(StencilOp::Keep)
, m_dpPass(StencilOp::Keep) {
    // empty
}

inline
StencilState::~StencilState() {
    // empty
}

inline
void StencilState::setStencilFunc(StencilFunc func, i32 ref, c8 mask) {
    m_stencilFunc = func;
    m_stencilFuncRef = ref;
    m_stencilFuncMask = mask;
}

inline
StencilState::StencilFunc StencilState::getStencilFunc() const {
    return m_stencilFunc;
}

inline
i32 StencilState::getStencilFuncRef() const {
    return m_stencilFuncRef;
}

inline
uc8 StencilState::getStencilFuncMask() const {
    return m_stencilFuncMask;
}

inline
void StencilState::setStencilOp(StencilOp sFail, StencilOp dpFail, StencilOp dpPass) {
    m_sFail = sFail;
    m_dpFail = dpFail;
    m_dpPass = dpPass;
}

inline
StencilState::StencilOp StencilState::getStencilOpSFail() const {
    return m_sFail;
}

inline
StencilState::StencilOp StencilState::getStencilOpDPFail() const {
    return m_dpFail;
}

inline
StencilState::StencilOp StencilState::getStencilOpDPPass() const {
    return m_dpPass;
}

inline
bool StencilState::operator == (const StencilState &rhs) const {
    return (m_sFail == rhs.m_sFail && m_dpFail == rhs.m_dpFail && m_dpPass == rhs.m_dpPass);
}

inline
bool StencilState::operator != (const StencilState &rhs) const {
    return !(*this == rhs);
}

struct BlendState {
    /// 
    enum class BlendFunc {
        FuncNone,
        FuncAdd,
        FuncSubstract,
        ReverseSubstract,
        Min,
        Max,
        Off
    };
    BlendFunc m_blendFunc;

    /// @brief  The default constructor.
    BlendState() noexcept;

    /// @brief  The constructor with the func.
    /// @param  func    [in] The new blending function to apply.
    explicit BlendState(BlendFunc func);

    bool operator == (const BlendState &rhs) const;
    bool operator != (const BlendState &rhs) const;

};

inline
BlendState::BlendState() noexcept
: m_blendFunc(BlendFunc::FuncNone) {
    // empty
}

inline
BlendState::BlendState(BlendFunc func)
: m_blendFunc(func) {
    // empty
}

inline
bool BlendState::operator == (const BlendState &rhs) const {
    return m_blendFunc == rhs.m_blendFunc;
}

inline
bool BlendState::operator != (const BlendState &rhs) const {
    return !(*this == rhs);
}

struct CullState {
    /// @brief  The cull-mode, describes direction for a polygon.
    enum class CullMode {
        CW,     ///< Clockwise
        CCW,    ///< Counter-clock wise
        Off     ///< Off
    };

    /// @brief  The cull-face, describes the direction for a valid polygon declaration.
    enum class CullFace {
        Front,          ///< Front polygons will be culled.
        Back,           ///< Back polygons will be culled
        FrontAndBack    ///< Show front an back polygon.
    };

    /// @brief  The default class constructor.
    CullState();

    /// @brief  The class constructor with the cull-mode and the cull face option.
    /// @param  mode        [in] The cull-mode, @see CullMode.
    /// @param  cullFace    [in] The cullFace mode, @see CullFace.
    explicit CullState(CullMode mode, CullFace cullFace);

    /// @brief  The class destructor.
    ~CullState();

    /// The compare operators.
    bool operator == (const CullState &rhs) const;
    bool operator != (const CullState &rhs) const;

private:
    CullMode m_state;
    CullFace m_cullFace;
};

inline
CullState::CullState()
: m_state(CullMode::CW)
, m_cullFace(CullFace::Back) {
    // empty
}

inline
CullState::CullState(CullMode mode, CullFace cullFace)
    : m_state(mode)
    , m_cullFace(cullFace) {
    // empty
}

inline
CullState::~CullState() {
    // empty
}

inline
bool CullState::operator == (const CullState &rhs) const {
    return (m_state == rhs.m_state && m_cullFace == rhs.m_cullFace);
}

inline
bool CullState::operator != (const CullState &rhs) const {
    return !(*this == rhs);
}

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

