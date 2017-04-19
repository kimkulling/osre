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

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace RenderBackend {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class StencilState {
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
        Greater
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

public:
    StencilState();
    ~StencilState();
    void setStencilFunc( StencilFunc, i32 ref, c8 mask );
    StencilFunc getStencilFunc() const;
    i32 getStencilFuncRef() const;
    uc8 getStencilFuncMask() const;
    void setStencilOp( StencilOp sFail, StencilOp dpFail, StencilOp dppass );
    StencilOp getStencilOpSFail() const;
    StencilOp getStencilOpDPFail() const;
    StencilOp getStencilOpDPPass() const;
    bool operator == ( const StencilState &rhs ) const;
    bool operator != ( const StencilState &rhs ) const;

    // Avoid copying 
    StencilState( const StencilState & ) = delete;
    StencilState& operator = ( const StencilState & ) = delete;

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
: m_stencilFunc( StencilFunc::Never )
, m_stencilFuncRef( 1 )
, m_stencilFuncMask( 0xFF )
, m_sFail( StencilOp::Keep )
, m_dpFail( StencilOp::Keep )
, m_dpPass( StencilOp::Keep ) {
    // empty
}

inline
StencilState::~StencilState() {
    // empty
}

inline
void StencilState::setStencilFunc( StencilFunc func, i32 ref, c8 mask ) {
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
void StencilState::setStencilOp( StencilOp sFail, StencilOp dpFail, StencilOp dpPass ) {
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
bool StencilState::operator == ( const StencilState &rhs ) const {
    return ( m_sFail == rhs.m_sFail && m_dpFail == rhs.m_dpFail && m_dpPass == rhs.m_dpPass );
}

inline
bool StencilState::operator != ( const StencilState &rhs ) const {
    return !( *this == rhs );
}

} // Namespace RenderBackend
} // Namespace OSRE
