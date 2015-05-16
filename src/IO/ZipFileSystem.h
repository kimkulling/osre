#pragma once

#include <osre/IO/AbstractFileSystem.h>

#include "unzip.h"

#include <map>
#include <vector>

namespace OSRE {
namespace IO {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::IO::ZipFileSystem
///	@ingroup	Infrastructure
///
///	@brief	Class which implements access for Zip-archives. 
///    
/// Currently only the read access is supported.
//-------------------------------------------------------------------------------------------------
class ZipFileSystem : public AbstractFileSystem {
public:
    ///	Upper length for filenames.
    static const ui32 FileNameSize = 256;

public:
    ///	The class constructor with archive name.
    ZipFileSystem( const Uri &rArchive );
    ///	The class destructor.
    virtual ~ZipFileSystem();
    ///	Opens a file instance from a zip archive.
    virtual Stream *open( const Uri &filename, Stream::AccessMode mode );
    ///	Close an opened file from a zip archive.
    virtual void close( Stream **pFile );
    ///	Returns true, if file exists in this zip archive.
    virtual bool fileExist( const Uri &filename );
    /// Search for a given file.
    virtual Stream *find( const Uri &file, Stream::AccessMode mode, CPPCore::TArray<String> *pSearchPaths );
    ///	Returns the zip schema description.
    virtual const String &getSchema() const;
    ///	Returns the working directory.
    virtual String getWorkingDirectory();

public:
    ///	Returns the file list in the archive.
    void getFileList( std::vector<String> &fileList );

private:
    bool openArchive();	
    bool isOpened() const;
    void mapArchive();
    void closeAllFiles();

private:
    StreamMap m_FileMap;
    std::vector<String> m_FileList;
    String m_ArchiveName;
    unzFile m_ZipFileHandle;
    ui32 m_NumberOfRefs;
    bool m_Dirty;
};

//-------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
