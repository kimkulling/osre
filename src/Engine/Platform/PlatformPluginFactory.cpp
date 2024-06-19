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
#include "Common/Logger.h"
#include "Debugging/osre_debugging.h"
#include "Platform/PlatformPluginFactory.h"
#ifdef OSRE_WINDOWS
#    include "Platform/win32/Win32SystemInfo.h"
#    include "Platform/win32/Win32OGLRenderContext.h"
#    include "Platform/win32/Win32Timer.h"
#    include "Platform/win32/Win32DbgLogStream.h"
#    include "Platform/win32/Win32DynamicLoader.h"
#    include "Platform/win32/Win32EventQueue.h"
#    include "Platform/win32/Win32Window.h"
#    include "Platform/win32/Win32OSService.h"
#else
#    include "Platform/sdl2/SDL2DynamicLoader.h"
#    include "Platform/sdl2/SDL2EventQueue.h"
#    include "Platform/sdl2/SDL2Initializer.h"
#    include "Platform/sdl2/SDL2OGLRenderContext.h"
#    include "Platform/sdl2/SDL2SystemInfo.h"
#    include "Platform/sdl2/SDL2Timer.h"
#    include "Platform/sdl2/SDL2Window.h"
#    include "Platform/sdl2/SDL2OSService.h"
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
    AbstractPlatformEventQueue *eventHandler = nullptr;
#ifdef OSRE_WINDOWS
    Win32Window *win32Window = static_cast<Win32Window *>(rootSurface);
    if (nullptr != win32Window) {
        eventHandler = new Win32EventQueue(win32Window);
        Win32EventQueue::registerEventQueue((Win32EventQueue *)eventHandler, win32Window->getHWnd());
    }
#else
    eventHandler = new SDL2EventHandler(rootSurface);
#endif // OSRE_WINDOWS
    rootSurface->setEventQueue(eventHandler);
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

AbstractOSService *PlatformPluginFactory::createOSService() {
    AbstractOSService *osService(nullptr);
#ifdef OSRE_WINDOWS
    osService = new Win32OSService;
#else
    osService = new SDL2OSService;
#endif

    return osService;
}

} // Namespace Platform
} // Namespace OSRE
