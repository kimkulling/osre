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
#include "OsreEdApp.h"
#include "Gui/UIElementsWin32.h"

#include <osre/Platform/AbstractOSService.h>
#include <osre/Platform/PlatformInterface.h>

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::Editor;
using namespace ::OSRE::Platform;

int main(int argc, char *argv[]) {
    OsreEdApp app(argc, argv);

    if (!UIElementsWin32::init()) {
        return -1;
    }

    const ui32 Margin = 10;
    ui32 width = 0, height = 0;
    UIElementsWin32::getMonitorResolution(width, height);
    if (!app.initWindow(Margin, Margin, width - Margin, height - Margin, "OSRE-Ed", false, RenderBackendType::OpenGLRenderBackend)) {
        return 1;
    }

    while (app.handleEvents()) {
        app.update();
        app.requestNextFrame();
    }

    MemoryStatistics::showStatistics();

    return 0;
}
