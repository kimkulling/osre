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
#pragma once
#ifndef ZFXCE2_INFRASTRUCTURE_PLATFORM_WIN32CRITICALSECTION_H_INC
#define ZFXCE2_INFRASTRUCTURE_PLATFORM_WIN32CRITICALSECTION_H_INC

#include <osre2/Infrastructure/Platform/AbstractCriticalSection.h>
#include <windows.h>

namespace ZFXCE2 {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Threading::Win32CriticalSection
///	@ingroup	Infrastructure
///
///	@brief  This class implements the win32-specific critical section.
//-------------------------------------------------------------------------------------------------
class DLL_EXPORT Win32CriticalSection : public AbstractCriticalSection {
public:
	///	The class constructor.
	Win32CriticalSection();
	///	The class destructor.
	~Win32CriticalSection();
	///	Enters the critical section.
	void enter();
	///	Tries to enter the critical section.
	bool tryEnter();
	///	Leaves the critical section.
	void leave();

private:
	CRITICAL_SECTION m_CriticalSection;
};

//-------------------------------------------------------------------------------------------------
inline
Win32CriticalSection::Win32CriticalSection() {
	InitializeCriticalSectionAndSpinCount( &m_CriticalSection, 1024 );
}

//-------------------------------------------------------------------------------------------------
inline
Win32CriticalSection::~Win32CriticalSection() {
	DeleteCriticalSection( &m_CriticalSection );
}

//-------------------------------------------------------------------------------------------------
inline
void Win32CriticalSection::enter() {
	::EnterCriticalSection( const_cast<LPCRITICAL_SECTION>( &m_CriticalSection ) );
}

//-------------------------------------------------------------------------------------------------
inline
bool Win32CriticalSection::tryEnter() {
	if ( 0 != ::TryEnterCriticalSection( &m_CriticalSection ) ) {
		return true;
	}

	return false;
}

//-------------------------------------------------------------------------------------------------
inline
void Win32CriticalSection::leave() {
	::LeaveCriticalSection( const_cast<LPCRITICAL_SECTION>( &m_CriticalSection ) );
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2

#endif // ZFXCE2_INFRASTRUCTURE_PLATFORM_WIN32CRITICALSECTION_H_INC
