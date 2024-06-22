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

#include "Platform/AbstractOSService.h"

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
/// @brief This class will implement the OS-services by using SDL2.
//-------------------------------------------------------------------------------------------------
class SDL2OSService final : public AbstractOSService {
public:
    /// @brief The class constructor.
    SDL2OSService();

    /// @brief The class destructor
    ~SDL2OSService() override = default;

    /// @brief Will return the monitor resolution.
    /// @param[out] width  The width of the monitor.
    /// @param[out} height The height of the monitor,
    void getMonitorResolution(ui32 &width, ui32 &height) override;

    /// @brief Will set the cursor visible state.
    /// @param[in] enabled  true for visible, false for not visible.
    void showCursor(bool enabled) override;
};

} // Namespace Platform
} // Namespace OSRE

