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
#include <osre/Common/Logger.h>
#include <osre/Debugging/osre_debugging.h>
#include <src/Engine/Platform/PlatformPluginFactory.h>
#ifdef OSRE_WINDOWS
#    include "Engine/Platform/win32/Win32SystemInfo.h"
#    include <src/Engine/Platform/win32/Win32OGLRenderContext.h>
#    include <src/Engine/Platform/win32/Win32Timer.h>
#    include <src/Engine/Platform/win32/Win32DbgLogStream.h>
#    include <src/Engine/Platform/win32/Win32DynamicLoader.h>
#    include <src/Engine/Platform/win32/Win32EventQueue.h>
#    include <src/Engine/Platform/win32/Win32Window.h>
#else
#    include <src/Engine/Platform/sdl2/SDL2DynamicLoader.h>
#    include <src/Engine/Platform/sdl2/SDL2EventQueue.h>
#    include <src/Engine/Platform/sdl2/SDL2Initializer.h>
#    include <src/Engine/Platform/sdl2/SDL2OGLRenderContext.h>
#    include <src/Engine/Platform/sdl2/SDL2SystemInfo.h>
#    include <src/Engine/Platform/sdl2/SDL2Timer.h>
#    include <src/Engine/Platform/sdl2/SDL2Window.h>
#endif

namespace OSRE {
namespace Platform {

bool PlatformPluginFactory::init() {
#ifndef OSRE_WINDOWS
    return SDL2Initializer::init();
#endif

    return true;
}

bool PlatformPluginFactory::release() {
#ifdef OSRE_LINUX
    if (type == PluginType::SDL2Plugin) {
        return SDL2Initializer::release();
    }
#endif
    return true;
}

AbstractPlatformEventQueue *PlatformPluginFactory::createPlatformEventHandler(AbstractWindow *rootSurface) {
    AbstractPlatformEventQueue *eventHandler(nullptr);
#ifdef OSRE_WINDOWS
    Win32Window *win32Surface = static_cast<Win32Window *>(rootSurface);
    if (nullptr != win32Surface) {
        eventHandler = new Win32EventQueue(win32Surface);
        Win32EventQueue::registerEventQueue((Win32EventQueue *)eventHandler, win32Surface->getHWnd());
    }
#else
    eventHandler = new SDL2EventHandler(rootSurface);
#endif // OSRE_WINDOWS

    osre_assert(nullptr != eventHandler);

    return eventHandler;
}

AbstractWindow *PlatformPluginFactory::createSurface(WindowsProperties *props) {
    AbstractWindow *surface(nullptr);
#ifdef OSRE_WINDOWS
    surface = new Win32Window(props);
#else
    surface = new SDL2Surface(props);
#endif // OSRE_WINDOWS
    osre_assert(nullptr != surface);

    return surface;
}

AbstractOGLRenderContext *PlatformPluginFactory::createRenderContext() {
    AbstractOGLRenderContext *renderCtx(nullptr);
#ifdef OSRE_WINDOWS
    renderCtx = new Win32RenderContext();
#else
    renderCtx = new SDL2RenderContext();
#endif // OSRE_WINDOWS
    osre_assert(nullptr != renderCtx);

    return renderCtx;
}

AbstractTimer *PlatformPluginFactory::createTimer() {
    AbstractTimer *timer(nullptr);
#ifdef OSRE_WINDOWS
    timer = new Win32Timer();
#else
    timer = new SDL2Timer();
#endif // OSRE_WINDOWS

    return timer;
}

Common::AbstractLogStream *PlatformPluginFactory::createPlatformLogStream() {
    Common::AbstractLogStream *stream(nullptr);
#ifdef OSRE_WINDOWS
    stream = new Win32DbgLogStream;
#endif // OSRE_WINDOWS

    return stream;
}

AbstractDynamicLoader *PlatformPluginFactory::createDynmicLoader() {
    AbstractDynamicLoader *dynloader(nullptr);
#ifdef OSRE_WINDOWS
    dynloader = new Win32DynamicLoader;
#else
    dynloader = new SDL2DynamicLoader();
#endif // OSRE_WINDOWS

    return dynloader;
}

AbstractSystemInfo *PlatformPluginFactory::createSystemInfo() {
    AbstractSystemInfo *sysInfo(nullptr);
#ifdef OSRE_WINDOWS
    sysInfo = new Win32SystemInfo;
#else
    sysInfo = new SDL2SystemInfo;
#endif // OSRE_WINDOWS

    return sysInfo;
}

} // Namespace Platform
} // Namespace OSRE
