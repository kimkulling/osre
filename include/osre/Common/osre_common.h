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

#include <string>
#include <string.h>
#include <stdio.h>
#include <cstddef>
#include <cmath>

#ifndef _WIN32
#  include <inttypes.h>
#endif 

namespace OSRE {

#if defined( _WIN32 ) || defined( _WIN64 )
#   define OSRE_WINDOWS
#   define _CRT_SECURE_NO_WARNINGS
#elif defined(__gnu_linux__)
#   define OSRE_GNU_LINUX
#elif defined(__APPLE__) || defined (__MACH__)

#elif defined(__ANDROID__)
#   define OSRE_ANDROID
#endif

#ifdef OSRE_WINDOWS
#  define TAG_DLL_EXPORT __declspec(dllexport)
#  define TAG_DLL_IMPORT __declspec(dllimport )
#endif 

#ifdef OSRE_WINDOWS
#   ifdef OSRE_BUILD_EXPORT
#       define OSRE_EXPORT TAG_DLL_EXPORT
#   else
#       define OSRE_EXPORT TAG_DLL_IMPORT
#   endif
#else
#   define OSRE_EXPORT  __attribute__ ((visibility("default")))
#endif

#if _MSC_VER >= 1200
#  pragma warning( disable : 4006 ) // Ignore double defined symbols warning
#  pragma warning( disable : 4786 )
#  pragma warning( disable : 4251 )
#  pragma warning( disable : 4275 )
#  pragma warning( disable : 4127 )	// Conditional expression is constant
#  pragma warning( disable : 4100 )
#  pragma warning( disable : 4201 ) // No standard extension
#  pragma warning( disable : 4316 )
#endif

// Declares thread-local data
#ifdef OSRE_WINDOWS
#   define ThreadLocal __declspec(thread)
#else
#   define ThreadLocal __thread
#endif

// Platform-specific data types
#if defined(__MINGW32__) || defined(__MINGW64__)
#   include <stdio.h>
    typedef int errno_t;
#endif

///	The data type unsigned char, 1 byte long.
typedef unsigned char uc8;

///	The data type signed char, 1 byte long.
typedef char c8;

///	The data type unsigned short, 2 byte long.
typedef unsigned short ui16;

///	The data type signed short, 2 byte long.
typedef signed short i16;

///	The data type signed int, 4 byte long.
typedef int	i32;

///	The data type unsigned int, 4 byte long.
typedef unsigned short ui16;

///	The data type signed int, 4 byte long.
typedef unsigned int ui32;

///	The data type for signed and unsigned int 8 bytes long.
#ifdef OSRE_WINDOWS
typedef __int64           i64;
typedef unsigned __int64  ui64;
#else
typedef int64_t           i64;
typedef uint64_t          ui64;
#endif

/// The data type for hash ids.
typedef unsigned long HashId;

///	The data type float, 4 byte long.
typedef float f32;

///	The data type double, 8 byte long.
typedef double d32;

///	The data type Object id.
typedef int ObjectId;

///	The data type for strings
typedef std::string String;

struct Handle {
    i32 m_idx;

    Handle()
    : m_idx( -1 ) {
        // empty
    }

    explicit Handle( i32 idx ) {
        init( idx );
    }

    void init( int idx ) {
        m_idx = idx;
    }
};

struct Color4 {
    f32 m_r, m_g, m_b, m_a;

    Color4()
    : m_r( 1.f )
    , m_g( 1.f )
    , m_b( 1.f )
    , m_a( 1.f ) {
        // empty
    }

    Color4( f32 r, f32 g, f32 b, f32 a )
    : m_r( r )
    , m_g( g )
    , m_b( b )
    , m_a( a ) {
        // empty
    }

    bool operator == ( const Color4 & rhs ) const {
        return ( m_r == rhs.m_r && m_g == rhs.m_g && m_b == rhs.m_b && m_a == rhs.m_a );
    }

    bool operator != ( const Color4 & rhs ) const {
        return !( *this == rhs );
    }
};

template<class T>
struct TVec2 {
    T v[ 2 ];

    TVec2() {
        set( 0, 0 );
    }

    TVec2( T x, T y ) {
        set( x, y );
    }

    void set( T x, T y ) {
        v[ 0 ] = x;
        v[ 1 ] = y;
    }

    T getX() const {
        return v[ 0 ];
    }

    void setX( T x ) {
        v[ 0 ] = x;
    }

    T getY() const {
        return v[ 1 ];
    }

    void setY( T y ) {
        v[ 1 ] = y;
    }

    T getSquaredLength() {
        return ( ( v[ 0 ] * v[ 0 ] ) + ( v[ 1 ] * v[ 1 ] ) );
    }

    T getLength() {
        return ::sqrt( getSquaredLength() );
    }

    T dotProduct( const TVec2<T> &rhs ) const {
        return ( this->v[ 0 ] * rhs.v[ 0 ] + this->v[ 1 ] * rhs.v[1] );
    }

    TVec2<T> crossProduct( const TVec2<T> &rhs ) const {
        return TVec2<T>( this->v[ 0 ] * rhs.v[ 1 ], this->v[ 1 ] * rhs.v[ 0 ] );
    }

    TVec2<T> operator + ( const TVec2<T> &rhs ) const {
        TVec2<T> res( v[ 0 ] + rhs.v[ 0 ], v[ 1 ] + rhs.v[ 1 ] );
        return res;
    }

    TVec2<T> operator - ( const TVec2<T> &rhs )  const {
        TVec2<T> res( v[ 0 ] - rhs.v[ 0 ], v[ 1 ] - rhs.v[ 1 ] );
        return res;
    }

    TVec2<T> operator * ( T scalar ) const {
        TVec2<T> res( v[ 0 ] * scalar, v[ 1 ] * scalar );
        return res;
    }

    bool operator == ( const TVec2<T> &rhs ) const {
        return ( v[ 0 ] == rhs.v[ 0 ] && v[ 1 ] );
    }

    bool operator != ( const TVec2<T> &rhs ) const {
        return !( this == rhs );
    }
};

template<class T>
inline
TVec2<T> operator * ( T scalar, TVec2<T> vec ) {
    TVec2<T> res( vec.v[ 0 ] * scalar, vec.v[ 1 ] * scalar );
    return res;
}

typedef TVec2<i32> Vec2i;
typedef TVec2<f32> Vec2f;

template<class T>
struct TVec3 {
    T v[ 3 ];

    TVec3() {
        set( 0, 0, 0 );
    }

    TVec3( T x, T y, T z ) {
        set( x, y, z );
    }

    void set( T x, T y, T z ) {
        v[ 0 ] = x;
        v[ 1 ] = y;
        v[ 2 ] = z;
    }

    T getX() const {
        return v[ 0 ];
    }

    void setX( T x ) {
        v[ 0 ] = x;
    }

    T getY() const {
        return v[ 1 ];
    }

    void setY( T y ) {
        v[ 1 ] = y;
    }

    T getZ() const {
        return v[ 2 ];
    }

    void setZ( T z ) {
        v[ 2 ] = z;
    }

    T getSquaredLength()  const {
        return ( ( v[ 0 ] * v[ 0 ] ) + ( v[ 1 ] * v[ 1 ] ) + ( v[ 2 ] * v[ 2 ] ) );
    }

    T getLength() {
        return ::sqrt( getSquaredLength() );
    }

    T  dotProduct( const TVec3<T> &rhs ) const {
        return v[ 0 ] * rhs.v[ 0 ] + v[ 1 ] * rhs.v[ 1 ] + v[ 2 ] * rhs.v[ 2 ];
    }

    TVec3<T> crossProduct( const TVec3<T> &rhs ) const {
        return TVec3<T>( v[ 1 ] * rhs.v[ 2 ]- v[ 2 ] * rhs.v[ 1 ]
            , v[ 2 ] * rhs.v[ 0 ] - v[ 0 ] * rhs.v[ 2 ]
            , v[ 0 ] * rhs.v[ 1 ] - v[ 1 ] * rhs.v[ 0 ] );
    }

    bool isZero() const {
        if ( v[ 0 ] <= 0 && v[ 1 ] <= 0 && v[ 2 ] <= 0 ) {
            return true;
        }
        return false;
    }

    TVec3<T> &operator += ( const TVec3<T> &rhs ) {
        v[ 0 ] += rhs.v[ 0 ];
        v[ 1 ] += rhs.v[ 1 ];
        v[ 2 ] += rhs.v[ 2 ];

        return *this;
    }

    TVec3<T> &operator += ( T val ) {
        v[ 0 ] += val;
        v[ 1 ] += val;
        v[ 2 ] += val;

        return *this;
    }

    TVec3<T> &operator -= ( const TVec3<T> &rhs ) {
        v[ 0 ] -= rhs.v[ 0 ];
        v[ 1 ] -= rhs.v[ 1 ];
        v[ 2 ] -= rhs.v[ 2 ];

        return *this;
    }

    TVec3<T> &operator -= ( T val ) {
        v[ 0 ] -= val;
        v[ 1 ] -= val;
        v[ 2 ] -= val;

        return *this;
    }

    TVec3<T> operator * (  T scalar )  const {
        TVec3<T> res( v[ 0 ] * scalar, v[ 1 ] * scalar, v[ 2 ] * scalar );
        return res;
    }

    TVec3<T> operator + ( const TVec3<T> &rhs ) const {
        TVec3<T> res( v[ 0 ] + rhs.v[ 0 ], v[ 1 ] + rhs.v[ 1 ], v[ 2 ] + rhs.v[ 2 ] );
        return res;
    }

    TVec3<T> operator - ( const TVec3<T> &rhs )  const {
        TVec3<T> res( v[ 0 ] - rhs.v[ 0 ], v[ 1 ] - rhs.v[ 1 ], v[ 2 ] - rhs.v[ 2 ] );
        return res;
    }

    bool operator == ( const TVec3<T> &rhs ) const {
        return ( v[ 0 ] == rhs.v[ 0 ] && v[ 1 ] == rhs.v[ 1 ] && v[ 2 ] == rhs.v[ 2 ] );
    }

    bool operator != ( const TVec3<T> &rhs ) const {
        return !( *this == rhs );
    }
};

template<class T>
inline
TVec3<T> operator * ( T scalar, TVec3<T> vec ) {
    TVec3<T> res( vec.v[ 0 ] * scalar, vec.v[ 1 ] * scalar, vec.v[ 2 ] * scalar );
    return res;
}


typedef TVec3<i32> Vec3i;
typedef TVec3<f32> Vec3f;

template<class T>
struct TVec4 {
    T v[ 4 ];

    TVec4() {
        set( 0, 0, 0, 1 );
    }

    TVec4( T x, T y, T z, T w ) {
        set( x, y, z, w );
    }

    void set( T x, T y, T z, T w ) {
        v[ 0 ] = x;
        v[ 1 ] = y;
        v[ 2 ] = z;
        v[ 3 ] = w;
    }

    T getX() const {
        return v[ 0 ];
    }

    void setX( T x ) {
        v[ 0 ] = x;
    }

    T getY() const {
        return v[ 1 ];
    }

    void setY( T y ) {
        v[ 1 ] = y;
    }

    T getZ() const {
        return v[ 2 ];
    }

    void setZ( T z ) {
        v[ 2 ] = z;
    }

    T getW() const {
        return v[ 3 ];
    }

    void setW( T w ) {
        v[ 3 ] = w;
    }

    TVec4<T> operator + ( const TVec4<T> &rhs )  const {
        TVec4<T> res( v[ 0 ] + rhs.v[ 0 ], v[ 1 ] + rhs.v[ 1 ], v[ 2 ] + rhs.v[ 2 ], 1 );
        return res;
    }

    TVec4<T> operator - ( const TVec4<T> &rhs ) const {
        TVec4<T> res( v[ 0 ] - rhs.v[ 0 ], v[ 1 ] - rhs.v[ 1 ], v[ 2 ] - rhs.v[ 2 ], 1 );
        return res;
    }

    TVec4<T> operator * ( T scalar )  const {
        TVec4<T> res( v[ 0 ] * scalar, v[ 1 ] * scalar, v[ 2 ] * scalar, 1 );
        return res;
    }

    bool operator == ( const TVec4<T> &rhs ) const {
        return ( v[ 0 ] == rhs.v[ 0 ] && v[ 1 ] == rhs.v[ 1 ] && v[ 2 ] == rhs.v[ 2 ] && v[ 3 ] == rhs.v[ 3 ] );
    }

    bool operator != ( const TVec4<T> &rhs ) const {
        return !( this == rhs );
    }
};

typedef TVec4<i32> Vec4i;
typedef TVec4<f32> Vec4f;

template<class T>
struct TRect2D {
    T m_x, m_y, m_width, m_height;

    TRect2D()
    : m_x( 0 )
    , m_y( 0 )
    , m_width( 0 )
    , m_height( 0 ) {
        // empty
    }

    TRect2D( T x, T y, T width, T height )
    : m_x( x )
    , m_y( y )
    , m_width( width )
    , m_height( height ) {
        // empty
    }

    ~TRect2D() {
        // empty
    }

    T getX1() const {
        return m_x;
    }

    T getY1() const {
        return m_y;
    }

    T getX2() const {
        return m_x + m_width;
    }

    T getY2() const {
        return m_y + m_height;
    }

    const bool operator == ( const TRect2D<T> &rhs ) const {
        return ( m_x == rhs.m_x && m_y == rhs.m_y && m_width == rhs.m_width && m_height == rhs.m_height );
    }

    const bool operator != ( const TRect2D<T> &rhs ) const {
        return ( m_x != rhs.m_x || m_y != rhs.m_y || m_width != rhs.m_width || m_height != rhs.m_height );
    }
};

typedef TRect2D<ui32> RectUI;

template<class T>
struct OSRE_EXPORT TQuat {
    T m_x, m_y, m_z, m_w;

    TQuat()
    : m_x( 0 )
    , m_y( 0 )
    , m_z( 0 )
    , m_w( 1 ) {
        // empty
    }

    void set( T x, T y, T z, T w ) {
        m_x = x;
        m_y = y;
        m_z = z;
        m_w = w;
    }
};

typedef TQuat<f32> Quatf;

///	@brief  Shortcut to avoid copy operations.
#define OSRE_NON_COPYABLE( NAME ) \
private: \
    NAME( const NAME & ) = delete;        \
    NAME& operator = ( const NAME & ) = delete;

} // Namespace OSRE
