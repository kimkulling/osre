/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011, ZFXCE2 Development Team
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
#include "Win32Timer.h"

namespace ZFXCE2 {
namespace Platform {

//-------------------------------------------------------------------------------------------------
Win32Timer::Win32Timer() 
: AbstractTimer( "platform/win32timer" )
, m_globeTime()
, m_globeFrequency()
, m_LastTime( 0.0 ) {
	::QueryPerformanceCounter( &m_globeTime );
	::QueryPerformanceFrequency( &m_globeFrequency );
}

//-------------------------------------------------------------------------------------------------
Win32Timer::~Win32Timer() {
	// empty
}

//-------------------------------------------------------------------------------------------------
d32 Win32Timer::getCurrentSeconds() {
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter( &currentTime );
	const d32 secs = static_cast<d32>( ( currentTime.QuadPart - m_globeTime.QuadPart ) ) / static_cast<d32>(m_globeFrequency.QuadPart );

	return secs;
}

//-------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------

} // Namespace System
} // Namespace ZFXCE2
