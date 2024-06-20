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

#include "Platform/PluginType.h"

namespace OSRE {

// Forward declarations
namespace Common {
    class AbstractLogStream;
}

namespace RenderBackend {
    class RenderBackendService;
}

namespace Platform {

class AbstractPlatformEventQueue;
class AbstractWindow;
class AbstractOGLRenderContext;
class AbstractTimer;
class AbstractThreadFactory;
class AbstractDynamicLoader;
class AbstractSystemInfo;

struct WindowsProperties;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Infrastructure
///
///	@brief  This class is used to create all platform-specific instances.
//-------------------------------------------------------------------------------------------------
struct PlatformPluginFactory {
    /// @brief  Will init the factory.
    static bool init();

    /// @brief  Will release the factory.
    static bool release();

    /// @brief  Creates a platform-specific event handler instance.
    static AbstractPlatformEventQueue *createPlatformEventHandler( AbstractWindow *rootSurface );

    /// @brief  Creates a platform-specific surface instance.
    static AbstractWindow *createSurface(WindowsProperties *pProps );

    /// @brief  Creates a platform-specific render context.
    static AbstractOGLRenderContext *createRenderContext();

    /// @brief  Creates a platform-specific timer instance.
    static AbstractTimer *createTimer();

    ///	@brief  Creates a platform-specific log stream, if any available.
    static Common::AbstractLogStream *createPlatformLogStream();

    ///	@brief  Creates a platform-specific dynamic-library loader.
    static AbstractDynamicLoader *createDynmicLoader();

    /// @brief  CReates the platform-specific system, info instance.
    static AbstractSystemInfo *createSystemInfo();

    ///	@brief  Will create the OS-Service.
    /// @return The OS-Serviec instance.
    static AbstractOSService *createOSService();
};

} // Namespace Platform
} // Namespace OSRE
