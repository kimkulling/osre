/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/IO/Stream.h>

namespace OSRE {
namespace IO {

//--------------------------------------------------------------------------------------------------------------------
///	@ingroup	Infrastructure
///
///	@brief	This class implements the basic file operations like reading and writing data,
///	positioning the file pointer in an already opened file. You can also get some basic infos about
///	a file like its size. Renaming, checking for existence and removing it offers static methods.
//--------------------------------------------------------------------------------------------------------------------
class FileStream : public Stream {
public:
    /// The default class constructor.
    FileStream() noexcept;
    /// The class constructor with URI and access mode.
    FileStream(const Uri &uri, AccessMode requestedAccess);
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
    ui32 getSize() const;
    /// Reads from file.
    ui32 read(void *pBuffer, ui32 size);
    /// Writes into file.
    ui32 write(const void *pBuffer, ui32 size);
    /// Reads a single integer value.
    ui32 readI32(i32 &value);
    /// Writes a single integer value.
    ui32 writeI32(i32 value);
    /// Reads a single unsigned integer value.
    ui32 readUI32(ui32 &value);
    /// Writes a single unsigned integer value.
    ui32 writeUI32(ui32 value);
    /// Moves to given position.
    Position seek(Offset offset, Origin origin);
    /// Position in the file.
    Position tell();
    /// Returns true, when the stream access is open.
    bool isOpen() const;

private:
    FILE *m_file;
};

} // Namespace IO
} // Namespace OSRE
