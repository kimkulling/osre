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

#include <osre/Platform/PluginType.h>

namespace OSRE {
namespace Platform {

class AbstractPlatformEventHandler;
class AbstractSurface;
class AbstractRenderContext;
class AbstractTimer;
class AbstractThreadFactory;

struct SurfaceProperties;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::PlatformPluginFactory
///	@ingroup	Infrastructure
///
///	@brief  This class is used to create all platform-specific instances.
//-------------------------------------------------------------------------------------------------
struct PlatformPluginFactory {
    /// @brief  Will init the factory.
    static bool init( PluginType type );

    /// @brief  Will release the factory.
    static bool release( PluginType type );

    /// @brief  Creates a platform-specific event handler instance.
    static AbstractPlatformEventHandler *createPlatformEventHandler( PluginType type, AbstractSurface *rootSurface );

    /// @brief  Creates a platform-specific surface instance.
    static AbstractSurface *createSurface( PluginType type, SurfaceProperties *pProps );

    /// @brief  Creates a platform-specific render context.
    static AbstractRenderContext *createRenderContext( PluginType type );

    /// @brief  Creates a platform-specific timer instance.
    static AbstractTimer *createTimer( PluginType type );

    /// @brief  Creates a platform-specific thread factory instance.
    static AbstractThreadFactory *createThreadFactory( PluginType type );
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
