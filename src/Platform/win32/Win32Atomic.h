#pragma once

#include <osre/Common/Types.h>
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
///	@class		ZFXCE2::Platform::AtomicInt
///	@ingroup	Infrastructure
///
///	@brief
//-------------------------------------------------------------------------------------------------
class Win32Atomic : public AbstractAtomic {
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
