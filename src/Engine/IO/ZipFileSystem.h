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
#pragma once

#include <osre/IO/AbstractFileSystem.h>
#include "contrib/unzip/unzip.h"
#include <map>
#include <vector>

namespace OSRE {
namespace IO {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::IO::ZipFileSystem
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
    virtual Stream *find(const Uri &file, Stream::AccessMode mode, StringArray *pSearchPaths);
    ///	Returns the zip schema description.
    virtual const c8 *getSchema() const;
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
    ui32 m_numberOfRefs;
    bool m_Dirty;
};

//-------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
