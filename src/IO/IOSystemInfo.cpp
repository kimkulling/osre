/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions
and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
and the following disclaimer in the documentation and/or other materials provided with the
distribution.

* Neither the name of the ZFXCE2 team, nor the names of its contributors may be used to endorse or
promote products derived from this software without specific prior written permission of the
ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
#include <osre/IO/IOSystemInfo.h>

#ifdef _WIN32
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
#ifdef CE_WINDOWS
    static String sep = "\\";
#else
    static String sep = "/";
#endif
    return sep;
}

//-------------------------------------------------------------------------------------------------
String IOSystemInfo::getCurrentDirToken() {
#ifdef CE_WINDOWS
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
#ifndef CE_WINDOWS
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
