/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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

template<class T>
class TAABB {
public:
    typedef TVec3<T> VecType;

    TAABB();
    TAABB( const VecType &min, const VecType &max );
    ~TAABB();
    void set( const VecType &min, const VecType &max );
    const TVec3<T> &getMin() const;
    const TVec3<T> &getMax() const;
    void merge( const VecType &vec );

private:
    VecType m_min;
    VecType m_max;
};

template<class T>
inline
TAABB<T>::TAABB()
: m_min( 999999,99999,99999)
, m_max( -999999, -99999, -99999 ) {
    // empty
}

template<class T>
inline
TAABB<T>::TAABB( const VecType &min, const VecType &max )
: m_min( min )
, m_max( max ) {
    // empty
}

template<class T>
inline
TAABB<T>::~TAABB() {
    // empty
}

template<class T>
inline
void TAABB<T>::set( const VecType &min, const VecType &max ) {
    m_min = min;
    m_max = max;
}

template<class T>
inline
const TVec3<T> &TAABB<T>::getMin() const {
    return m_min;
}

template<class T>
inline
const TVec3<T> &TAABB<T>::getMax() const {
    return m_max;
}

template<class T>
inline
void TAABB<T>::merge( const VecType &vec ) {
    // set min values
    if ( vec.getX() < m_min.getX() ) {
        m_min.setX( vec.getX() );
    }
    if ( vec.getY() < m_min.getY() ) {
        m_min.setY( vec.getY() );
    }
    if ( vec.getZ() < m_min.getZ() ) {
        m_min.setZ( vec.getZ() );
    }

    // set max values
    if ( vec.getX() > m_max.getX() ) {
        m_max.setX( vec.getX() );
    }
    if ( vec.getY() > m_max.getY() ) {
        m_max.setY( vec.getY() );
    }
    if ( vec.getZ() > m_max.getZ() ) {
        m_max.setZ( vec.getZ() );
    }

}

} // Namespace Collision
} // Namespace OSRE
