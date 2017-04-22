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

namespace OSRE {
namespace RenderBackend {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class CullState {
public:
    enum class CullMode {
        Front,
        Back, 
        FrontAndBack
    };

public:
    CullState();
    explicit CullState( CullMode mode );
    ~CullState();
    void setCullMode( CullMode cullMode );
    CullMode getCullMode() const;
    bool operator == ( const CullState &rhs ) const;
    bool operator != ( const CullState &rhs ) const;

private:
    CullMode m_state;
};

inline
CullState::CullState()
: m_state( CullMode::Front ) {
    // empty
}

inline
CullState::CullState( CullMode mode )
: m_state( mode ) {
    // empty
}

inline
CullState::~CullState() {
    // empty
}

inline
void CullState::setCullMode( CullMode cullMode ) {
    m_state = cullMode;
}

inline
CullState::CullMode CullState::getCullMode() const {
    return m_state;
}

inline
bool CullState::operator == ( const CullState &rhs ) const {
    return m_state == rhs.m_state;
}

inline
bool CullState::operator != ( const CullState &rhs ) const {
    return !( *this == rhs );
}

} // Namespace RenderBackend
} // Namespace OSRE
