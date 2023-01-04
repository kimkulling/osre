/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
    ~FileStream() override;
    /// true for files with permission.
    bool canRead() const override;
    /// true for files with permission.
    bool canWrite() const override;
    /// true for files with permission.
    bool canSeek() const override;
    /// Opens the file.
    bool open() override;
    /// Close the file.
    bool close() override;
    /// Returns file size.
    size_t getSize() const override;
    /// Reads from file.
    size_t read(void *pBuffer, size_t size) override;
    /// Writes into file.
    size_t write(const void *pBuffer, size_t size) override;
    /// Reads a single integer value.
    size_t readI32(i32 &value) override;
    /// Writes a single integer value.
    size_t writeI32(i32 value) override;
    /// Reads a single unsigned integer value.
    size_t readUI32(ui32 &value) override;
    /// Writes a single unsigned integer value.
    size_t writeUI32(ui32 value) override;
    /// Moves to given position.
    Position seek(Offset offset, Origin origin) override;
    /// Position in the file.
    Position tell() override;
    /// Returns true, when the stream access is open.
    bool isOpen() const override;

private:
    FILE *m_file;
};

} // Namespace IO
} // Namespace OSRE
