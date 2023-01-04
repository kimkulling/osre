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

#include <osre/Common/Logger.h>
#include <osre/Platform/Windows/MinWindows.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This class implements a log-stream, which will be visible in the Windows-Debugger
///	output window.
//-------------------------------------------------------------------------------------------------
class Win32DbgLogStream : public Common::AbstractLogStream {
public:
    ///	The class constructor.
    Win32DbgLogStream();
    ///	The class destructor, non virtual.
    ~Win32DbgLogStream();
    ///	Writes the message into the debug output buffer.
    void write(const String &msg);
};

inline Win32DbgLogStream::Win32DbgLogStream() {
    // empty
}

inline Win32DbgLogStream::~Win32DbgLogStream() {
    // empty
}

inline void Win32DbgLogStream::write(const String &msg) {
    if (!msg.empty()) {
        ::OutputDebugString(msg.c_str());
    }
}

} // Namespace Platform
} // Namespace OSRE
