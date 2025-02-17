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
#pragma once

#include "Platform/AbstractSystemInfo.h"
#include <cppcore/IO/FileSystem.h>
#include <SDL.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
/// @brief Implaments system info queries.
//-------------------------------------------------------------------------------------------------
class SDL2SystemInfo final : public AbstractSystemInfo {
public:
    /// @brief The class constructor.
    SDL2SystemInfo() = default;

    /// @brief The class destructor.
    ~SDL2SystemInfo() = default;

    /// @brief Returns the resolutions.
    /// @param resolution   The resolution of the desktop.
    void getDesktopResolution( Resolution &resolution ) override;
    
    /// @brief Returns the free space of a given disk,
    /// @param drive             The drive to check.
    /// @param freeSpaceInBytes  The free space in bytes.
    /// @return true if successful, false in case of an error.
    bool getDiskInfo(const c8 *drive, ui64 &freeSpaceInBytes) override;
};

inline void SDL2SystemInfo::getDesktopResolution( Resolution &resolution ) {
    SDL_DisplayMode dm;
    if ( SDL_GetDesktopDisplayMode( 0, &dm ) != 0 ) {
        return;
    }
    resolution.m_width = dm.w;
    resolution.m_height = dm.h;
}

inline bool SDL2SystemInfo::getDiskInfo(const c8 *drive, ui64 &freeSpaceInBytes) {
    freeSpaceInBytes = 0;
    cppcore::FileSystem fs(drive);
    cppcore::FSSpace *space = fs.getFreeDiskSpace();
    if (nullptr == space) {
        return false;
    }

    freeSpaceInBytes = space->free;

    return true;
}

} // Namespace Platform
} // Namespace OSRE
