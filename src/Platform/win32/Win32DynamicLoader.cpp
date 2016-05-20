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

#include "Win32DynamicLoader.h"
#include <osre/Common/StringUtils.h>

#include <windows.h>

namespace OSRE {
namespace Platform {

using namespace ::OSRE::Common;

Win32DynamicLoader::Win32DynamicLoader()
: AbstractDynamicLoader()
, m_libmap()
, m_handles() {
    // empty
}

Win32DynamicLoader::~Win32DynamicLoader() {
    for ( ui32 i = 0; i < m_handles.size(); i++ ) {
        delete m_handles[ i ];
    }
    m_libmap.clear();
}

LibHandle *Win32DynamicLoader::load( const c8 *libName ) {
    if ( nullptr == libName ) {
        return nullptr;
    }
    
    LibHandle *libHandle( lookupLib( libName ) );
    if ( nullptr != libHandle ) {
        return libHandle;
    }

    HMODULE handle = ::LoadLibrary( libName );
    if ( NULL != handle ) {
        libHandle = new LibHandle;
        m_handles.add( libHandle );
        libHandle->m_handle = ( void* ) handle;
        ui32 key( StringUtils::hashName( libName ) );
        m_libmap.insert( key, libHandle );
    }

    return libHandle;
}
    
LibHandle *Win32DynamicLoader::lookupLib( const c8 *libName ) {
    if ( nullptr == libName ) {
        return nullptr;
    }

    LibHandle *libHandle( nullptr );
    const ui32 key( StringUtils::hashName( libName ) );
    if ( m_libmap.hasKey( key ) ) {
        m_libmap.getValue( key, libHandle );
    }

    return libHandle;
}

void Win32DynamicLoader::unload( const c8 *libName ) {
    if ( nullptr == libName ) {
        return;
    }

    const ui32 key( StringUtils::hashName( libName ) );
    if ( m_libmap.hasKey( key ) ) {
        LibHandle *libHandle( nullptr );
        m_libmap.getValue( key, libHandle );
        m_libmap.remove( key );
        delete libHandle;
    }
}
    
void *Win32DynamicLoader::loadFunction( const char *name ) {
    return nullptr;
}

} // Namespace Platform
} // Namespace OSRE
