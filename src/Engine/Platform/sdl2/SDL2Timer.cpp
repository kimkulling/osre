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
#include <src/Engine/Platform/sdl2/SDL2Timer.h>

#include <SDL.h>

namespace OSRE {
namespace Platform {

SDL2Timer::SDL2Timer()
: AbstractTimer( "platform/sdl2timer" )
, m_lasttick( 0 ){
    // empty
}

SDL2Timer::~SDL2Timer( ) {
    // empty
}

i64 SDL2Timer::getMilliCurrentSeconds( ) {
    i64 ticks =  static_cast<i64>( SDL_GetTicks() / 1000U );
    return ticks;
}

i64 SDL2Timer::getTimeDiff( ) {
    i64 currentTime = getMilliCurrentSeconds();
    if ( m_lasttick == 0 ) {
        m_lasttick = currentTime;
        return 0;
    } else {
        i64 diff( currentTime - m_lasttick );
        if( diff > 1000 ) {
            diff = AbstractTimer::getRequestedTimeStep();
        }
        m_lasttick = currentTime;
        
        return diff;
    }
}

} // Namespace Platform
} // Namespace OSRE
