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
#include <osre/IO/IOService.h>
#include <osre/Common/Tokenizer.h>
#include <osre/Common/Logger.h>
#include <src/Engine/IO/ZipFileSystem.h>

#include "LocaleFileSystem.h"

#include <algorithm>

IMPLEMENT_SINGLETON( ::OSRE::IO::IOService )

namespace OSRE {
namespace IO {

using namespace OSRE::Common;
//IOService * IOService::s_instance = nullptr;

static const String Tag = "IOService";

//-------------------------------------------------------------------------------------------------
static AbstractFileSystem *createFS( const Uri &file ) {
    const String &schema( file.getScheme() );
    if( schema == "zip" ) {
        return new ZipFileSystem( file );
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------
IOService::IOService() 
: AbstractService( "io/ioserver" )
, m_mountedMap() {
    CREATE_SINGLETON( IOService );
}

//-------------------------------------------------------------------------------------------------
IOService::~IOService() {
    DESTROY_SINGLETON( IOService );
}

//-------------------------------------------------------------------------------------------------
bool IOService::onOpen() {
    // create the locale file system
    AbstractFileSystem *pFileSystem( nullptr );    
    pFileSystem = new LocaleFileSystem;
    mountFileSystem( pFileSystem->getSchema(), pFileSystem );

    return true;
}

//-------------------------------------------------------------------------------------------------
bool IOService::onClose() {
    return true;
}

//-------------------------------------------------------------------------------------------------
bool IOService::onUpdate( d32 timeDiff ) {
    return true;
}

//-------------------------------------------------------------------------------------------------
AbstractFileSystem *IOService::addFileSystem( const String &name, const Uri &file ) {
    if( file.isEmpty() || !file.isValid() ) {
        return nullptr;
    }

    AbstractFileSystem *fs( nullptr );
    if( fileExists( file ) ) {
        fs = createFS( file );
        if( fs ) {
            m_mountedMap[ name ] = fs;
        } else {
            osre_debug( Tag, "Cannot create file system " + file.getResource() );
        }
    }

    return fs;
}

//-------------------------------------------------------------------------------------------------
void IOService::mountFileSystem( const String &schema, AbstractFileSystem *pFileSystem ) {
    assert( nullptr != pFileSystem );

    m_mountedMap[ schema ] = pFileSystem;
}

//-------------------------------------------------------------------------------------------------
void IOService::umountFileSystem( const String &schema, AbstractFileSystem *pFileSystem ) {
    assert( nullptr != pFileSystem );

    MountedMap::iterator it = m_mountedMap.find( schema );
    if ( m_mountedMap.end() == it ) {
        return;
    }

    m_mountedMap.erase( it );
}

//-------------------------------------------------------------------------------------------------
Stream *IOService::openStream( const Uri &file, Stream::AccessMode mode ) {
    Stream *pStream( nullptr );
    AbstractFileSystem *pFS = getFileSystem( file.getScheme() );
    if ( pFS ) {
        pStream  = pFS->open( file, mode );
    }

    return pStream;
}

//-------------------------------------------------------------------------------------------------
void IOService::closeStream( Stream **ppStream ) {
    if( nullptr == ppStream ) {
        return;
    }
    
    const String &schema( (*ppStream)->getUri().getScheme() );
    AbstractFileSystem *pFS = getFileSystem( schema );
    if( pFS ) {
        pFS->close( ppStream );
    }
}

//-------------------------------------------------------------------------------------------------
AbstractFileSystem *IOService::getFileSystem( const String &schema ) const {
    if ( m_mountedMap.empty() ) {
        return nullptr;
    }

    MountedMap::const_iterator it = m_mountedMap.find( schema );
    if ( m_mountedMap.end() != it ) {
        return it->second;
    } 

    return nullptr;
}

//-------------------------------------------------------------------------------------------------
bool IOService::fileExists( const Uri &file ) const {
    bool exists( false );
    AbstractFileSystem *pFS = this->getFileSystem( file.getScheme() );
    if ( pFS ) {
        exists = pFS->fileExist( file );
    }

    return exists;
}

//-------------------------------------------------------------------------------------------------
IOService *IOService::create() {
    return new IOService;
}

//-------------------------------------------------------------------------------------------------
bool IOService::normalizePath( const String &path, const c8 sep, String &normalized ) {
    normalized = "";
    if ( path.empty() ) {
        return true;
    }
    normalized = path;
    std::replace( normalized.begin(), normalized.end(), sep, '/');
    
    return true;
}

//-------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
