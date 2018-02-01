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
#include <osre/Platform/AtomicInt.h>

#include <intrin.h>
        
extern "C" long  __cdecl _InterlockedCompareExchange( long volatile* a_vpDest, long a_Exchange, long a_Comp );
extern "C" long  __cdecl _InterlockedExchange( long volatile* a_vpTarget, long a_Value );
extern "C" long  __cdecl _InterlockedExchangeAdd( long volatile* a_vpAddend, long a_Value );
extern "C" long  __cdecl _InterlockedIncrement( long volatile* a_vpAddend );
extern "C" long  __cdecl _InterlockedDecrement( long volatile* a_vpAddend );

#pragma intrinsic (_InterlockedCompareExchange)
#pragma intrinsic (_InterlockedExchange)
#pragma intrinsic (_InterlockedExchangeAdd)
#pragma intrinsic (_InterlockedIncrement)
#pragma intrinsic (_InterlockedDecrement)

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
/// @ingroup    Engine
///
///	@brief This class implements the atomic into with intrinsics from Win32-API. 
//-------------------------------------------------------------------------------------------------
class Win32Atomic : public AbstractAtomic {
public:
    /// @brief The class constructor.
    /// @param val   [in] The initial vale.
    Win32Atomic( i32 val );
    
    /// @brief  The class destructor.
    virtual ~Win32Atomic() override;

    /// @brief Increments the value by the given value.
    /// @param val   [in] The initial vale.
    virtual void incValue( i32 value ) override;


    /// @brief Decrements the value by the given value.
    /// @param val   [in] The initial vale.
    virtual void decValue( i32 value ) override;

    /// @brief returns the current value.
    /// @return The result.
    virtual i32 getValue() override;

    /// @brief Increments the value and returns the result.
    /// @return The result.
    virtual i32 inc() override;
    
    /// @brief Decrements the value and returns the result.
    /// @return The result.
    virtual i32 dec() override;

private:
    mutable long m_value;
};

inline
Win32Atomic::Win32Atomic( i32 val ) 
: m_value( val ) {
    // empty
}

inline
Win32Atomic::~Win32Atomic( ) {
    // empty
}

inline
void Win32Atomic::incValue( i32 value ) {
    static_cast< void >( _InterlockedExchangeAdd( &m_value, value ) );
}

inline
void Win32Atomic::decValue( i32 value ) {
    static_cast< void >( _InterlockedExchangeAdd( &m_value, -1 * value ) );
}

inline
i32 Win32Atomic::getValue( ) {
    return _InterlockedExchangeAdd( &m_value, 0 );
}

inline
i32 Win32Atomic::inc() {
    return _InterlockedIncrement( &m_value );
}

inline
i32 Win32Atomic::dec() {
    return _InterlockedDecrement( &m_value );
}

} // Namespace Platform
} // Namespace OSRE
