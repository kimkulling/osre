#pragma once

#include <osre/Common/Types.h>
#include <osre/Platform/AtomicInt.h>

#include <SDL.h>
#include <SDL_atomic.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::SDL2Atomic
///	@ingroup	Infrastructure
///
///	@brief  This class implements the atomic int based on SDL2.
//-------------------------------------------------------------------------------------------------
class SDL2Atomic :public AbstractAtomic {
public:
    /// The class constructor.
    SDL2Atomic( i32 value );
    /// The class destructor.
    virtual ~SDL2Atomic();
    ///	Implements value increment.
    virtual void incValue( i32 value );
    ///	Implements value decrement.
    virtual void decValue( i32 value );
    ///	Implements value get.
    virtual i32 getValue();
    ///	Implements value increment.
    virtual i32 inc();
    ///	Implements value decrement.
    virtual i32 dec();

private:
    SDL_atomic_t m_value;
};

//-------------------------------------------------------------------------------------------------
inline
SDL2Atomic::SDL2Atomic( i32 value ) {
    SDL_AtomicSet( &m_value, value );
}

//-------------------------------------------------------------------------------------------------
inline
SDL2Atomic::~SDL2Atomic( ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline
void SDL2Atomic::incValue( i32 value ) {
    SDL_AtomicAdd( &m_value, value );
}

//-------------------------------------------------------------------------------------------------
inline
void SDL2Atomic::decValue( i32 value ) {
    SDL_AtomicAdd( &m_value, -value );
}

//-------------------------------------------------------------------------------------------------
inline
i32 SDL2Atomic::getValue( ) {
    return ( SDL_AtomicGet( &m_value ) );
}
        
//-------------------------------------------------------------------------------------------------
inline
i32 SDL2Atomic::inc( ) {
    SDL_AtomicIncRef( &m_value );
    return getValue();
}

//-------------------------------------------------------------------------------------------------
inline
i32 SDL2Atomic::dec( ) {
    SDL_AtomicDecRef( &m_value );
    return getValue();
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
