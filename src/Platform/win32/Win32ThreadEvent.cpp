/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-2015, ZFXCE2 Development Team
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
#include "Win32ThreadEvent.h"
#include <osre2/Infrastructure/Debugging/ce_assert.h>
#include <osre2/Infrastructure/Common/Logger.h>

namespace ZFXCE2 {
namespace Platform {

//-------------------------------------------------------------------------------------------------
Win32ThreadEvent::Win32ThreadEvent() 
: m_EventHandle( nullptr ) {
	m_EventHandle = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	ce_assert( nullptr != m_EventHandle );
}

//-------------------------------------------------------------------------------------------------
Win32ThreadEvent::~Win32ThreadEvent() {
	if ( m_EventHandle ) {
		::CloseHandle( m_EventHandle );
		m_EventHandle = nullptr;
	}
}

//-------------------------------------------------------------------------------------------------
void Win32ThreadEvent::signal() {
	if ( !m_EventHandle ) {
		ce_debug( "Signal handle is not valid." );
		return;
	}

	static_cast<void>( ::SetEvent( m_EventHandle ) );
}

//-------------------------------------------------------------------------------------------------
void Win32ThreadEvent::wait() {
	static_cast<void>( ::WaitForSingleObject( m_EventHandle, INFINITE ) );
}

//-------------------------------------------------------------------------------------------------
void Win32ThreadEvent::waitForTimeout( ui32 ms ) {
	ui32 timeout = ms;
	if ( 0 == ms ) {
		timeout = INFINITE;
	}
	static_cast<void>( ::WaitForSingleObject( m_EventHandle, timeout ) );
}

//-------------------------------------------------------------------------------------------------
HANDLE Win32ThreadEvent::getHandle() const {
	return m_EventHandle;
}

//-------------------------------------------------------------------------------------------------

} // Namespace PLatform
} // Namespace ZFXCE2
