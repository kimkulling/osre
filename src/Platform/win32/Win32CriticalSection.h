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
#pragma once

#include <osre/Platform/AbstractCriticalSection.h>
#include <osre/Debugging/osre_debugging.h>
#include <windows.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Threading::Win32CriticalSection
/// @ingroup    Engine
///
///	@brief  This class implements the win32-specific critical section.
//-------------------------------------------------------------------------------------------------
class Win32CriticalSection : public AbstractCriticalSection {
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
	OSRE_VALIDATE( InitializeCriticalSectionAndSpinCount( &m_CriticalSection, 1024 ), "Error while InitializeCriticalSectionAndSpinCount" );
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
} // Namespace OSRE
