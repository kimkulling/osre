#pragma once

#include <osre/Platform/AbstractThreadLocalStorage.h>
#include <osre/Debugging/osre_debugging.h>

#include <SDL.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Platform::SDL2ThreadLocalStorage
///	@ingroup	Engine
///
///	@brief This class implements the SDL"-based Thread Local Storage API.
//-------------------------------------------------------------------------------------------------
class SDL2ThreadLocalStorage : public AbstractThreadLocalStorage {
public:
    SDL2ThreadLocalStorage();
    virtual ~SDL2ThreadLocalStorage();
    void *get() override;
    void set( void *data ) override;

private:
    SDL_TLSID m_index;
};

inline
SDL2ThreadLocalStorage::SDL2ThreadLocalStorage()
: AbstractThreadLocalStorage()
, m_index( 99999999 ) {
    // empty
}

inline
SDL2ThreadLocalStorage::~SDL2ThreadLocalStorage() {
    // empty
}

inline
void *SDL2ThreadLocalStorage::get() {
    OSRE_ASSERT( 99999999 != m_index );

    return ::SDL_TLSGet( m_index );
}

inline
void SDL2ThreadLocalStorage::set( void *data ) {
    OSRE_ASSERT( 99999999 != m_index );

    ::SDL_TLSSet( m_index, data, 0 );
}

} // Namespace Platform
} // Namespace OSRE
