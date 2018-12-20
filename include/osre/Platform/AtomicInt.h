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

#include <osre/Platform/PlatformCommon.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This is the abstract base class for all atomic implementations.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractAtomic {
public:
    /// @brief  The class destructor, virtual.
    virtual ~AbstractAtomic();

    /// @brief  Increments the int.
    /// @param  value   [in] Increment value.
    virtual void incValue( i32 value ) = 0;

    /// @brief  Decrements the int.
    /// @param  value   [in] Decrement value.
    virtual void decValue( i32 value ) = 0;
    
    /// @brief  Will return the atomic in value.
    /// @return The value.
    virtual i32 getValue( ) = 0;
    
    /// @brief  Increment by one.
    /// @return The new value
    virtual i32 inc() = 0;

    /// @brief  Decrement by one.
    /// @return The new value
    virtual i32 dec() = 0;
};

inline
AbstractAtomic::~AbstractAtomic() {
    // empty
}

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  Implements the accessor for an atomic operation.
//-------------------------------------------------------------------------------------------------
class AtomicInt {
public:
    /// @brief  The class constructor.
    /// @param  val     [in] The initial value.
    AtomicInt( i32 val );

    /// @brief  The class destructor.
    ~AtomicInt();

    /// @brief  increments the int.
    /// @param  value   [in] Increment value.
    void incValue( i32 value );

    /// @brief  Decrements the int.
    /// @param  value   [in] Decrement value.
    void decValue( i32 value );
    
    /// @brief  Returns the value.
    /// @return The value.
    i32 getValue() const;
    
    /// @brief  ++ op impl.
    i32 operator ++ ();

    /// @brief  ++ op impl.
    i32 operator -- ( );

private:
    AbstractAtomic *m_pImpl;
};

} // Namespace Platform
} // Namespace OSRE
