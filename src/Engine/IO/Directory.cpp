/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/IO/Directory.h>

#include <sys/types.h>
#include <sys/stat.h>

#ifdef OSRE_WINDOWS
#   include <osre/Platform/Windows/MinWindows.h>
#endif // OSRE_WINDOWS

namespace OSRE {
namespace IO {
        
bool Directory::exists(const String &dir) {
    struct stat info;
    if ( -1 == ::stat( dir.c_str(), &info ) ) {
        return false;
    }

    if (info.st_mode & S_IFDIR) {
        return true;
    }

    return false;
}

void Directory::getDirectoryAndFile(const String &pathAndFilename, String &path, String &filename) {
    path.clear();
    filename.clear();
    if (pathAndFilename.empty()) {
        return;
    }

    String::size_type pos = pathAndFilename.rfind("/");
    if (String::npos == pos) {
        filename = pathAndFilename;
        return;
    }

    path = pathAndFilename.substr(0, pos + 1);
    filename = pathAndFilename.substr(pos + 1, pathAndFilename.size() - pos - 1);
}

String Directory::getDirSeparator() {
#ifdef OSRE_WINDOWS
    static String sep = "\\";
#else
    static String sep = "/";
#endif
    return sep;
}

bool Directory::setCurrentDirectory(const String& absPath) {
    if (absPath.empty()) {
        return false;
    }
#ifdef OSRE_WINDOWS
    return TRUE == ::SetCurrentDirectory(absPath.c_str());
#else
    return 0 == chdir(absPath.c_str());
#endif
}

String Directory::getCurrentDirectory() {
    const size_t BufferLen = 256;
    c8 buffer[BufferLen];
#ifdef OSRE_WINDOWS
    DWORD len = ::GetCurrentDirectory(BufferLen, buffer);
    if (len>0) {
        String path(buffer);
        return path;
    }
    return "";
#else
    buffer = getcwd(buffer, BufferSize);
    String path(buffer);
    return path;
#endif
}

bool Directory::createDirectory(const c8* name) {
    if (nullptr == name) {
        return false;
    }

#ifdef OSRE_WINDOWS
    return TRUE == ::CreateDirectory(name, nullptr);
#else
    return 0 == mkdir(name);
#endif
}

} // Namespace IO
} // Namespace OSRE
