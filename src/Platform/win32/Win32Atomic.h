/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-2015, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions
and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
and the following disclaimer in the documentation and/or other materials provided with the
distribution.

* Neither the name of the ZFXCE2 team, nor the names of its contributors may be used to endorse or
promote products derived from this software without specific prior written permission of the
ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
#pragma once
#ifndef ZFXCE2_INFRASTRUCTURE_PLATFORM_WIN32ATOMIC_H_INC
#define ZFXCE2_INFRASTRUCTURE_PLATFORM_WIN32ATOMIC_H_INC

#include <osre2/Types.h>
#include <osre2/Infrastructure/Platform/AtomicInt.h>

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


namespace ZFXCE2 {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Platform::AtomicInt
///	@ingroup	Infrastructure
///
///	@brief
//-------------------------------------------------------------------------------------------------
class DLL_EXPORT Win32Atomic : public AbstractAtomic {
public:
    Win32Atomic( i32 val );
    virtual ~Win32Atomic();
    virtual void incValue( i32 value );
    virtual void decValue( i32 value );
    virtual i32 getValue( );
    virtual i32 inc( );
    virtual i32 dec( );

private:
    mutable long m_value;
};

//-------------------------------------------------------------------------------------------------
inline
Win32Atomic::Win32Atomic( i32 val ) 
: m_value( val ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline
Win32Atomic::~Win32Atomic( ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline
void Win32Atomic::incValue( i32 value ) {
    static_cast< void >( _InterlockedExchangeAdd( &m_value, value ) );
}

//-------------------------------------------------------------------------------------------------
inline
void Win32Atomic::decValue( i32 value ) {
    static_cast< void >( _InterlockedExchangeAdd( &m_value, -1 * value ) );
}

//-------------------------------------------------------------------------------------------------
inline
i32 Win32Atomic::getValue( ) {
    return _InterlockedExchangeAdd( &m_value, 0 );
}

//-------------------------------------------------------------------------------------------------
inline
i32 Win32Atomic::inc() {
    return _InterlockedIncrement( &m_value );
}

//-------------------------------------------------------------------------------------------------
inline
i32 Win32Atomic::dec() {
    return _InterlockedDecrement( &m_value );
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2

#endif // ZFXCE2_INFRASTRUCTURE_PLATFORM_WIN32ATOMIC_H_INC
