/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "ZipFileSystem.h"
#include "ZipFileStream.h"
#include <osre/IO/IOSystemInfo.h>
#include <osre/Common/Logger.h>
#include <osre/Debugging/osre_debugging.h>

#include <algorithm>
#include <cassert>

namespace OSRE {
namespace IO {

using namespace ::CPPCore;

static const c8 *ZipSchema = "zip";
static const c8 *Tag = "ZipFileSystem";

ZipFileSystem::ZipFileSystem( const Uri &archive ) 
: m_FileMap()
, m_FileList()
, m_ArchiveName( archive.getAbsPath() )
, m_ZipFileHandle( nullptr )
, m_Dirty( true ) {
    if ( openArchive() ) {
        mapArchive();
    }
}

ZipFileSystem::~ZipFileSystem() {
    closeAllFiles();
    if (nullptr != m_ZipFileHandle ) {
        unzClose( m_ZipFileHandle );
        m_ZipFileHandle = nullptr;
    }
}

Stream *ZipFileSystem::open( const Uri &file, Stream::AccessMode mode ) {
    if ( !isOpened() || mode != Stream::AccessMode::ReadAccess ) {
        return nullptr;
    }
    
    // ensure mapped archive
    if ( m_Dirty ) {
        mapArchive();
    }

    // look if file is already open
    StreamMap::const_iterator it( m_FileMap.find( file.getResource() ) );
    if( m_FileMap.end() != it ) {
        return it->second;
    }

    Stream *pZipStream( nullptr );
    const String tmpName = file.getResource();
    Uri uri( tmpName );
    if ( fileExist( Uri( uri ) ) ) {
        pZipStream = new ZipFileStream( uri, m_ZipFileHandle );
        m_FileMap[ tmpName ] = pZipStream;
    }

    return pZipStream;
}

void ZipFileSystem::close( Stream **ppZipFileStream ) {
    OSRE_ASSERT( nullptr != *ppZipFileStream );
    
    const String &name = ( *ppZipFileStream )->getUri().getResource();
    StreamMap::iterator it = m_FileMap.find( name );
    if ( m_FileMap.end() == it ) {
        return;
    }

    Stream *zipFilestream = (*it).second;
    delete zipFilestream;
    m_FileMap.erase( it );
    (*ppZipFileStream) = nullptr;
}

bool ZipFileSystem::fileExist( const Uri &file ) {
    if ( file.isEmpty() ) {
        osre_debug( Tag, "Filename is empty." );
        return false;
    }
    
    std::vector<String>::iterator it = std::find( m_FileList.begin(), m_FileList.end(), file.getResource() );
    if ( m_FileList.end() == it ) {
        return false;
    }
    
    return true;
}

Stream *ZipFileSystem::find( const Uri &file, Stream::AccessMode mode, CPPCore::TArray<String> *searchPaths ) {
    if( !fileExist( file ) ) {
        return nullptr;
    }

    Stream *stream(nullptr);
    if (nullptr != searchPaths) {
        for (ui32 i = 0; i < searchPaths->size(); ++i) {
            const String &folder = (*searchPaths)[i];
            String abspath = folder + file.getResource();
            Uri currentFile(file.getScheme() + "://" + abspath);

            stream = open(currentFile, mode);
            if ( nullptr != stream) {
                return stream;
            }
        }
    }
    return this->open( file, mode );
}

const c8 *ZipFileSystem::getSchema() const {
    return ZipSchema;
}

String ZipFileSystem::getWorkingDirectory() {

#ifdef OSRE_WINDOWS
    return String( ".\\" );
#else
    return String( "./" );
#endif

}

void ZipFileSystem::getFileList( std::vector<String> &rFileList ) {
    if (nullptr == m_ZipFileHandle ) {
        rFileList.resize( 0 );
    } else {
        rFileList = m_FileList;
    }
}

bool ZipFileSystem::openArchive() {
    OSRE_ASSERT(nullptr == m_ZipFileHandle );

    String current_dir = IOSystemInfo::getCurrentDirectory();
    if (m_ArchiveName.empty()) {
        return false;
    }

    m_ZipFileHandle = unzOpen( m_ArchiveName.c_str() );
    if (nullptr != m_ZipFileHandle) {
        return true;
    }

    return false;
}

bool ZipFileSystem::isOpened() const {
    return (nullptr != m_ZipFileHandle );
}

void ZipFileSystem::mapArchive() {
    OSRE_ASSERT(nullptr != m_ZipFileHandle );

    if (!m_FileList.empty()) {
        m_FileList.resize(0);
    }

    //	at first ensure file is already open
    if ( UNZ_OK == unzGoToFirstFile( m_ZipFileHandle ) )  {
        c8 filename[ FileNameSize ];
        unzGetCurrentFileInfo( m_ZipFileHandle, NULL, filename, FileNameSize, NULL, 0, NULL, 0 );
        m_FileList.push_back( filename );
        unzCloseCurrentFile( m_ZipFileHandle );
            
        // loop over all files
        while ( unzGoToNextFile( m_ZipFileHandle ) != UNZ_END_OF_LIST_OF_FILE )  {
            unzGetCurrentFileInfo( m_ZipFileHandle, NULL, filename, FileNameSize, NULL, 0, NULL, 0);
            m_FileList.push_back( filename );
            unzCloseCurrentFile( m_ZipFileHandle );
        }
    }
    
    std::sort( m_FileList.begin(), m_FileList.end() );
    m_Dirty = false;
}

void ZipFileSystem::closeAllFiles() {
    /// TODO: implement me
}

} // Namespace IO
} // Namespace OSRE
