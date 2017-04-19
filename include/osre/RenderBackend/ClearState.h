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
class ClearState {
public:
    enum ClearBitType {
        ColorBit   = 1 << 0,    ///<
        DepthBit   = 1 << 1,    ///<
        StencilBit = 1 << 2     ///<
    };

public:
    ClearState();
    ~ClearState();
    void setClearState( ui32 state );
    ui32 getClearState() const;
    bool operator == ( const ClearState &rhs ) const;
    bool operator != ( const ClearState &rhs ) const;

private:
    ui32 m_state;
};

inline
ClearState::ClearState()
: m_state( 0 ) {
    // empty
}

inline
ClearState::~ClearState() {
    // empty
}

inline
void ClearState::setClearState( ui32 state ) {
    m_state = state;
}

inline
ui32 ClearState::getClearState() const {
    return m_state;
}

inline
bool ClearState::operator == ( const ClearState &rhs ) const {
    return m_state == rhs.m_state;
}

inline
bool ClearState::operator != ( const ClearState &rhs ) const {
    return !( *this == rhs );
}

} // Namespace RenderBackend
} // Namespace OSRE
