/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class describes a ray. Ray have a starting point (origin) and a direction.
///
/// You can use rays to see if any bounding objects will intersect with the ray.
//-------------------------------------------------------------------------------------------------
template<class T>
class TRay {
public:
    /// @brief The default class constructor.
    TRay();
    /// @brief The class constructor with the parameters.
    /// @param origin       The origin of the ray.
    /// @param direction    The direction of the ray.
    TRay( const TVec3<T> &origin, const TVec3<T> &direction );
    
    ///	@brief  The class destructor.
    ~TRay() = default;
    const TVec3<T> &getOrigin() const;
    const TVec3<T> &getDirection() const;
    bool operator == ( const TRay<T> &rhs) const;
    bool operator != (const TRay<T> &rhs) const;

private:
    TVec3<T> m_origin;
    TVec3<T> m_direction;
};

template<class T>
inline TRay<T>::TRay() : m_origin(), m_direction() {
    // empty
}

template<class T>
inline TRay<T>::TRay( const TVec3<T> &origin, const TVec3<T> &direction ) : m_origin( origin ), m_direction( direction ) {
    // empty
}

template<class T>
inline const TVec3<T> &TRay<T>::getOrigin() const {
    return m_origin;
}

template<class T>
inline const TVec3<T> &TRay<T>::getDirection() const {
    return m_direction;
}

template<class T>
inline bool TRay<T>::operator == (const TRay<T> &rhs) const {
    return ( m_origin == rhs.m_origin && m_direction == rhs.m_direction );
}

template<class T>
inline bool TRay<T>::operator != (const TRay<T> &rhs) const {
    return !( *this == rhs );
}

} // Namespace Common
} // Namespace OSRE
