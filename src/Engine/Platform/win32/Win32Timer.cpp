/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Win32Timer.h"

namespace OSRE {
namespace Platform {

Win32Timer::Win32Timer() 
: AbstractTimer( "platform/win32timer" )
, m_globeTime()
, m_globeFrequency()
, m_LastTime( 0.0 ) {
    ::QueryPerformanceCounter( &m_globeTime );
    ::QueryPerformanceFrequency( &m_globeFrequency );
}

Win32Timer::~Win32Timer() {
    // empty
}

d32 Win32Timer::getCurrentSeconds() {
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter( &currentTime );
    const d32 secs = static_cast<d32>( ( currentTime.QuadPart - m_globeTime.QuadPart ) ) 
        / static_cast<d32>(m_globeFrequency.QuadPart );

    return secs;
}

d32 Win32Timer::getTimeDiff() {
    d32 currentTime = getCurrentSeconds();
    if ( m_LastTime == 0.0 ) {
        m_LastTime = currentTime;
        return 0.0;
    } else {
        d32 diff( currentTime - m_LastTime );
        if( diff > 1000.0 ) {
            diff = AbstractTimer::getRequestedTimeStep();
        }
        m_LastTime = currentTime;
        return diff;
    }
}


} // Namespace Platform
} // Namespace OSRE
