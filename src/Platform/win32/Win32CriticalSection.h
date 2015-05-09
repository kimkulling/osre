#pragma once

#include <osre/Platform/AbstractCriticalSection.h>
#include <windows.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Threading::Win32CriticalSection
///	@ingroup	Infrastructure
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
