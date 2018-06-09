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

#include <SDL.h>

namespace OSRE {
namespace Platform {

class SDL2SystemInfo : public AbstractSystemInfo {
public:
    SDL2SystemInfo();
    virtual ~SDL2SystemInfo();
    void getDesktopResolution( Resolution &resolution ) override;
    bool getDiskInfo(const c8 *drive, ui64 &freeSpaceInBytes) override;
};

inline
SDL2SystemInfo::SDL2SystemInfo() {
    // empty
}

inline
SDL2SystemInfo::~SDL2SystemInfo() {
    // empty
}

inline
void SDL2SystemInfo::getDesktopResolution( Resolution &resolution ) {
    SDL_DisplayMode dm;
    if ( SDL_GetDesktopDisplayMode( 0, &dm ) != 0 ) {
        return;
    }
    resolution.m_width = dm.w;
    resolution.m_height = dm.h;
}

inline
bool SDL2SystemInfo::getDiskInfo(const c8 *drive, ui64 &freeSpaceInBytes) {
    // todo
    freeSpaceInBytes = 0;
    return false;
}

} // Namespace Platform
} // Namespace OSRE
