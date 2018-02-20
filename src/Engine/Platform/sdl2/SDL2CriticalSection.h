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

#include <osre/Platform/AbstractCriticalSection.h>

#include <SDL.h>
#include <SDL_atomic.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the critical section based on SDL2.
//-------------------------------------------------------------------------------------------------
class SDL2CriticalSection : public AbstractCriticalSection {
public:
    SDL2CriticalSection();
    virtual ~SDL2CriticalSection( );
    void enter() override;
    bool tryEnter() override;
    void leave() override; 

private:
    SDL_SpinLock m_spinlock;
};

inline
SDL2CriticalSection::SDL2CriticalSection()
: AbstractCriticalSection()
, m_spinlock( 0 ) {
    // empty
}

inline
SDL2CriticalSection::~SDL2CriticalSection() {
    SDL_AtomicUnlock( &m_spinlock );
}

inline
void SDL2CriticalSection::enter() {
    SDL_AtomicLock( &m_spinlock );
}

inline
bool SDL2CriticalSection::tryEnter() {
    if( SDL_TRUE == SDL_AtomicTryLock( &m_spinlock ) ) {
        return true;
    }

    return false;
}

inline
void SDL2CriticalSection::leave() {
    SDL_AtomicUnlock( &m_spinlock );
}

} // Namespace Platform
} // Namespace OSRE
