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
#pragma once

#include <osre/Platform/AbstractSystemInfo.h>
#include <osre/Platform/Windows/MinWindows.h>

namespace OSRE {
namespace Platform {
        
//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the System Information API for Win32-based systems.
//-------------------------------------------------------------------------------------------------
class Win32SystemInfo : public AbstractSystemInfo {
public:
    Win32SystemInfo();
    virtual ~Win32SystemInfo();
    void getDesktopResolution( Resolution &resolution ) override;
    bool getDiskInfo(const c8 *drive, ui64 &freeSpaceInBytes) override;
};

inline
Win32SystemInfo::Win32SystemInfo() {
    // empty
}

inline
Win32SystemInfo::~Win32SystemInfo() {
    // empty
}

inline
void Win32SystemInfo::getDesktopResolution( Resolution &resolution ) {
    RECT actualDesktop;
    if ( TRUE == ::GetWindowRect( GetDesktopWindow(), &actualDesktop ) ) {
        resolution.m_width = actualDesktop.right - actualDesktop.left;
        resolution.m_height = actualDesktop.bottom - actualDesktop.top;
    }
}

inline
bool Win32SystemInfo::getDiskInfo(const c8 *drive, ui64 &freeSpaceInBytes) {
    if (nullptr == drive) {
        freeSpaceInBytes = 0;
        return false;
    }

    PULARGE_INTEGER  freeByteAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
    BOOL result = ::GetDiskFreeSpaceEx(drive, freeByteAvailable, totalNumberOfBytes, totalNumberOfFreeBytes);
    ::memcpy(&freeSpaceInBytes, &freeByteAvailable->QuadPart, sizeof(PULARGE_INTEGER));

    return TRUE == result;
}

} // Namespace Platform
} // Namespace OSRE
