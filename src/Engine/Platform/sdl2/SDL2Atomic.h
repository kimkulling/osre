/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <SDL.h>
#include <SDL_atomic.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the atomic int based on SDL2.
//-------------------------------------------------------------------------------------------------
class SDL2Atomic : public AbstractAtomic {
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

inline
SDL2Atomic::SDL2Atomic( i32 value ) {
    SDL_AtomicSet( &m_value, value );
}

inline
SDL2Atomic::~SDL2Atomic( ) {
    // empty
}

inline
void SDL2Atomic::incValue( i32 value ) {
    SDL_AtomicAdd( &m_value, value );
}

inline
void SDL2Atomic::decValue( i32 value ) {
    SDL_AtomicAdd( &m_value, -value );
}

inline
i32 SDL2Atomic::getValue( ) {
    return ( SDL_AtomicGet( &m_value ) );
}
        
inline
i32 SDL2Atomic::inc( ) {
    SDL_AtomicIncRef( &m_value );
    return getValue();
}

inline
i32 SDL2Atomic::dec( ) {
    SDL_AtomicDecRef( &m_value );
    return getValue();
}

} // Namespace Platform
} // Namespace OSRE
