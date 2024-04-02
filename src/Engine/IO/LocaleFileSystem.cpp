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
#include "LocaleFileSystem.h"
#include "FileStream.h"
#include <osre/IO/File.h>
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
using namespace ::cppcore;

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

    return File::exists(filename.getAbsPath());
}

Stream *LocaleFileSystem::find(const Uri &file, Stream::AccessMode mode, StringArray *searchPaths) {
    if ( !searchPaths ) {
        return nullptr;
    }

    Stream *stream( nullptr );
    for ( ui32 i=0; i<searchPaths->size(); ++i ) {
        const String &path = (*searchPaths)[ i ];
        String abspath = path + file.getResource();
        Uri currentFile( file.getScheme() +"://" + abspath );
        stream = open(currentFile, mode );
        if (nullptr != stream) {
            return stream;
        }
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
