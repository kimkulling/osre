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
#include "Profiling/FPSCounter.h"
#include "Common/Logger.h"
#include "Platform/AbstractTimer.h"

namespace OSRE::Profiling {

using namespace ::OSRE::Common;

static constexpr c8 Tag[] = "FPSCounter";

FPSCounter::FPSCounter( Platform::AbstractTimer *timer ) :
        m_timerPtr(timer), m_timeDiff(0), m_lastTime(0), m_fps(0), m_lastFPS(0) {
    if (m_timerPtr != nullptr) {
        m_timeDiff = m_timerPtr->getTimeDiff();
    }
}

static constexpr ui32 SecondsInMs = 1000;

ui32 FPSCounter::getFPS() {
    if (m_timerPtr== nullptr) {
        osre_debug(Tag, "No valid timer instance.");
        return 0;
    }

    m_timeDiff = m_timerPtr->getTimeDiff();
    m_lastTime += m_timeDiff.microSeconds;
    m_fps++;
    if (m_lastTime > SecondsInMs) {
        m_lastTime = 0;
        m_lastFPS = m_fps;
        m_fps = 0;
    }

    return m_lastFPS;
}

}
