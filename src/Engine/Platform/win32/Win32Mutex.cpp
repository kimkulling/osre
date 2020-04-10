/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "src/Engine/Platform/win32/Win32Mutex.h"
#include <osre/Debugging/osre_debugging.h>

namespace OSRE {
namespace Platform {

Win32Mutex::Win32Mutex(ui32 timeout)
: AbstractMutex()
, m_handle( nullptr )
, m_timeout( timeout ) {
    m_handle = ::CreateMutex(NULL, FALSE, NULL);
}

Win32Mutex::~Win32Mutex() {
    ::CloseHandle(m_handle);
    m_handle = NULL;
}

void Win32Mutex::lock() {
    OSRE_ASSERT(nullptr != m_handle);
    
    ::WaitForSingleObject(m_handle, INFINITE);
}

bool Win32Mutex::tryLock() {
    OSRE_ASSERT(nullptr != m_handle);
    
    bool ok(false);
    if (::WaitForSingleObject(m_handle, m_timeout)) {
        ok = true;
    }

    return ok;
}

bool Win32Mutex::unlock() {
    OSRE_ASSERT(nullptr != m_handle);
    
    bool ok(false);
    HRESULT hr = ::ReleaseMutex(m_handle);
    if (NULL != hr) {
        ok = true;
    }

    return ok;
}

} // Namespace Platform
} // Namespace OSRE
