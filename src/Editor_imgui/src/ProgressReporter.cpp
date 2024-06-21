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
#include "ProgressReporter.h"

#include "Platform/AbstractWindow.h"
#include "Platform/win32/Win32Window.h"
#include "Gui/UIElements.h"
#include "Platform/Windows/MinWindows.h"
#include "Platform/AbstractOSService.h"
#include "Platform/PlatformInterface.h"

namespace OSRE {
namespace Editor {

using namespace ::OSRE::Platform;

ProgressReporter::ProgressReporter(AbstractWindow *window) :
        mWindow(window),
        mProgress(0),
        mProgressBar(nullptr) {
    // empty
}

ProgressReporter::~ProgressReporter() {
    if (nullptr != mProgressBar) {
        UIElements::deleteProgressBar(mProgressBar);
    }
}

void ProgressReporter::start() {
    if (nullptr != mWindow) {
        mWindow->setWindowsMouseCursor(DefaultMouseCursorType::WaitCursor);
    }
    
    if (nullptr != mProgressBar) {
        return;
    }

    ui32 width = 0u, heihgt = 0u;
    PlatformInterface::getInstance()->getOSServices()->getMonitorResolution(width, heihgt);
    ui32 x = width / 2u - 50u;
    ui32 y = heihgt / 2u - 10u;
    Rect2ui r(x, y, 300u, 20u);
    Platform::Win32Window *w = (Platform::Win32Window*) mWindow;
    if (nullptr == w) {
        return;
    }

    mProgressBar = UIElements::createProgressBar(1, w->getHWnd(), r);
}

void ProgressReporter::stop() {
    if (nullptr != mWindow) {
        mWindow->setWindowsMouseCursor(Platform::DefaultMouseCursorType::ComonCursor);
        UIElements::deleteProgressBar(mProgressBar);
        mProgressBar = nullptr;
    }
}

void ProgressReporter::update(i32 percent) {
    mProgress = percent;
    UIElements::updateProgressBar(mProgressBar, percent);
}

void ProgressReporter::reset() {
    mProgress = 0;
}

} // namespace Editor
} // namespace OSRE 
