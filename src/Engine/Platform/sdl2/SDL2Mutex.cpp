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
#include "src/Engine/Platform/sdl2/SDL2Mutex.h"
#include <osre/Debugging/osre_debugging.h>

#include <SDL.h>

namespace OSRE {
namespace Platform {

SDL2Mutex::SDL2Mutex()
: AbstractMutex()
, m_mutex( nullptr ) {
    m_mutex = SDL_CreateMutex();
}

SDL2Mutex::~SDL2Mutex() {
    SDL_DestroyMutex(m_mutex);
    m_mutex = nullptr;
}

void SDL2Mutex::lock() {
    OSRE_ASSERT(nullptr != m_mutex);

    static_cast<void>(SDL_LockMutex(m_mutex));
}

bool SDL2Mutex::tryLock() {
    OSRE_ASSERT(nullptr != m_mutex);

    bool ok(false);
    i32 status = SDL_TryLockMutex(m_mutex);
    if (status == 0) {
        ok = true;
    } 

    return ok;
}

bool SDL2Mutex::unlock() {
    OSRE_ASSERT(nullptr != m_mutex);

    return 0 == SDL_UnlockMutex(m_mutex);
}

} // Namespace Platform
} // Namespace OSRE

