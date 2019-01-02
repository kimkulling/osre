/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Win32ThreadEvent.h"
#include <osre/Common/Logger.h>
#include <cassert>

namespace OSRE {
namespace Platform {

static const c8 *Tag = "Win32ThreadEvent";

Win32ThreadEvent::Win32ThreadEvent() 
: m_EventHandle( nullptr ) {
	m_EventHandle = ::CreateEvent( NULL, FALSE, FALSE, NULL );
	assert( nullptr != m_EventHandle );
}

Win32ThreadEvent::~Win32ThreadEvent() {
	if ( m_EventHandle ) {
		::CloseHandle( m_EventHandle );
		m_EventHandle = nullptr;
	}
}

void Win32ThreadEvent::signal() {
	if ( !m_EventHandle ) {
		osre_debug( Tag, "Signal handle is not valid." );
		return;
	}

	static_cast<void>( ::SetEvent( m_EventHandle ) );
}

void Win32ThreadEvent::waitForOne() {
	static_cast<void>( ::WaitForSingleObject( m_EventHandle, INFINITE ) );
}

void Win32ThreadEvent::waitForAll() {
    static_cast< void >( ::WaitForMultipleObjects( 1, &m_EventHandle, TRUE, INFINITE ) );
}

void Win32ThreadEvent::waitForTimeout( ui32 ms ) {
	ui32 timeout = ms;
	if ( 0 == ms ) {
		timeout = INFINITE;
	}
	static_cast<void>( ::WaitForSingleObject( m_EventHandle, timeout ) );
}

HANDLE Win32ThreadEvent::getHandle() const {
	return m_EventHandle;
}

} // Namespace PLatform
} // Namespace OSRE
