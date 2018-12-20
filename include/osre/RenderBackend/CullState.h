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
///	@brief  This class stores the different information regarding colling for a pipeline pass.
//-------------------------------------------------------------------------------------------------
class CullState {
public:
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

public:
    /// @brief  The default class constructor.
    CullState();

    /// @brief  The class constructor with the cull-mode and the cull face option.
    /// @param  mode        [in] The cull-mode, @see CullMode.
    /// @param  cullFace    [in] The cullFace mode, @see CullFace.
    explicit CullState( CullMode mode, CullFace cullFace );
    
    /// @brief  The class destructor.
    ~CullState();
    
    /// @brief  Will set the new cull mode.
    /// @param  cullmode    [in] The new cull mode.
    void setCullMode( CullMode cullMode );

    /// @brief  Will return the current active cull mode.
    /// @return The active cull mode.
    CullMode getCullMode() const;

    /// @brief  Will set the new cull face mode.
    /// @param  cullface    [in] The new cull face mode.
    void setCullFace( CullFace cullFace );

    /// @brief  Will return the cull-face mode.
    /// @return The cull-face mode.
    CullFace getCullFace() const;

    /// The compare operators.
    bool operator == ( const CullState &rhs ) const;
    bool operator != ( const CullState &rhs ) const;

private:
    CullMode m_state;
    CullFace m_cullFace;
};

inline
CullState::CullState()
: m_state( CullMode::CW )
, m_cullFace( CullFace::Back ) {
    // empty
}

inline
CullState::CullState( CullMode mode, CullFace cullFace )
: m_state( mode )
, m_cullFace( cullFace ) {
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
void CullState::setCullFace( CullFace cullFace ) {
    m_cullFace = cullFace;
}

inline
CullState::CullFace CullState::getCullFace() const {
    return m_cullFace;
}

inline
bool CullState::operator == ( const CullState &rhs ) const {
    return ( m_state == rhs.m_state && m_cullFace == rhs.m_cullFace );
}

inline
bool CullState::operator != ( const CullState &rhs ) const {
    return !( *this == rhs );
}

} // Namespace RenderBackend
} // Namespace OSRE
