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

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace Common {

const f32 COL_SHIFT = 255.0f;

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief This class implements a color in RGBA representation.
///
///	The value is stored in one 32 bit DWORD ( for the Windows platform ).
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT ColorRGBA {
public:
    ///	@brief	The default class constructor.
    ColorRGBA();

    /// @brief  The class constructor with an array of the 4 float components.
    /// @param  pData       [in] A pointer showing to the buffer.
    ColorRGBA( f32 *pData );

    ///	@brief	The class constructor with the color components.
    ///	@param	r, g, b, a	[in] The several components.
    ColorRGBA( f32 r, f32 g, f32 b, f32 a );

    ///	@brief	The class copy constructor.
    ///	@param	other	[in] Instance to copy from.
    ColorRGBA( const ColorRGBA &other );

    ///	@brief	The class destructor.
    ~ColorRGBA();

    ///	@brief	Set the color components.
    ///	@param	r, g, b, a	[in] The several components.
    void set( f32 r, f32 g, f32 b, f32 a );

    ///	@brief	Returns the 32-bit word with the color representation from the given components as
    ///			a RGBA representation.
    ///	@param	r   [in] The red channel
    ///	@param	g   [in] The green channel.
    ///	@param	b   [in] The blue channel.
    ///	@param	a	[in] The alpha channel.
    ///	@return	The color a encoded as one 32-bit DWORD.
    static ui32 getAsRGBA( f32 r, f32 g, f32 b, f32 a );

    /// @brief  Returns the color encoded as RGBA value.
    /// @return The value.
    ui32 getAsRGBA() const;

    ///	@brief	Returns the 32-bit word with the color representation from the given components as
    ///			a ARGB representation.
    ///	@param	r   [in] The red channel
    ///	@param	g   [in] The green channel.
    ///	@param	b   [in] The blue channel.
    ///	@param	a	[in] The alpha channel.
    ///	@return	The color a encoded as one 32-bit DWORD.
    static ui32 getAsARGB( f32 r, f32 g, f32 b, f32 a );
    
    /// @brief  Returns the color encoded as ARGB value.
    /// @return The value.
    ui32 getAsARGB() const;

    const f32 *getValue() const;

    ///	@brief	The equal operator.
    bool operator == ( const ColorRGBA &rOther ) const;
    
    ///	@brief	The assignment operator.
    ColorRGBA &operator = ( const ColorRGBA &rOther );

    f32 m_ColorValues[ 4 ];
};

inline
ColorRGBA::ColorRGBA() {
    m_ColorValues[ 0 ] = 1.0f;
    m_ColorValues[ 1 ] = 0.0f;
    m_ColorValues[ 2 ] = 0.0f;
    m_ColorValues[ 3 ] = 0.0f;
}

inline
ColorRGBA::ColorRGBA( f32 *pData ) {
    m_ColorValues[ 0 ] = pData[ 0 ];
    m_ColorValues[ 1 ] = pData[ 1 ];
    m_ColorValues[ 2 ] = pData[ 2 ];
    m_ColorValues[ 3 ] = pData[ 3 ];
}

inline
ColorRGBA::ColorRGBA( f32 r, f32 g, f32 b, f32 a ) {
    set( r, g, b, a );
}

inline
ColorRGBA::ColorRGBA( const ColorRGBA &other ) {
    ::memcpy( m_ColorValues, other.m_ColorValues, sizeof( f32 ) * 4 );
}

inline
ColorRGBA::~ColorRGBA() {
    // empty
}

inline
void ColorRGBA::set( f32 r, f32 g, f32 b, f32 a ) {
    m_ColorValues[ 0 ] = r;
    m_ColorValues[ 1 ] = g;
    m_ColorValues[ 2 ] = b;
    m_ColorValues[ 3 ] = a;
}

inline
ui32 ColorRGBA::getAsRGBA( f32 r, f32 g, f32 b, f32 a ) {
    uc8 R = (uc8) ( r * COL_SHIFT);
    uc8 G = (uc8) ( g * COL_SHIFT);
    uc8 B = (uc8) ( b * COL_SHIFT);
    uc8 A = (uc8) ( a * COL_SHIFT);
    
    return ( R + ( G<<8 ) + ( B<<16 ) + ( A<<24 ) );
}

inline
ui32 ColorRGBA::getAsRGBA() const {
    return ColorRGBA::getAsRGBA( m_ColorValues[ 0 ], m_ColorValues[ 1 ], m_ColorValues[ 2 ], 
        m_ColorValues[ 3 ] );
}

inline
ui32 ColorRGBA::getAsARGB( f32 r, f32 g, f32 b, f32 a ) {
    uc8 R = (uc8) ( r * COL_SHIFT );
    uc8 G = (uc8) ( g * COL_SHIFT );
    uc8 B = (uc8) ( b * COL_SHIFT );
    uc8 A = (uc8) ( a * COL_SHIFT );

    return ( A + ( R<<8 ) + ( G<<16 ) + ( B<<24 ) );
}

inline
ui32 ColorRGBA::getAsARGB() const {
    return ColorRGBA::getAsARGB( m_ColorValues[ 0 ], m_ColorValues[ 1 ], m_ColorValues[ 2 ], 
        m_ColorValues[ 3 ] );
}

inline
const f32 *ColorRGBA::getValue() const {
    return &m_ColorValues[ 0 ];
}

inline
bool ColorRGBA::operator == ( const ColorRGBA &other ) const {
    if ( 0 == ::memcmp( &m_ColorValues[ 0 ], &other.m_ColorValues[ 0 ], sizeof( f32 ) * 4 ) ) {
        return true;
    } 

    return false;
}

inline
ColorRGBA &ColorRGBA::operator = ( const ColorRGBA &other ) {
    if ( other == *this ) {
        return *this;
    }

    ::memcpy( m_ColorValues, other.m_ColorValues, sizeof( f32 ) * 4 );
    
    return *this;
}

} // Namespace Common
} // Namespace OSRE
