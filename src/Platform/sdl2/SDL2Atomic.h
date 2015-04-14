#pragma once

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
#ifndef ZFXCE2_INFRASTRUCTURE_PLATFORM_SDL2ATOMIC_H_INC
#define ZFXCE2_INFRASTRUCTURE_PLATFORM_SDL2ATOMIC_H_INC

#include <osre2/Types.h>
#include <osre2/Infrastructure/Platform/AtomicInt.h>

#include <SDL.h>
#include <SDL_atomic.h>

namespace ZFXCE2 {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::SDL2Atomic
///	@ingroup	Infrastructure
///
///	@brief  This class implements the atomic int based on SDL2.
//-------------------------------------------------------------------------------------------------
class DLL_EXPORT SDL2Atomic :public AbstractAtomic {
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

#endif // ZFXCE2_INFRASTRUCTURE_PLATFORM_SDL2ATOMIC_H_INC
