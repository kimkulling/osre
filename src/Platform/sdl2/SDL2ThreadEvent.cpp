#include "SDL2ThreadEvent.h"
#include <osre/Common/Logger.h>
#include "SDL_thread.h"
#include <cassert>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
SDL2ThreadEvent::SDL2ThreadEvent()
: m_bool( SDL_FALSE )
, m_lock( nullptr )
, m_event( nullptr ) {
    m_lock = SDL_CreateMutex();
    m_event = SDL_CreateCond();
}

//-------------------------------------------------------------------------------------------------
SDL2ThreadEvent::~SDL2ThreadEvent( ) {
    SDL_DestroyCond( m_event );
    m_event = nullptr;

    SDL_DestroyMutex( m_lock );
    m_lock = nullptr;
}

//-------------------------------------------------------------------------------------------------
void SDL2ThreadEvent::signal( ) {
    SDL_LockMutex( m_lock );
    m_bool = SDL_TRUE;
    SDL_CondSignal( m_event );
    SDL_UnlockMutex( m_lock );
}

//-------------------------------------------------------------------------------------------------
void SDL2ThreadEvent::wait( ) {
    m_bool = SDL_FALSE;
    SDL_LockMutex( m_lock );
    while( !m_bool ) {
        SDL_CondWait( m_event, m_lock );
    }
    SDL_UnlockMutex( m_lock );
}

//-------------------------------------------------------------------------------------------------
void SDL2ThreadEvent::waitForTimeout( ui32 ms ) {
    // todo!
}

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2
