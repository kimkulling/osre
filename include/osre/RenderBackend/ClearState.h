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

namespace OSRE {
namespace RenderBackend {
        
//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class describes the current active clear states for the render-pass.
///
/// It is possible to request several clear states. The states will be set as bits.
//-------------------------------------------------------------------------------------------------
class ClearState {
public:
    enum class ClearBitType : int {
        ColorBit   = 1 << 0,    ///< Clear the color buffer.
        DepthBit   = 1 << 1,    ///< Clear the depth buffer.
        StencilBit = 1 << 2     ///< Clear the stencil buffer.
    };

public:
    /// @brief  The default class constructor.
    ClearState();

    /// @brief  The class constructor with the requested clear states.
    /// @param  states      [in] The requested clear states.
    explicit ClearState( ui32 states );

    /// @brief  The class destructor.
    ~ClearState();

    /// @brief  Will set the requested clear states.
    /// @param  states      [in] The requested clear states.
    void setClearState( ui32 states );

    /// @brief  Will return the active clear states.
    /// @return The requested clear states.
    ui32 getClearState() const;

    /// The compare operators.
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
ClearState::ClearState( ui32 state )
: m_state( state ) {
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
