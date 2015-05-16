#pragma once

#include <osre/IO/Stream.h>

namespace OSRE {
namespace IO {

//--------------------------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::IO::FileStream
///	@ingroup	Infrastructure
///
///	@brief	This class implements the basic file operations like reading and writing data,
///	positioning the file pointer in an already opened file. You can also get some basic infos about
///	a file like its size. Renaming, checking for existence and removing it offers static methods.
//--------------------------------------------------------------------------------------------------------------------
class FileStream : public Stream {
public:
    /// The default class constructor.
    FileStream();
    /// The class constructor with URI and access mode.
    FileStream( const Uri &uri, AccessMode requestedAccess );
    /// The class destructor.
    ~FileStream();
    /// true for files with permission.
    bool canRead() const;
    /// true for files with permission.
    bool canWrite() const;
    /// true for files with permission.
    bool canSeek() const;
    /// Opens the file.
    bool open();
    /// Close the file.
    bool close();
    /// Returns file size.
    ui32 getSize();
    /// Reads from file.
    ui32 read( void *pBuffer, ui32 size );
    /// Writes into file.
    ui32 write( const void *pBuffer, ui32 size );
    /// Moves to given position.
    Position seek( Offset offset, Origin origin );
    /// Position in the file.
    Position tell();
    /// Returns true, when the stream access is open.
    bool isOpen() const;

private:
    FILE *m_pFile;
};

//--------------------------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
