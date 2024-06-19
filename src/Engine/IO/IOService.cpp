/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "IO/IOService.h"
#include "Common/Tokenizer.h"
#include "Common/Logger.h"
#include "Engine/IO/ZipFileSystem.h"
#include "Engine/IO/LocaleFileSystem.h"

IMPLEMENT_SINGLETON( ::OSRE::IO::IOService )

namespace OSRE {
namespace IO {

using namespace OSRE::Common;

static constexpr c8 Tag[]           = "IOService";
static constexpr c8 Zip_Extension[] = "zip";

static AbstractFileSystem *createFS( const Uri &file ) {
    if ( !file.isValid() ) {
        return nullptr;
    }

    const String &schema( file.getScheme() );
    if( Zip_Extension == schema ) {
        return new ZipFileSystem( file );
    }

    return nullptr;
}

IOService::IOService() : AbstractService( "io/ioserver" ), m_mountedMap() {
    CREATE_SINGLETON( IOService );

    m_mountedMap["file"] = new LocaleFileSystem();
}

IOService::~IOService() {
    DESTROY_SINGLETON( IOService );
}

bool IOService::onOpen() {
    // create the locale file system
    AbstractFileSystem *pFileSystem( nullptr );    
    pFileSystem = new LocaleFileSystem;
    mountFileSystem( pFileSystem->getSchema(), pFileSystem );

    return true;
}

bool IOService::onClose() {
    for (MountedMap::iterator it = m_mountedMap.begin(); it != m_mountedMap.end(); ++it) {
        delete it->second;
    }
    return true;
}

bool IOService::onUpdate() {
    return true;
}

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

void IOService::mountFileSystem( const String &schema, AbstractFileSystem *pFileSystem ) {
    assert( nullptr != pFileSystem );

    m_mountedMap[ schema ] = pFileSystem;
}

void IOService::umountFileSystem( const String &schema, AbstractFileSystem *pFileSystem ) {
    assert( nullptr != pFileSystem );

    MountedMap::iterator it = m_mountedMap.find( schema );
    if ( m_mountedMap.end() == it ) {
        return;
    }
    if (it->second == pFileSystem) {
        m_mountedMap.erase(it);
    }
}

Stream *IOService::openStream( const Uri &file, Stream::AccessMode mode ) {
    Stream *pStream( nullptr );
    AbstractFileSystem *pFS = getFileSystem( file.getScheme() );
    if ( pFS ) {
        pStream  = pFS->open( file, mode );
    }

    return pStream;
}

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

bool IOService::fileExists( const Uri &file ) const {
    bool exists( false );
    AbstractFileSystem *fs = this->getFileSystem( file.getScheme() );
    if ( fs ) {
        exists = fs->fileExist( file );
    }

    return exists;
}

IOService *IOService::create() {
    return new IOService;
}

} // Namespace IO
} // Namespace OSRE
