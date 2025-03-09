/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "FileStream.h"

#include "Debugging/osre_debugging.h"
#include <sys/stat.h>
#include <sys/types.h>

namespace OSRE {
namespace IO {

FileStream::FileStream(const Uri &uri, AccessMode reqAccess) : Stream(uri, reqAccess) {
    // empty
}

FileStream::~FileStream() {
    if (isOpen()) {
        FileStream::close();
    }
}

bool FileStream::canRead() const {
    return true;
}

bool FileStream::canWrite() const {
    return true;
}

bool FileStream::canSeek() const {
    return true;
}

bool FileStream::open() {
    if (isOpen()) {
        return false;
    }

    osre_assert(nullptr == mFile);
    const String &abspath = mUri.getAbsPath();
    String modestr;
    AccessMode mode = getAccessMode();
    if (AccessMode::ReadAccessBinary == mode) {
        modestr = "rb";
    } else if (AccessMode::WriteAccessBinary == mode) {
        modestr = "wb";
    } else if (AccessMode::ReadAccess == mode) {
        modestr = "r";
    } else if (AccessMode::WriteAccess == mode) {
        modestr = "w";
    } else if (AccessMode::ReadWriteAccess == mode) {
        modestr = "r+";
    } else if (AccessMode::AppendAccess == mode) {
        modestr = "ra";
    }
    osre_assert(!modestr.empty());

#if defined(OSRE_WINDOWS) && !defined(__MINGW32__) && !defined(__MINGW64__)
    errno_t err;
    err = ::fopen_s(&mFile, abspath.c_str(), modestr.c_str());
    osre_assert(0 == err);
#else
    mfile = ::fopen(abspath.c_str(), modestr.c_str());
#endif

    return (nullptr != mFile);
}

bool FileStream::close() {
    if (!isOpen()) {
        return false;
    }

    ::fclose(mFile);
    mFile = nullptr;
    return true;
}

size_t FileStream::getSize() const {
    osre_assert(!mUri.getAbsPath().empty());

    const String &abspath(mUri.getAbsPath());
#ifdef OSRE_WINDOWS
    // Get the windows specific file-size
    struct __stat64 fileStat;
    int err = _stat64(abspath.c_str(), &fileStat);
    if (0 != err) {
        return 0;
    }

    return (static_cast<ui32>(fileStat.st_size));
#else
    // For unix
    struct stat fileStat;
    int err = stat(abspath.c_str(), &fileStat);
    if (0 != err) {
        return 0;
    }
    return static_cast<ui32>(fileStat.st_size);
#endif
}

size_t FileStream::read(void *buffer, size_t size) {
    if (!buffer || 0 == size) {
        return 0;
    }

    if (!isOpen()) {
        return 0;
    }

    return ::fread(buffer, sizeof(uc8), size, mFile);
}

size_t FileStream::write(const void *buffer, size_t size) {
    osre_assert(nullptr != buffer);
    if (!isOpen() || 0 == size || !buffer) {
        return 0;
    }

    return ::fwrite(buffer, sizeof(c8), size, mFile);
}

size_t FileStream::readI32(i32 &value) {
    if (isOpen()) {
        return ::fread(&value, sizeof(i32), 1, mFile);
    }

    return 0;
}

size_t FileStream::writeI32(i32 value) {
    if (isOpen()) {
        return ::fwrite(&value, sizeof(i32), 1, mFile);
    }

    return 0;
}

size_t FileStream::readUI32(ui32 &value) {
    if (isOpen()) {
        return ::fread(&value, sizeof(ui32), 1, mFile);
    }

    return 0;
}

size_t FileStream::writeUI32(ui32 value) {
    if (isOpen()) {
        return ::fwrite(&value, sizeof(ui32), 1, mFile);
    }

    return 0;
}

FileStream::Position FileStream::seek(Offset offset, Origin origin) {
    i32 originValue(0);
    if (origin == Stream::Origin::Current) {
        originValue = SEEK_CUR;
    } else {
        originValue = SEEK_SET;
    }
    if (isOpen()) {
        return ::fseek(mFile, offset, originValue);
    }

    return 0;
}

FileStream::Position FileStream::tell() {
    if (isOpen()) {
        return (::ftell(mFile));
    }

    return 0;
}

bool FileStream::isOpen() const {
    return (mFile != nullptr);
}

} // Namespace IO
} // Namespace OSRE
