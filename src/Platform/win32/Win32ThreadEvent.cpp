#include "Win32ThreadEvent.h"
#include <osre/Common/Logger.h>
#include <cassert>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
Win32ThreadEvent::Win32ThreadEvent() 
: m_EventHandle( nullptr ) {
	m_EventHandle = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	assert( nullptr != m_EventHandle );
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
		osre_debug( "Signal handle is not valid." );
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
