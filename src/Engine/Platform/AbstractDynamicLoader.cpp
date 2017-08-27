/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Platform/AbstractDynamicLoader.h>

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace Platform {

using namespace ::OSRE::Common;

static const i32 NotInitesHandle = -1;

LibHandle::LibHandle()
: m_index( NotInitesHandle )
, m_handle( nullptr ) {
    // empty
}

LibHandle::~LibHandle() {
    // empty
}

AbstractDynamicLoader::AbstractDynamicLoader()
: m_libmap()
, m_handles()
, m_activeLib( nullptr ) {
    // empty
}

AbstractDynamicLoader::~AbstractDynamicLoader() {
    for ( ui32 i = 0; i < m_handles.size(); i++ ) {
        delete m_handles[ i ];
    }
    m_libmap.clear();
}

void AbstractDynamicLoader::addLib( const String &libName, LibHandle *libHandle ) {
    if ( nullptr == libHandle ) {
        return;
    }

    libHandle->m_index = static_cast<i32>( m_handles.size() );
    m_handles.add( libHandle );
    ui32 key( Common::StringUtils::hashName( libName ) );
    m_libmap.insert( key, libHandle );
    setActiveLib( libHandle );
}

void AbstractDynamicLoader::removeLib( const String &libName ) {
    const ui32 key( StringUtils::hashName( libName ) );
    if ( m_libmap.hasKey( key ) ) {
        LibHandle *libHandle( nullptr );
        m_libmap.getValue( key, libHandle );
        if ( nullptr != libHandle ) {
            if ( NotInitesHandle != libHandle->m_index ) {
                m_handles[ static_cast<size_t>( libHandle->m_index ) ] = nullptr;
                if ( libHandle == getActiveLib() ) {
                    setActiveLib( nullptr );
                }
                m_libmap.remove( key );
                delete libHandle;
            }
        }
    }
}

LibHandle *AbstractDynamicLoader::findLib( const String &libName ) {
    if ( libName.empty() ) {
        return nullptr;
    }

    LibHandle *libHandle( nullptr );
    const ui32 key( Common::StringUtils::hashName( libName.c_str() ) );
    if ( m_libmap.hasKey( key ) ) {
        m_libmap.getValue( key, libHandle );
    }

    return libHandle;
}

void AbstractDynamicLoader::setActiveLib( LibHandle *handle ) {
    m_activeLib = handle;
}

LibHandle *AbstractDynamicLoader::getActiveLib() const {
    return m_activeLib;
}

} // Namespace Platform
} // Namespace OSRE
