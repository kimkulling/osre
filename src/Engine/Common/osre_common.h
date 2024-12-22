/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Common/glm_common.h"

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
#    pragma warning(disable : 4996) // Force incline does not work
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

/// @brief  The data type for signed int 8 bytes long.
using i64 = int64_t;

/// @brief  The data type for unsigned int 8 bytes long.
using ui64 = uint64_t;

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
using StringArray = ::cppcore::TArray<String>;

/// @brief  The data type for hash ids.
using HashId = ui64;

/// @brief  A struct to manage a handle.
struct Handle {
    static constexpr i32 Invalid = -1;

    i32 m_idx;

    /// @brief The default class constructor.
    Handle() : m_idx(Invalid) {
        // empty
    }

    /// @brief The class constructor with the index.
    /// @param[in] idx   The index.
    explicit Handle(i32 idx) {
        init(idx);
    }

    /// @brief Inits the handle with a new index.
    /// @param[in] idx   The new index.
    void init(i32 idx) {
        m_idx = idx;
    }

    /// @brief Will validate the index.
    /// @return true if valid, false if not.
    bool isValid() const {
        return m_idx != Invalid;
    }

    /// @brief Equal operator.
    bool operator == (const Handle &rhs) const {
        return m_idx == rhs.m_idx;
    }

    /// @brief Not-equal operator.
    bool operator != (const Handle &rhs) const {
        return !(*this == rhs);
    }
};

/// @brief  This type shall be used to store binary data.
using MemoryBuffer = cppcore::TArray<c8>;

/// @brief  A time stamp.
struct Time {
    i64 Microseconds; ///< The store time stamp in microseconds

    /// @brief  The default constructor.
    Time();

    /// @brief  The constructor with the timestamp in ms.
    /// @param[in] microseconds  The timestamp in microseconds.
    Time(i64 microseconds);

    void setMilliSeconds(i64 ms) {
        Microseconds = ms * 1000l; 
    }

    /// @brief The class destructor.
    ~Time() = default;

    /// @brief Will return the timestamp in seconds.
    /// @return The timestamp in secods.
    f32 asSeconds() const;

    /// @brief Will return the timestamp in milliseconds.
    /// @return The timestamp in millisecods.
    i32 asMilliSeconds() const;

    /// @brief Will return the timestamp in microseconds.
    /// @return The timestamp in microsecods.
    i64 asMicroSeconds() const;
};

inline Time::Time() : Microseconds(0L) {}

inline Time::Time(i64 microseconds) : Microseconds(microseconds) {}

inline f32 Time::asSeconds() const {
    return Microseconds / 1000000.f;
}

inline i32 Time::asMilliSeconds() const {
    return static_cast<i32>(Microseconds / 1000);
}

inline i64 Time::asMicroSeconds() const {
    return Microseconds;
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

    glm::vec4 toVec4() const {
        glm::vec4 col = {};
        col.r = m_r;
        col.g = m_g;
        col.b = m_b;
        col.a = m_a;
        return col;
    }

    glm::vec3 toVec3() const {
        glm::vec3 col = {};
        col.r = m_r;
        col.g = m_g;
        col.b = m_b;
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
            x1(0), y1(0), x2(0), y2(0), width(0), height(0) {
        // empty
    }

    /// @brief  The class constructor with parameters
    /// @param[in] x       X coordinate of upper left corner.
    /// @param[in] y       Y coordinate of upper left corner.
    /// @param[in] width   The width of the rectangle.
    /// @param[in] height  The height of the rectangle.
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
    /// @param[in] x       X coordinate of upper left corner.
    /// @param[in] y       Y coordinate of upper left corner.
    /// @param[in] width_  The width of the rectangle.
    /// @param[in] height_ The height of the rectangle.
    void set(T x, T y, T width_, T height_) {
        x1 = x;
        y1 = y;
        width = width_;
        height = height_;
        x2 = x + width;
        y2 = y + height;
    }

    /// @brief  Will set the rectangle-geometry with the upper left corner and the lower right corner.
    /// @param[in] x1_     X coordinate of the upper left corner.
    /// @param[in] y1_     Y coordinate of the upper left corner.
    /// @param[in] x2_     X coordinate of the lower right corner.
    /// @param[in] y2_     Y coordinate of the lower right corner.
    void setEdges(T x1_, T y1_, T x2_, T y2_) {
        x1 = x1_;
        y1 = y1_;
        x2 = x2_;
        y2 = y2_;
        width = x2 - x1;
        height = y2 - y1;
    }

    /// @brief  Return the X-coordinate of the upper left corner.
    /// @return The x-coordinate.
    T getX1() const {
        return x1;
    }

    /// @brief  Return the y-coordinate of the upper left corner.
    /// @return The y-coordinate.
    T getY1() const {
        return y1;
    }

    /// @brief  Return the X-coordinate of the lower right corner.
    /// @return The x-coordinate.
    T getX2() const {
        return x2;
    }

    /// @brief  Return the Y-coordinate of the lower right corner.
    /// @return The y-coordinate.
    T getY2() const {
        return y2;
    }

    /// @brief  Return the width of the rect.
    /// @return The width.
    T getWidth() const {
        return width;
    }

    /// @brief  Return the height of the rect.
    /// @return The height.
    T getHeight() const {
        return height;
    }

    /// @brief  Returns true, if he point is in the rect.
    /// @param[in] pt   The 2D-point to check.
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

    ///	@brief  Will return the relative center of the rect.
    /// @return The relative center as a point.
    TPoint2<T> getRelativeCenter() const {
        TPoint2<T> c(width / 2, height / 2);
        return c;
    }

    ///	@brief  Will return the absolute center of the rect.
    /// @return The absolute center as a point.
    TPoint2<T> getAbsoluteCenter() const {
        TPoint2<T> c(x1+width / 2, y1+height / 2);
        return c;
    }
};

using Rect2ui = TRectangle<ui32>;
using Rect2i = TRectangle<i32>;

/// @brief  The resolution type
template <class T>
struct TResolution {
    T width;
    T height;

    TResolution(T w, T h) : width(w), height(h) {
        // empty
    }

    T getArea() const {
        return Width * Height;
    }
};

///	@brief  Shortcut to avoid copy operations.
#define OSRE_NON_COPYABLE(NAME)  \
private:                         \
    NAME(const NAME &) = delete; \
    NAME(const NAME &&) = delete; \
    NAME &operator=(const NAME &) = delete; \
    NAME &operator=(const NAME &&) = delete;


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

enum class LifetimeState {
    Invalid = -1,
    Inited,
    Created,
    Running,
    Detroyed,
    Error,
    Count
};

} // Namespace OSRE

void *operator new(size_t size);
void operator delete(void *ptr) noexcept;

#define osre_new(T) new T()
#define osre_new_array(T, len) new T[len]
#define osre_delete delete
#define osre_delete_array delete[]
