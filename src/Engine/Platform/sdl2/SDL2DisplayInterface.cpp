/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "SDL2DisplayInterface.h"
#include <osre/Common/Logger.h>
#include "SDL.h"

namespace OSRE {
namespace Platform {

static constexpr c8 Tag[] = "SDL2DisplayInterface";

static void logError() {
    osre_debug(Tag, "Error occurred : " + String(SDL_GetError()) + ".");
}

i32 SDL2DisplayInterface::getNumDisplays() {
    i32 numDisplays = SDL_GetNumVideoDisplays();
    return numDisplays;
}

bool SDL2DisplayInterface::getDisplayResolution( ui32 displayIndex, ui32 &width, ui32 &height ) {
    SDL_Rect rect;
    width = height = 0;
    if (0 != SDL_GetDisplayBounds(static_cast<i32>(displayIndex), &rect)) {
        logError();
        return false;
    }
    
    width = rect.w;
    height = rect.h;

    return true;
}

bool SDL2DisplayInterface::getDisplayDPI( ui32 displayIndex, DisplayDPIInfo *ddpiinfo ) {
    if (nullptr == ddpiinfo) {
        return false;
    }

    f32 ddpi, hdpi, vdpi;
    if (0 != SDL_GetDisplayDPI(static_cast<i32>(displayIndex), &ddpi, &hdpi, &vdpi)) {
        logError();
        return false;
    }

    ddpiinfo->ddpi = ddpi;
    ddpiinfo->hdpi = hdpi;
    ddpiinfo->vdpi = vdpi;

    return true;
}


} // namespace Platform
} // namespace OSRE
