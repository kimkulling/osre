/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/IO/IOSystemInfo.h>

#ifdef OSRE_WINDOWS

#  include <direct.h>
#  include <stdio.h>
#  include <stdlib.h>
#else
#  include <unistd.h>
#endif


namespace OSRE {
namespace IO {

//-------------------------------------------------------------------------------------------------
IOSystemInfo::IOSystemInfo() {
    // empty
}

//-------------------------------------------------------------------------------------------------
IOSystemInfo::~IOSystemInfo() {
    // empty
}

//-------------------------------------------------------------------------------------------------
String IOSystemInfo::getDirSeparator() {
#ifdef OSRE_WINDOWS
    static String sep = "\\";
#else
    static String sep = "/";
#endif
    return sep;
}

//-------------------------------------------------------------------------------------------------
String IOSystemInfo::getCurrentDirToken() {
#ifdef OSRE_WINDOWS
    static String token = ".\\";
#else
    static String token = "./";
#endif

    return token;
}

//-------------------------------------------------------------------------------------------------
String IOSystemInfo::getCurrentDirectory() {
    static const ui32 buffersize = 256;
    c8 buffer[buffersize];
    c8 *retPtr( nullptr );
#ifndef OSRE_WINDOWS
    // POSIX call
    retPtr = ::getcwd( buffer, buffersize );
#else
    // WIN32-API call
    retPtr = ::_getcwd( buffer, buffersize );
#endif
    if( nullptr == retPtr ) {
        return nullptr;
    }

    String currentDir( buffer );
    currentDir += getDirSeparator();

    return currentDir;
}

//-------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
