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
#include "LocaleFileSystem.h"
#include "FileStream.h"

#include <osre/Common/Logger.h>
#include <cassert>

#ifdef OSRE_WINDOWS
#  include <osre/Platform/Windows/MinWindows.h>
#else
#   include <stdio.h>
#   include <unistd.h>
#   include <string.h>
#endif

namespace OSRE {
namespace IO {

using namespace ::OSRE::Common;
using namespace ::CPPCore;

static const c8 *BaseFileSchema = "file";
static const c8 *Tag = "IOService";

LocaleFileSystem::LocaleFileSystem() 
: m_FileMap() {
    // empty
}

LocaleFileSystem::~LocaleFileSystem() {
    for ( StreamMap::iterator it = m_FileMap.begin(); it != m_FileMap.end(); ++it )	{
        if ( it->second ) {
            delete it->second;
        }
    }
    m_FileMap.clear();
}

Stream *LocaleFileSystem::open( const Uri &file, Stream::AccessMode mode ) {
    if ( file.isEmpty() ) {
        osre_debug( Tag, "Uri is empty." );
        return nullptr;
    }

    Stream *pFileStream( nullptr );
    String::size_type pos = file.getResource().rfind( "xml" );
    if ( String::npos == pos ) {
        pFileStream = new FileStream( file, mode );
    }

    if ( nullptr == pFileStream ) {
        return nullptr;
    }

    if ( pFileStream->open() ) {
        m_FileMap[ file.getResource() ] = pFileStream;
    } else {
        delete pFileStream;
        pFileStream = nullptr;
    }

    return pFileStream;
}

void LocaleFileSystem::close( Stream **pFile ) {
    if (nullptr == pFile) {
        return;
    }

    if ( ( *pFile )->isOpen() ) {
        ( *pFile )->close();
    }

    const Uri &rFile = (*pFile)->getUri();
    StreamMap::iterator it = m_FileMap.find( rFile.getResource() );
    m_FileMap.erase( it );
    (*pFile) = nullptr;
}

bool LocaleFileSystem::fileExist( const Uri &filename ) {
    if( filename.isEmpty() ) {
        return false;
    }

    bool exists( false );
    FILE *pFileStream( nullptr );
#ifdef OSRE_WINDOWS
    errno_t err = ::fopen_s( &pFileStream, filename.getAbsPath().c_str(), "r" );
    if ( 0 != err ) {
        osre_debug( Tag, "Error while opening the file." );
    }
#else
    pFileStream = ::fopen(  filename.getAbsPath().c_str(), "r" );
#endif

    if ( pFileStream ) {
        exists = true;
        ::fclose( pFileStream );
    }

    return exists;
}

Stream *LocaleFileSystem::find( const Uri &rFile, Stream::AccessMode mode,  TArray<String> *pSearchPaths ) {
    if ( !pSearchPaths ) {
        return nullptr;
    }

    Stream *pStream( nullptr );
    for ( ui32 i=0; i<pSearchPaths->size(); ++i ) {
        const String &path = (*pSearchPaths)[ i ];
        String abspath = path + rFile.getResource();
        Uri file( rFile.getScheme() +"://" + abspath );
        pStream = open( file, mode );
        return pStream;
    }

    return nullptr;
}

const c8 *LocaleFileSystem::getSchema() const {
    return BaseFileSchema;
}

String LocaleFileSystem::getWorkingDirectory() {
    String workingDir;
    static const ui32 Size = 256;
    char buffer[ Size ];

#ifdef OSRE_WINDOWS
    GetCurrentDirectory( Size-1, buffer );
    workingDir = buffer;
#else
    workingDir = getcwd( buffer, Size-1);
#endif

    return workingDir;
}

} // Namespace IO
} // Namespace OSRE
