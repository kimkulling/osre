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
namespace Collision {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
template<class T>
class TRay {
public:
    TRay();
    TRay(const glm::vec3 &origin, const glm::vec3 &direction);
    ~TRay();
    glm::vec3 validate(T time);
    const glm::vec3 &getOrigin() const;
    const glm::vec3 &getDirection() const;
    bool operator == ( const TRay<T> &rhs) const;
    bool operator != (const TRay<T> &rhs) const;

private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
};

template<class T>
inline
TRay<T>::TRay()
: m_origin()
, m_direction() {
    // empty
}

template<class T>
inline TRay<T>::TRay(const glm::vec3 &origin, const glm::vec3 &direction) :
        m_origin(origin), m_direction(direction) {
    // empty
}

template<class T>
inline
TRay<T>::~TRay() {
    // empty
}

template<class T>
inline glm::vec3 TRay<T>::validate(T time) {
    const TVec3<T> res = m_origin + time * m_direction;
    return res;
}

template<class T>
inline const glm::vec3 &TRay<T>::getOrigin() const {
    return m_origin;
}

template<class T>
inline const glm::vec3  &TRay<T>::getDirection() const {
    return m_direction;
}

template<class T>
inline
bool TRay<T>::operator == (const TRay<T> &rhs) const {
    return ( m_origin == rhs.m_origin && m_direction == rhs.m_direction );
}

template<class T>
inline
bool TRay<T>::operator != (const TRay<T> &rhs) const {
    return !( *this == rhs );
}

} // Namespace Collision
} // Namespace OSRE
