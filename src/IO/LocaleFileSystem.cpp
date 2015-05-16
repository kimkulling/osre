#include "LocaleFileSystem.h"
#include "FileStream.h"

#include <osre/Common/Logger.h>
#include <cassert>

#ifdef _WIN32
#    include <windows.h>
#else
#   include <stdio.h>
#   include <unistd.h>
#   include <string.h>
#endif

namespace OSRE {
namespace IO {

using namespace ::OSRE::Common;
using namespace ::CPPCore;

static const String BaseFileSchema = "file";

//-------------------------------------------------------------------------------------------------
LocaleFileSystem::LocaleFileSystem() 
: m_FileMap() {
    // empty
}

//-------------------------------------------------------------------------------------------------
LocaleFileSystem::~LocaleFileSystem() {
    for ( StreamMap::iterator it = m_FileMap.begin(); it != m_FileMap.end(); ++it )	{
        if ( it->second ) {
            delete it->second;
        }
    }
    m_FileMap.clear();
}

//-------------------------------------------------------------------------------------------------
Stream *LocaleFileSystem::open( const Uri &file, Stream::AccessMode mode ) {
    if ( file.isEmpty() ) {
        osre_debug( "Uri is empty." );
        return nullptr;
    }

    Stream *pFileStream( nullptr );
    String::size_type pos = file.getResource().rfind( "xml" );
    if ( String::npos == pos ) {
        pFileStream = new FileStream( file, mode );
    }

    if ( pFileStream->open() ) {
        m_FileMap[ file.getResource() ] = pFileStream;
    } else {
        delete pFileStream;
        pFileStream = nullptr;
    }

    return pFileStream;
}

//-------------------------------------------------------------------------------------------------
void LocaleFileSystem::close( Stream **pFile ) {
    if ( ( *pFile )->isOpen() ) {
        ( *pFile )->close();
    }

    const Uri &rFile = (*pFile)->getUri();
    StreamMap::iterator it = m_FileMap.find( rFile.getResource() );
    m_FileMap.erase( it );
    (*pFile) = NULL;
}

//-------------------------------------------------------------------------------------------------
bool LocaleFileSystem::fileExist( const Uri &filename ) {
    if( filename.isEmpty() ) {
        return false;
    }

    bool exists( false );
    FILE *pFileStream( nullptr );
#ifdef CE_WINDOWS
    errno_t err = ::fopen_s( &pFileStream, filename.getAbsPath().c_str(), "r" );
    if ( 0 != err ) {
        ce_debug( "Error while opening the file." );
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

//-------------------------------------------------------------------------------------------------
Stream *LocaleFileSystem::find( const Uri &rFile, Stream::AccessMode mode, 
                                TArray<String> *pSearchPaths ) {
    if ( !pSearchPaths ) {
        return nullptr;
    }

    Stream *pStream( nullptr );
    for ( ui32 i=0; i<pSearchPaths->size(); ++i ) {
        const String &path = (*pSearchPaths)[ i ];
        String abspath = path + rFile.getResource();
        Uri file( rFile.getScheme() +"://" + abspath );
        pStream = open( file, mode );
        if ( pStream )
        {
            return pStream;
        }
    }

    return nullptr;
}

//-------------------------------------------------------------------------------------------------
const String &LocaleFileSystem::getSchema() const {
    return BaseFileSchema;
}

//-------------------------------------------------------------------------------------------------
String LocaleFileSystem::getWorkingDirectory() {
    String workingDir;
    static const ui32 Size = 256;
    char buffer[ Size ];

#ifdef _WIN32
    GetCurrentDirectory( Size-1, buffer );
    workingDir = buffer;
#else
    getcwd( buffer, Size-1);
#endif

    return workingDir;
}

//-------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
