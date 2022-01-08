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

#include <osre/Platform/AbstractWindow.h>

namespace OSRE {
namespace Platform {
        
//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This abstract base class represents the interface for all system-specific information 
///         calls.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractSystemInfo {
public:
    /// @brief  The class destructor, virtual.
    virtual ~AbstractSystemInfo();

    /// @brief  Will return the current desktop resolution.
    /// @param  resolution  [out] The resolution info.
    virtual void getDesktopResolution( Resolution &resolution ) = 0;

    /// @brief  Will return disk information.
    /// @param  drive      [in] The drive descriptor.
    /// @param  freeSpace  [out] The free space.
    /// @return true, if this feature is supported.
    virtual bool getDiskInfo( const c8 *drive, ui64 &freeSpaceInBytes) = 0;
};

inline
AbstractSystemInfo::~AbstractSystemInfo() {
    // empty
}

} // Namespace Platform
} // Namespace OSRE
