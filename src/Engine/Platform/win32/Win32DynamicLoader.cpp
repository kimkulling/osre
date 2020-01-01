/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Platform/Windows/MinWindows.h>

namespace OSRE {
namespace Platform {

using namespace ::OSRE::Common;

Win32DynamicLoader::Win32DynamicLoader()
: AbstractDynamicLoader() {
    // empty
}

Win32DynamicLoader::~Win32DynamicLoader() {
}

LibHandle *Win32DynamicLoader::load( const String &libName ) {
    if ( libName.empty() ) {
        return nullptr;
    }

    LibHandle *libHandle( lookupLib( libName ) );
    if ( nullptr != libHandle ) {
        return libHandle;
    }

    HMODULE handle = ::LoadLibrary( libName.c_str() );
    if ( NULL != handle ) {
        libHandle = new LibHandle;
        libHandle->m_handle = ( void* ) handle;
        AbstractDynamicLoader::addLib( libName, libHandle );
        AbstractDynamicLoader::setActiveLib( libHandle );
    }

    return libHandle;
}
    
LibHandle *Win32DynamicLoader::lookupLib( const String &libName ) {
    LibHandle *libHandle( AbstractDynamicLoader::findLib( libName ) );
    AbstractDynamicLoader::setActiveLib( libHandle );

    return libHandle;
}

void Win32DynamicLoader::unload( const String &libName ) {
    AbstractDynamicLoader::removeLib( libName );
}
    
void *Win32DynamicLoader::loadFunction( const String &libName ) {
    if ( libName.empty() ) {
        return nullptr;
    }
    return ::GetProcAddress( ( HMODULE ) getActiveLib()->m_handle, libName.c_str() );
}

} // Namespace Platform
} // Namespace OSRE
