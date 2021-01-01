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

// clang-format off
#if defined(_WIN32) || defined(_WIN64)
#    define OSRE_WINDOWS
#    define _CRT_SECURE_NO_WARNINGS
#    ifndef WIN32_LEAN_AND_MEAN
#        define WIN32_LEAN_AND_MEAN // Minimal windows header
#    endif // WIN32_LEAN_AND_MEAN
#elif defined(__gnu_linux__)
#    define OSRE_GNU_LINUX
#elif defined(__APPLE__) || defined(__MACH__)
#    error "Currently not supported platform"
#elif defined(__ANDROID__)
#    define OSRE_ANDROID
#endif

#include <cppcore/Container/TArray.h>

#include <emmintrin.h>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstddef>
#include <sstream>
#include <string>

#ifndef OSRE_WINDOWS
#    include <inttypes.h>
#endif

namespace OSRE {

#ifdef OSRE_WINDOWS
#    define OSRE_TAG_DLL_EXPORT __declspec(dllexport)
#    define OSRE_TAG_DLL_IMPORT __declspec(dllimport)
#endif

#ifdef OSRE_WINDOWS
#    ifdef OSRE_BUILD_EXPORT
#        define OSRE_EXPORT OSRE_TAG_DLL_EXPORT
#    else
#        define OSRE_EXPORT OSRE_TAG_DLL_IMPORT
#    endif
#else
#    define OSRE_EXPORT __attribute__((visibility("default")))
#endif

#if _MSC_VER >= 1200
#    pragma warning(disable : 4006) // Ignore double defined symbols warning
#    pragma warning(disable : 4786) // Identifier was truncated to 'number' characters in the debug information
#    pragma warning(disable : 4251) // class 'type' needs to have dll-interface to be used by clients of class 'type2'
#    pragma warning(disable : 4275) // non DLL-interface classkey 'identifier' used as base for DLL-interface classkey 'identifier'
#    pragma warning(disable : 4127) // Conditional expression is constant
#    pragma warning(disable : 4201) // No standard extension
#    pragma warning(disable : 4316) // Object allocated on the heap may not be aligned for this type
#    pragma warning(disable : 4996) //
#endif

// Declares thread-local data
#ifdef OSRE_WINDOWS
#    define ThreadLocal __declspec(thread)
#else
#    define ThreadLocal __thread
#endif

// Platform-specific data types
#if defined(__MINGW32__) || defined(__MINGW64__)
#    include <stdio.h>
typedef int errno_t;
#endif
// clang-format on

///	The data type unsigned char, 1 byte long.
using uc8 = unsigned char;

///	The data type signed char, 1 byte long.
using c8 = char;

///	The data type unsigned short, 2 byte long.
using ui16 = unsigned short;

///	The data type signed short, 2 byte long.
using i16 = signed short;

///	The data type signed int, 4 byte long.
using i32 = int;

///	The data type unsigned int, 4 byte long.
using ui16 = unsigned short;

///	The data type signed int, 4 byte long.
typedef unsigned int ui32;

///	The data type for signed and unsigned int 8 bytes long.
// clang-format off
#ifdef OSRE_WINDOWS

using i64 = __int64;
using ui64 = unsigned __int64;

#else

using i64 = int64_t;
using ui64 = uint64_t ;

#endif
// clang-format on

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

/// A handle struct.
struct Handle {
    i32 m_idx;

    Handle() :
            m_idx(-1) {
        // empty
    }

    explicit Handle(i32 idx) {
        init(idx);
    }

    void init(i32 idx) {
        m_idx = idx;
    }

    bool operator==(const Handle &rhs) const {
        return m_idx == rhs.m_idx;
    }

    bool operator!=(const Handle &rhs) const {
        return !(*this == rhs);
    }
};

using MemoryBuffer = CPPCore::TArray<c8>;

struct Time {
    i64 m_microseconds;

    Time();
    Time(i64 microseconds);
    f32 asSeconds() const;
    i32 asMilliSeconds() const;
    i64 asMicroSeconds() const;
};

inline Time::Time() :
        m_microseconds(0) {
    // empty
}

inline Time::Time(i64 microseconds) :
        m_microseconds(microseconds) {
    // empty
}

inline f32 Time::asSeconds() const {
    return m_microseconds / 1000000.f;
}

inline i32 Time::asMilliSeconds() const {
    return static_cast<i32>(m_microseconds / 1000);
}

inline i64 Time::asMicroSeconds() const {
    return m_microseconds;
}

struct Color4 {
    f32 m_r, m_g, m_b, m_a;

    Color4() :
            m_r(1.f),
            m_g(1.f),
            m_b(1.f),
            m_a(1.f) {
        // empty
    }

    Color4(f32 r, f32 _g, f32 b, f32 a) :
            m_r(r),
            m_g(_g),
            m_b(b),
            m_a(a) {
        // empty
    }

    bool operator==(const Color4 &rhs) const {
        return (m_r == rhs.m_r && m_g == rhs.m_g && m_b == rhs.m_b && m_a == rhs.m_a);
    }

    bool operator!=(const Color4 &rhs) const {
        return !(*this == rhs);
    }

    f32 operator[](ui32 index) const {
        switch (index) {
            case 0: return m_r;
            case 1: return m_g;
            case 2: return m_b;
            case 3: return m_a;
            default:
                break;
        }

        return 0.0f;
    }
};

template <class T>
struct TVec2 {
    T v[2];

    TVec2() {
        set(0, 0);
    }

    TVec2(T x, T y) {
        set(x, y);
    }

    void set(T x, T y) {
        v[0] = x;
        v[1] = y;
    }

    T getX() const {
        return v[0];
    }

    void setX(T x) {
        v[0] = x;
    }

    T getY() const {
        return v[1];
    }

    void setY(T y) {
        v[1] = y;
    }

    T getSquaredLength() {
        return ((v[0] * v[0]) + (v[1] * v[1]));
    }

    T getLength() {
        return ::sqrt(getSquaredLength());
    }

    T dotProduct(const TVec2<T> &rhs) const {
        return (this->v[0] * rhs.v[0] + this->v[1] * rhs.v[1]);
    }

    TVec2<T> crossProduct(const TVec2<T> &rhs) const {
        return TVec2<T>(this->v[0] * rhs.v[1], this->v[1] * rhs.v[0]);
    }

    TVec2<T> operator+(const TVec2<T> &rhs) const {
        TVec2<T> res(v[0] + rhs.v[0], v[1] + rhs.v[1]);
        return res;
    }

    TVec2<T> operator-(const TVec2<T> &rhs) const {
        TVec2<T> res(v[0] - rhs.v[0], v[1] - rhs.v[1]);
        return res;
    }

    TVec2<T> operator*(T scalar) const {
        TVec2<T> res(v[0] * scalar, v[1] * scalar);
        return res;
    }

    bool operator==(const TVec2<T> &rhs) const {
        return (v[0] == rhs.v[0] && v[1]);
    }

    bool operator!=(const TVec2<T> &rhs) const {
        return !(this == rhs);
    }

    T operator[](ui32 index) const {
        return v[index];
    }
};

template <class T>
inline TVec2<T> operator*(T scalar, TVec2<T> vec) {
    TVec2<T> res(vec.v[0] * scalar, vec.v[1] * scalar);
    return res;
}

using Vec2i = TVec2<i32>;
using Vec2f = TVec2<f32>;

template <class T>
struct TVec3 {
    T v[3];

    TVec3() {
        set(0, 0, 0);
    }

    TVec3(T x, T y, T z) {
        set(x, y, z);
    }

    void set(T x, T y, T z) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
    }

    T getX() const {
        return v[0];
    }

    void setX(T x) {
        v[0] = x;
    }

    T getY() const {
        return v[1];
    }

    void setY(T y) {
        v[1] = y;
    }

    T getZ() const {
        return v[2];
    }

    void setZ(T z) {
        v[2] = z;
    }

    T getSquaredLength() const {
        return ((v[0] * v[0]) + (v[1] * v[1]) + (v[2] * v[2]));
    }

    T getLength() {
        return ::sqrt(getSquaredLength());
    }

    T dotProduct(const TVec3<T> &rhs) const {
        return v[0] * rhs.v[0] + v[1] * rhs.v[1] + v[2] * rhs.v[2];
    }

    TVec3<T> crossProduct(const TVec3<T> &rhs) const {
        return TVec3<T>(v[1] * rhs.v[2] - v[2] * rhs.v[1], v[2] * rhs.v[0] - v[0] * rhs.v[2], v[0] * rhs.v[1] - v[1] * rhs.v[0]);
    }

    bool isZero() const {
        if (v[0] <= 0 && v[1] <= 0 && v[2] <= 0) {
            return true;
        }
        return false;
    }

    TVec3<T> &operator+=(const TVec3<T> &rhs) {
        v[0] += rhs.v[0];
        v[1] += rhs.v[1];
        v[2] += rhs.v[2];

        return *this;
    }

    TVec3<T> &operator+=(T val) {
        v[0] += val;
        v[1] += val;
        v[2] += val;

        return *this;
    }

    TVec3<T> &operator-=(const TVec3<T> &rhs) {
        v[0] -= rhs.v[0];
        v[1] -= rhs.v[1];
        v[2] -= rhs.v[2];

        return *this;
    }

    TVec3<T> &operator-=(T val) {
        v[0] -= val;
        v[1] -= val;
        v[2] -= val;

        return *this;
    }

    TVec3<T> operator*(T scalar) const {
        TVec3<T> res(v[0] * scalar, v[1] * scalar, v[2] * scalar);
        return res;
    }

    TVec3<T> operator+(const TVec3<T> &rhs) const {
        TVec3<T> res(v[0] + rhs.v[0], v[1] + rhs.v[1], v[2] + rhs.v[2]);
        return res;
    }

    TVec3<T> operator-(const TVec3<T> &rhs) const {
        TVec3<T> res(v[0] - rhs.v[0], v[1] - rhs.v[1], v[2] - rhs.v[2]);
        return res;
    }

    bool operator==(const TVec3<T> &rhs) const {
        return (v[0] == rhs.v[0] && v[1] == rhs.v[1] && v[2] == rhs.v[2]);
    }

    bool operator!=(const TVec3<T> &rhs) const {
        return !(*this == rhs);
    }

    T operator[](ui32 index) const {
        return v[index];
    }
};

template <class T>
inline TVec3<T> operator*(T scalar, TVec3<T> vec) {
    TVec3<T> res(vec.v[0] * scalar, vec.v[1] * scalar, vec.v[2] * scalar);
    return res;
}

using Vec3i = TVec3<i32>;
using Vec3f = TVec3<f32>;

template <class T>
struct TVec4 {
    T v[4];

    TVec4() {
        set(0, 0, 0, 1);
    }

    TVec4(T x, T y, T z, T w) {
        set(x, y, z, w);
    }

    void set(T x, T y, T z, T w) {
        v[0] = x;
        v[1] = y;
        v[2] = z;
        v[3] = w;
    }

    T getX() const {
        return v[0];
    }

    void setX(T x) {
        v[0] = x;
    }

    T getY() const {
        return v[1];
    }

    void setY(T y) {
        v[1] = y;
    }

    T getZ() const {
        return v[2];
    }

    void setZ(T z) {
        v[2] = z;
    }

    T getW() const {
        return v[3];
    }

    void setW(T w) {
        v[3] = w;
    }

    TVec4<T> operator+(const TVec4<T> &rhs) const {
        TVec4<T> res(v[0] + rhs.v[0], v[1] + rhs.v[1], v[2] + rhs.v[2], 1);
        return res;
    }

    TVec4<T> operator-(const TVec4<T> &rhs) const {
        TVec4<T> res(v[0] - rhs.v[0], v[1] - rhs.v[1], v[2] - rhs.v[2], 1);
        return res;
    }

    TVec4<T> operator*(T scalar) const {
        TVec4<T> res(v[0] * scalar, v[1] * scalar, v[2] * scalar, 1);
        return res;
    }

    bool operator==(const TVec4<T> &rhs) const {
        return (v[0] == rhs.v[0] && v[1] == rhs.v[1] && v[2] == rhs.v[2] && v[3] == rhs.v[3]);
    }

    bool operator!=(const TVec4<T> &rhs) const {
        return !(this == rhs);
    }

    T operator[](ui32 index) const {
        return v[index];
    }
};

using Vec4i = TVec4<i32>;
using Vec4f = TVec4<f32>;

template <class T>
struct TQuat {
    T m_x, m_y, m_z, m_w;

    TQuat() :
            m_x(0), m_y(0), m_z(0), m_w(1) {
        // empty
    }

    TQuat(T x, T y, T z, T w) :
            m_x(x), m_y(y), m_z(z), m_w(1) {
        // empty
    }
    
    void set(T x, T y, T z, T w) {
        m_x = x;
        m_y = y;
        m_z = z;
        m_w = w;
    }
};

using Quatf = TQuat<f32>;

template <class T>
struct TPoint2 {
    T m_x, m_y;

    TPoint2() :
            m_x(0),
            m_y(0) {
        // empty
    }

    TPoint2(T x, T y) :
            m_x(x),
            m_y(y) {
        // empty
    }

    bool operator==(const TPoint2<T> &rhs) const {
        return (m_x == rhs.m_x && m_y == rhs.m_y);
    }

    bool operator!=(const TPoint2<T> &rhs) const {
        return !(*this == rhs);
    }
};

using Point2i = TPoint2<i32>;
using Point2ui = TPoint2<ui32>;

/// @brief  Helper class t represent a 2D rectangle.
template <class T>
struct TRectangle {
    T m_x1, m_y1, m_x2, m_y2, m_width, m_height;

    /// @brief  The default class constructor.
    TRectangle() :
            m_x1(0),
            m_y1(0),
            m_x2(0),
            m_y2(0),
            m_width(0),
            m_height(0) {
        // empty
    }

    /// @brief  The class constructor with width and height, x and 0 are set to 0,0.
    /// @param  width   [in] The width of the rectangle.
    /// @param  height  [in] The height of the rectangle.
    TRectangle(T width, T height) :
            m_x1(0),
            m_y1(0),
            m_x2(width),
            m_y2(height),
            m_width(width),
            m_height(height) {
        // empty
    }

    /// @brief  The class constructor with parameters
    /// @param  x       [in] X coordinate of upper left corner.
    /// @param  y       [in] Y coordinate of upper left corner.
    /// @param  width   [in] The width of the rectangle.
    /// @param  height  [in] The height of the rectangle.
    TRectangle(T x, T y, T width, T height) :
            m_x1(x),
            m_y1(y),
            m_x2(x + width),
            m_y2(y + height),
            m_width(width),
            m_height(height) {
        // empty
    }

    /// @brief  The class destructor.
    ~TRectangle() {
        // empty
    }

    /// @brief  Will set the rectangle-geometry with the upper left corner, width and height.
    /// @param  x       [in] X coordinate of upper left corner.
    /// @param  y       [in] Y coordinate of upper left corner.
    /// @param  width   [in] The width of the rectangle.
    /// @param  height  [in] The height of the rectangle.
    void set(T x, T y, T width, T height) {
        m_x1 = x;
        m_y1 = y;
        m_x2 = x + width;
        m_y2 = y + height;
        m_width = width;
        m_height = height;
    }

    /// @brief  Will set the rectangle-geometry with the upper left corner and the lower right corner.
    /// @param  x1      [in] X coordinate of the upper left corner.
    /// @param  y1      [in] Y coordinate of the upper left corner.
    /// @param  x2      [in] X coordinate of the lower right corner.
    /// @param  y2      [in] Y coordinate of the lower right corner.
    void setEdges(T x1, T y1, T x2, T y2) {
        m_x1 = x1;
        m_y1 = y1;
        m_x2 = x2;
        m_y2 = y2;
        m_width = x2 - x1;
        m_height = y2 - y1;
    }

    /// @brief  Return the X-coordinate of the upper left corner.
    /// @brief  The x-coordinate.
    T getX1() const {
        return m_x1;
    }

    /// @brief  Return the X-coordinate of the upper left corner.
    /// @brief  The y-coordinate.
    T getY1() const {
        return m_y1;
    }

    T getX2() const {
        return m_x2;
    }

    T getY2() const {
        return m_y2;
    }

    T getWidth() const {
        return m_width;
    }

    T getHeight() const {
        return m_height;
    }

    bool isIn(const TPoint2<T> &pt) const {
        if (pt.m_x >= m_x1 && pt.m_y >= m_y1 && pt.m_x <= m_x2 && pt.m_y <= m_y2) {
            return true;
        }

        return false;
    }

    const bool operator==(const TRectangle<T> &rhs) const {
        return (m_x1 == rhs.m_x1 && m_y1 == rhs.m_y1 && m_width == rhs.m_width && m_height == rhs.m_height);
    }

    const bool operator!=(const TRectangle<T> &rhs) const {
        return (m_x1 != rhs.m_x1 || m_y1 != rhs.m_y1 || m_width != rhs.m_width || m_height != rhs.m_height);
    }
};

using Rect2ui = TRectangle<ui32>;
using Rect2i = TRectangle<i32>;

struct float4 {
    union {
        __m128 m_val4;
        f32 m_vals[4];
    };

    float4();
    float4(f32 a, f32 b, f32 c, f32 d);
    float4(const float4 &rhs);
    const float4 operator+=(const float4 &v);
    const float4 operator-=(const float4 &v);
    const float4 operator*=(const float4 &v);
    const float4 operator/=(const float4 &v);
};

inline float4::float4() {
    m_vals[0] = 0.0f;
    m_vals[1] = 0.0f;
    m_vals[2] = 0.0f;
    m_vals[3] = 0.0f;
}

inline float4::float4(f32 a, f32 b, f32 c, f32 d) :
        m_val4(_mm_set_ps(d, c, b, a)) {
    // empty
}

inline float4::float4(const float4 &rhs) :
        m_val4(rhs.m_val4) {
    // empty
}

inline const float4 float4::operator+=(const float4 &v) {
    m_val4 = _mm_add_ps(m_val4, v.m_val4);
    return *this;
}

inline const float4 float4::operator-=(const float4 &v) {
    m_val4 = _mm_sub_ps(m_val4, v.m_val4);
    return *this;
}

inline const float4 float4::operator*=(const float4 &v) {
    m_val4 = _mm_mul_ps(m_val4, v.m_val4);
    return *this;
}

inline const float4 float4::operator/=(const float4 &v) {
    m_val4 = _mm_div_ps(m_val4, v.m_val4);
    return *this;
}

///	@brief  Shortcut to avoid copy operations.
#define OSRE_NON_COPYABLE(NAME)  \
private:                         \
    NAME(const NAME &) = delete; \
    NAME &operator=(const NAME &) = delete;

template <class T>
inline String osre_to_string(T val) {
    std::stringstream str;
    str << val;

    return str.str();
}

// Archive file version
static const i32 CurrentMajorVersion = 0;
static const i32 CurrentMinorVersion = 1;

struct Version {
    i32 mMajor;
    i32 mMinor;

    Version(i32 major, i32 minor) :
            mMajor(major), mMinor(minor) {
        // empty
    }
};

class OSRE_EXPORT MemoryStatistics {
public:
    static size_t sAllocated;
    static size_t sNumNew;
    static size_t sActiveAllocs;

    static void addAllocated(size_t allocSize);
    static void releaseAlloc();
    static void showStatistics();
};

} // Namespace OSRE

void *operator new(size_t size);
void operator delete(void *ptr) noexcept;

#define osre_new(T) new T()
#define osre_new_array(T, len) new T[len]
#define osre_delete delete
#define osre_delete_array delete[]
