/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Platform/AbstractThreadLocalStorage.h>
#include <osre/Debugging/osre_debugging.h>
#include <windows.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Platform::Win32ThreadLocalStorage
///	@ingroup	Engine
///
///	@brief This class implements the Win32-based Thread Local Storage API.
//-------------------------------------------------------------------------------------------------
class Win32ThreadLocalStorage : public AbstractThreadLocalStorage {
public:
    Win32ThreadLocalStorage();
    virtual ~Win32ThreadLocalStorage() override;
    virtual void *get() override;
    virtual void set( void *data ) override;

private:
    DWORD m_index;
};

inline
Win32ThreadLocalStorage::Win32ThreadLocalStorage()
: AbstractThreadLocalStorage()
, m_index( TLS_OUT_OF_INDEXES ){
    m_index = ::TlsAlloc();
    OSRE_ASSERT( TLS_OUT_OF_INDEXES != m_index );
}
    
inline
Win32ThreadLocalStorage::~Win32ThreadLocalStorage() {
    OSRE_ASSERT( TLS_OUT_OF_INDEXES != m_index );

    ::TlsFree( m_index );
}
    
inline
void *Win32ThreadLocalStorage::get() {
    OSRE_ASSERT( TLS_OUT_OF_INDEXES != m_index );

    return ::TlsGetValue( m_index );
}
    
inline
void Win32ThreadLocalStorage::set( void *data ) {
    OSRE_ASSERT( TLS_OUT_OF_INDEXES != m_index );

    ::TlsSetValue( m_index, data );
}

} // Namespace Platform
} // Namespace OSRE
