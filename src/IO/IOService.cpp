/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-2015, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the 
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions 
  and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
  and the following disclaimer in the documentation and/or other materials provided with the 
  distribution.

* Neither the name of the ZFXCE2 team, nor the names of its contributors may be used to endorse or 
  promote products derived from this software without specific prior written permission of the 
  ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
#include <osre/IO/IOService.h>
#include <osre/Common/Tokenizer.h>
#include <osre/Common/Logger.h>

// private header
#include <src/IO/ZipFileSystem.h>

#include "LocaleFileSystem.h"

#include <algorithm>

namespace OSRE {
namespace IO {

using namespace OSRE::Common;

IMPLEMENT_SINGLETON( IOService )

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
            osre_debug("Cannot create file system " + file.getResource() );
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
