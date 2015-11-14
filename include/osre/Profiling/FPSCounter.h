/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>

namespace OSRE {

// Forward declarations
namespace Platform {
    class AbstractTimer;
}

namespace Profiling {
        
//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Profiling::FPSCounter
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
class FPSCounter {
public:
    ///	@brief  The class constructor.
    /// @param  timer   [in] The timer instance.
    FPSCounter( Platform::AbstractTimer  *timer );
    
    ///	@brief  The class destructor.
    ~FPSCounter();

    ///	@brief  Returns the counted frames per seconds.
    /// @return The counted frames per second.
    ui32 getFPS();

private:
    Platform::AbstractTimer  *m_timer;
    d32 m_timeDiff, m_lastTime;
    ui32 m_fps;
};

} // Namespace Profiling
} // Namespace OSRE
