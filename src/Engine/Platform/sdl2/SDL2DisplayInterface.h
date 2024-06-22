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
#pragma once

#include "Platform/AbstractDisplayInterface.h"

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the Display API with SDL2.
//-------------------------------------------------------------------------------------------------
class SDL2DisplayInterface final : public AbstractDisplayInterface {
public:
    /// @brief The default class constructor.
    SDL2DisplayInterface() = default;

    /// @brief The class destructor.
    ~SDL2DisplayInterface() override = default;
    
    /// @brief Returns the number of detected displays.
    /// @return The number of detected displays.
    i32 getNumDisplays() override;

    /// @brief Will return the resolution of the requested displays.
    /// @param displayIndex     The display index.
    /// @param width            The width.
    /// @param height           The height.
    /// @return true if display exists and is accessable.
    bool getDisplayResolution(ui32 displayIndex, ui32 &width, ui32 &height) override;
    
    /// @brief Will return the DPI info of the requested display.
    /// @param displayIndex     The display index.
    /// @param ddpiinfo         The DPI info. 
   /// @return true if display exists and is accessable.
     bool getDisplayDPI(ui32 displayIndex, DisplayDPIInfo *ddpiinfo) override;
};

} // namespace Platform
} // namespace OSRE
