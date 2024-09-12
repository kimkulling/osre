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
#include <stdio.h>
#include <iostream>

#define main main

#include "App/App.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/MeshBuilder.h"
#include "Common/Logger.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/TransformMatrixBlock.h"
#include "App/Entity.h"
#include "Platform/AbstractWindow.h"
#include "Common/glm_common.h"
//#include "Platform/PlatformOperations.h"
#include "Platform/PlatformInterface.h"
#include "Platform/win32/Win32Window.h"

#include "OSREEdApp.h"

using namespace OSRE;
using namespace OSRE::RenderBackend;
using namespace OSRE::App;
using namespace OSRE::Platform;
using namespace OSRE::Editor;

static constexpr c8 Tag[] = "HelloWorldApp";

int main(int argc, char *argv[]) {
    std::cout << "Editor version 0.1\n";

    OsreEdApp osreApp(argc, argv);
    if (!osreApp.initWindow(100, 100, 1024, 768, "test", WindowMode::Windowed, WindowType::Root, App::RenderBackendType::OpenGLRenderBackend)) {
        return -1;
    }


    // Main loop
    bool done = false;
    while (!done) {
        done = !osreApp.handleEvents();
        osreApp.update();
        osreApp.requestNextFrame();
    }

    return 0;
}
