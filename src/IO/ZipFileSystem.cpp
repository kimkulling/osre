#include "ZipFileSystem.h"
#include "ZipFileStream.h"
#include <osre/IO/IOSystemInfo.h>
#include <osre/Common/Logger.h>

#include "unzip.h"
#include <algorithm>
#include <cassert>

namespace OSRE {
namespace IO {

using namespace ::CPPCore;

static const String ZipSchema = "zip";

//-------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------
ZipFileSystem::~ZipFileSystem() {
    closeAllFiles();
    if ( NULL != m_ZipFileHandle ) {
        unzClose( m_ZipFileHandle );
        m_ZipFileHandle = nullptr;
    }
}

//-------------------------------------------------------------------------------------------------
Stream *ZipFileSystem::open( const Uri &file, Stream::AccessMode mode ) {
    if ( !isOpened() || mode != Stream::ReadAccess ) {
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
    if ( fileExist( tmpName ) ) {
        pZipStream = new ZipFileStream( tmpName, m_ZipFileHandle );
        m_FileMap[ tmpName ] = pZipStream;
    }

    return pZipStream;
}

//-------------------------------------------------------------------------------------------------
void ZipFileSystem::close( Stream **ppZipFileStream ) {
    assert( NULL != *ppZipFileStream );
    
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

//-------------------------------------------------------------------------------------------------
bool ZipFileSystem::fileExist( const Uri &file ) {
    if ( file.isEmpty() ) {
        osre_debug( "Filename is empty." );
        return false;
    }
    
    std::vector<String>::iterator it = std::find( m_FileList.begin(), m_FileList.end(), file.getResource() );
    if ( m_FileList.end() == it ) {
        return false;
    }
    
    return true;
}

//-------------------------------------------------------------------------------------------------
Stream *ZipFileSystem::find( const Uri &file, Stream::AccessMode mode, CPPCore::TArray<String> *pSearchPaths ) {
    if( !fileExist( file ) ) {
        return nullptr;
    }

    return this->open( file, Stream::ReadAccess );
}

//-------------------------------------------------------------------------------------------------
const String &ZipFileSystem::getSchema() const {
    return ZipSchema;
}

//-------------------------------------------------------------------------------------------------
String ZipFileSystem::getWorkingDirectory() {

#ifdef _WIN32
    return String( ".\\" );
#else
    return String( "./" );
#endif

}

//-------------------------------------------------------------------------------------------------
void ZipFileSystem::getFileList( std::vector<String> &rFileList ) {
    if ( NULL == m_ZipFileHandle ) {
        rFileList.resize( 0 );
    } else {
        rFileList = m_FileList;
    }
}

//-------------------------------------------------------------------------------------------------
bool ZipFileSystem::openArchive() {
    assert( NULL == m_ZipFileHandle );

    String current_dir = IOSystemInfo::getCurrentDirectory();
    if ( m_ArchiveName.empty() )	
        return false;

    m_ZipFileHandle = unzOpen( m_ArchiveName.c_str() );
    if ( NULL != m_ZipFileHandle )
        return true;

    return false;
}
//-------------------------------------------------------------------------------------------------
bool ZipFileSystem::isOpened() const {
    return ( NULL != m_ZipFileHandle );
}

//-------------------------------------------------------------------------------------------------
void ZipFileSystem::mapArchive() {
    assert( NULL != m_ZipFileHandle );

    if ( !m_FileList.empty() )
        m_FileList.resize( 0 );

    //	at first ensure file is already open
    if ( UNZ_OK == unzGoToFirstFile( m_ZipFileHandle ) )  {
        c8 filename[ FileNameSize ];
        unzGetCurrentFileInfo( m_ZipFileHandle, NULL, filename, FileNameSize, NULL, 0, NULL, 0 );
        m_FileList.push_back( filename );
        unzCloseCurrentFile( m_ZipFileHandle );
            
        // loop over all files
        while ( unzGoToNextFile( m_ZipFileHandle ) != UNZ_END_OF_LIST_OF_FILE )  {
            c8 filename[ FileNameSize ];
            unzGetCurrentFileInfo( m_ZipFileHandle, NULL, filename, FileNameSize, NULL, 0, NULL, 0);
            m_FileList.push_back( filename );
            unzCloseCurrentFile( m_ZipFileHandle );
        }
    }
    
    std::sort( m_FileList.begin(), m_FileList.end() );
    m_Dirty = false;
}

//-------------------------------------------------------------------------------------------------
void ZipFileSystem::closeAllFiles() {
    /// TODO: implement me
}

//-------------------------------------------------------------------------------------------------

} // Namespace VFS
} // Namespace OSRE
