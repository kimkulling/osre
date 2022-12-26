/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Platform/AbstractOSService.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements common os services.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Win32OSService : public AbstractOSService {
public:
    ///	@brief The class constructor, default impl.
    Win32OSService() = default;

    ///	@brief The class destructor, default impl.
    ~Win32OSService() override = default;

    /// @brief Will return the current monitor resolution.
    /// @param width The width.
    /// @param heigt The height.
    void getMonitorResolution(ui32 &width, ui32 &heigt) override;
    void showCursor(bool enabled) override;
    static String getLastErrorAsString();
};

} // Namespace Platform
} // Namespace OSRE
