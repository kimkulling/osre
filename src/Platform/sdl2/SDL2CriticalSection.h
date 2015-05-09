#pragma once

#include <osre/Platform/AbstractCriticalSection.h>

#include <SDL.h>
#include <SDL_atomic.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::SDL2CriticalSection
///	@ingroup	Infrastructure
///
///	@brief  This class implements the critical section based on SDL2.
//-------------------------------------------------------------------------------------------------
class SDL2CriticalSection : public AbstractCriticalSection {
public:
    SDL2CriticalSection();
    virtual ~SDL2CriticalSection( );
    virtual void enter();
    virtual bool tryEnter();
    virtual void leave();

private:
    SDL_SpinLock m_spinlock;
};

//-------------------------------------------------------------------------------------------------
inline
SDL2CriticalSection::SDL2CriticalSection()
: AbstractCriticalSection()
, m_spinlock( 0 ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline
SDL2CriticalSection::~SDL2CriticalSection() {
    SDL_AtomicUnlock( &m_spinlock );
}

//-------------------------------------------------------------------------------------------------
inline
void SDL2CriticalSection::enter() {
    SDL_AtomicLock( &m_spinlock );
}

//-------------------------------------------------------------------------------------------------
inline
bool SDL2CriticalSection::tryEnter() {
    if( SDL_TRUE == SDL_AtomicTryLock( &m_spinlock ) ) {
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------
inline
void SDL2CriticalSection::leave() {
    SDL_AtomicUnlock( &m_spinlock );
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
