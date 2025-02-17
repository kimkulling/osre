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

#include "Platform/AbstractDisplayInterface.h"

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace Platform {

struct MonitorInfo;

//-------------------------------------------------------------------------------------------------
/// @ingroup	Engine
///
/// @brief This class implements the windows-specific display interface.
//-------------------------------------------------------------------------------------------------
class Win32DisplayInterface : public AbstractDisplayInterface {
public:
    /// @brief The default class constructor.
    Win32DisplayInterface() = default;
    
    /// @brief The class destructor.
    ~Win32DisplayInterface() override;
    
    /// @brief  Will return the number of detected display.
    /// @return The number of detected displays.
    i32 getNumDisplays() override;
    
    /// @brief Will return the resolution for the requested display.
    /// @param[in]  displayIndex   The requested display index.
    /// @param[out] width          The width.
    /// @param[out] height         The height.
    /// @return true, if successful, false if not.
    bool getDisplayResolution(ui32 displayIndex, ui32 &width, ui32 &height) override;
    
    /// @brief Will return the dpi info for the requested display dpi info.
    /// @param[out] ddpiinfo       The dpi info.
    /// @return true, if successful, false if not.
    bool getDisplayDPI(ui32 displayIndex, DisplayDPIInfo *ddpiinfo) override;

private:
    ::cppcore::TArray<MonitorInfo*> mMonitorInfo;
};

} // namespace Platform
} // namespace OSRE
