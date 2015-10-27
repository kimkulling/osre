/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
void SDL2ThreadEvent::waitForOne( ) {
    m_bool = SDL_FALSE;
    SDL_LockMutex( m_lock );
    while( !m_bool ) {
        SDL_CondWait( m_event, m_lock );
    }
    SDL_UnlockMutex( m_lock );
}

//-------------------------------------------------------------------------------------------------
void SDL2ThreadEvent::waitForAll() {
    m_bool = SDL_FALSE;
    SDL_LockMutex( m_lock );
    while (!m_bool) {
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
} // Namespace OSRE
