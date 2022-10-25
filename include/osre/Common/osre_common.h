/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2022 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/glm_common.h>
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

/// @brief  The data type unsigned char, 1 byte long.
using uc8 = unsigned char;

/// @brief  The data type signed char, 1 byte long.
using c8 = char;

/// @brief  The data type unsigned short, 2 byte long.
using ui16 = unsigned short;

/// @brief  The data type signed short, 2 byte long.
using i16 = signed short;

/// @brief  The data type signed int, 4 byte long.
using i32 = int;

/// @brief  The data type unsigned int, 4 byte long.
using ui16 = unsigned short;

/// @brief  The data type signed int, 4 byte long.
using ui32 = unsigned int;

/// @brief  The data type for signed and unsigned int 8 bytes long.
// clang-format off
#ifdef OSRE_WINDOWS

using i64 = __int64;
using ui64 = unsigned __int64;

#else

using i64 = int64_t;
using ui64 = uint64_t ;

#endif
// clang-format on

/// @brief  The data type for hash ids.
using HashId = ui64;

/// @brief  The data type float, 4 byte long.
using f32 = float;

/// @brief  The data type double, 8 byte long.
using d32 = double;

/// @brief  The data type Object id.
using ObjectId = int;

/// @brief  The id for unique ids.
using guid = ui64;

/// @brief  The data type for strings
using String = std::string;

/// @brief The data type for an array of strings.
using StringArray = ::CPPCore::TArray<String>;


/// @brief  A handle struct.
struct Handle {
    static constexpr i32 Invalid = -1;
    
    i32 m_idx;

    Handle() : m_idx(Invalid) {
        // empty
    }

    explicit Handle(i32 idx) {
        init(idx);
    }

    void init(i32 idx) {
        m_idx = idx;
    }

    bool isValid() const {
        return m_idx != Invalid;
    }

    bool operator == (const Handle &rhs) const {
        return m_idx == rhs.m_idx;
    }

    bool operator != (const Handle &rhs) const {
        return !(*this == rhs);
    }
};

/// @brief  This type shall be used to store binary data.
using MemoryBuffer = CPPCore::TArray<c8>;

/// @brief  A time stamp.
struct Time {
    i64 m_microseconds;

    Time();
    Time(i64 microseconds);
    f32 asSeconds() const;
    i32 asMilliSeconds() const;
    i64 asMicroSeconds() const;
};

inline Time::Time() : m_microseconds(0) {}

inline Time::Time(i64 microseconds) : m_microseconds(microseconds) {}

inline f32 Time::asSeconds() const {
    return m_microseconds / 1000000.f;
}

inline i32 Time::asMilliSeconds() const {
    return static_cast<i32>(m_microseconds / 1000);
}

inline i64 Time::asMicroSeconds() const {
    return m_microseconds;
}

/// @brief  This type can be used to define a color with 4 colors.
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

    glm::vec4 toVec4() {
        glm::vec4 col = {};
        col.r = m_r;
        col.g = m_g;
        col.b = m_b;
        col.a = m_a;
        return col;
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

/// @brief  a 2D point.
template <class T>
struct TPoint2 {
    T x, y;

    /// @brief  The default class constructor.
    TPoint2() :
            x(0),
            y(0) {
        // empty
    }

    /// @brief  The class constructor with the values.
    /// @param  x   [in] The x-component.
    /// @param  y   [in] The y-component.
    TPoint2(T x_, T y_) :
            x(x_),
            y(y_) {
        // empty
    }

    ///	@brief  The class destructor.
    ~TPoint2() = default;

    /// @brief  The compare operator.
    bool operator==(const TPoint2<T> &rhs) const {
        return (x == rhs.x && y == rhs.y);
    }

    /// @brief  The not-equal operator.
    bool operator!=(const TPoint2<T> &rhs) const {
        return !(*this == rhs);
    }
};

using Point2i = TPoint2<i32>;
using Point2ui = TPoint2<ui32>;

/// @brief  Helper class t represent a 2D rectangle.
template <class T>
struct TRectangle {
    T x1, y1, x2, y2, width, height;

    /// @brief  The default class constructor.
    TRectangle() :
            x1(0),
            y1(0),
            x2(0),
            y2(0),
            width(0),
            height(0) {
        // empty
    }

    /// @brief  The class constructor with parameters
    /// @param  x       [in] X coordinate of upper left corner.
    /// @param  y       [in] Y coordinate of upper left corner.
    /// @param  width   [in] The width of the rectangle.
    /// @param  height  [in] The height of the rectangle.
    TRectangle(T x, T y, T width, T height) :
            x1(x),
            y1(y),
            x2(x + width),
            y2(y + height),
            width(width),
            height(height) {
        // empty
    }

    /// @brief  The class destructor.
    ~TRectangle() = default;

    /// @brief  Will set the rectangle-geometry with the upper left corner, width and height.
    /// @param  x       [in] X coordinate of upper left corner.
    /// @param  y       [in] Y coordinate of upper left corner.
    /// @param  width_  [in] The width of the rectangle.
    /// @param  height_ [in] The height of the rectangle.
    void set(T x, T y, T width_, T height_) {
        x1 = x;
        y1 = y;
        width = width_;
        height = height_;
        x2 = x + width;
        y2 = y + height;
    }

    /// @brief  Will set the rectangle-geometry with the upper left corner and the lower right corner.
    /// @param  x1_     [in] X coordinate of the upper left corner.
    /// @param  y1_     [in] Y coordinate of the upper left corner.
    /// @param  x2_     [in] X coordinate of the lower right corner.
    /// @param  y2_     [in] Y coordinate of the lower right corner.
    void setEdges(T x1_, T y1_, T x2_, T y2_) {
        x1 = x1_;
        y1 = y1_;
        x2 = x2_;
        y2 = y2_;
        width = x2 - x1;
        height = y2 - y1;
    }

    /// @brief  Return the X-coordinate of the upper left corner.
    /// @brief  The x-coordinate.
    T getX1() const {
        return x1;
    }

    /// @brief  Return the y-coordinate of the upper left corner.
    /// @brief  The y-coordinate.
    T getY1() const {
        return y1;
    }

    /// @brief  Return the X-coordinate of the lower right corner.
    /// @brief  The x-coordinate.
    T getX2() const {
        return x2;
    }

    /// @brief  Return the Y-coordinate of the lower right corner.
    /// @brief  The y-coordinate.
    T getY2() const {
        return y2;
    }

    /// @brief  Return the width of the rect.
    /// @brief  The width.
    T getWidth() const {
        return width;
    }

    /// @brief  Return the height of the rect.
    /// @brief  The height.
    T getHeight() const {
        return height;
    }

    /// @brief  Returns true, if he point is in the rect.
    /// @param  pt  [in] The 2D-point to check.
    /// @return true if the point is in the rectangle, false if not.
    bool isIn(const TPoint2<T> &pt) const {
        if (pt.x >= x1 && pt.y >= y1 && pt.x <= x2 && pt.y <= y2) {
            return true;
        }

        return false;
    }

    /// @brief  The compare operator.
    const bool operator==(const TRectangle<T> &rhs) const {
        return (x1 == rhs.x1 && y1 == rhs.y1 && width == rhs.width && height == rhs.height);
    }

    /// @brief  The not-equal operator.
    const bool operator!=(const TRectangle<T> &rhs) const {
        return (x1 != rhs.x1 || y1 != rhs.y1 || width != rhs.width || height != rhs.height);
    }
};

using Rect2ui = TRectangle<ui32>;
using Rect2i = TRectangle<i32>;

/// @brief  a float4 representation type.
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

/// @brief  The resolution type
template <class T>
struct TResolution {
    T Width, Height;

    TResolution(T w, T h) : Width(w), Height(h) {
        // empty
    }

    ~TResolution() = default;

    T getArea() const {
        return Width * Height;
    }
};

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
static constexpr i32 CurrentMajorVersion = 0;
static constexpr i32 CurrentMinorVersion = 1;

/// @brief  The version type
struct Version {
    i32 mMajor;
    i32 mMinor;

    Version(i32 major, i32 minor) : mMajor(major), mMinor(minor) {
        // empty
    }

    ~Version() = default;
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
