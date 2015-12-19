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
#include <src/Platform/PlatformPluginFactory.h>
#include <osre/Common/Logger.h>
#ifdef _WIN32
#   include <src/Platform/win32/Win32Surface.h>
#   include <src/Platform/win32/Win32Eventhandler.h>
#   include <src/Platform/win32/Win32Timer.h>
#   include <src/Platform/win32/Win32RenderContext.h>
#   include <src//Platform/win32/Win32ThreadFactory.h>
#   include <src/Platform/win32/Win32DbgLogStream.h>
#endif
#include <src/Platform/sdl2/SDL2Surface.h>
#include <src/Platform/sdl2/SDL2EventHandler.h>
#include <src/Platform/sdl2/SDL2RenderContext.h>
#include <src/Platform/sdl2/SDL2Timer.h>
#include <src/Platform/sdl2/SDL2ThreadFactory.h>
#include <src/Platform/sdl2/SDL2Initializer.h>

namespace OSRE {
namespace Platform {

static const String Tag = "PlatformPluginFactory";

//-------------------------------------------------------------------------------------------------
bool PlatformPluginFactory::init( PluginType type ) {
    static_cast< void >( createThreadFactory( type ) );
    if( type == SDL2Plugin ) {
        return SDL2Initializer::init();
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
bool PlatformPluginFactory::release( PluginType type ) {
    if( type == SDL2Plugin ) {
        return SDL2Initializer::release();
    }
    return true;
}

//-------------------------------------------------------------------------------------------------
AbstractPlatformEventHandler *PlatformPluginFactory::createPlatformEventHandler( PluginType type, AbstractSurface *rootSurface ) {
    AbstractPlatformEventHandler *pEventHandler( nullptr );
    switch( type ) {
#ifdef OSRE_WINDOWS
        case Platform::WindowsPlugin: {
                Win32Surface *win32Surface = ( Win32Surface* ) rootSurface;
                if( win32Surface ) {
                    pEventHandler = new Win32Eventhandler;
                    Win32Eventhandler::registerEventServer( ( Win32Eventhandler* ) pEventHandler, win32Surface->getHWnd() );
                }
            }
            break;
#endif // OSRE_WINDOWS

        case Platform::SDL2Plugin:
            pEventHandler = new SDL2EventHandler();
            break;

        default:
            break;
    }

    OSRE_ASSERT(nullptr != pEventHandler);

    return pEventHandler;
}

//-------------------------------------------------------------------------------------------------
AbstractSurface *PlatformPluginFactory::createSurface( PluginType type, SurfaceProperties *pProps ) {
    AbstractSurface *pSurface( nullptr );
    switch( type ) {
#ifdef OSRE_WINDOWS
        case Platform::WindowsPlugin:
            pSurface = new Win32Surface( pProps );
            break;
#endif // OSRE_WINDOWS

        case Platform::SDL2Plugin:
            pSurface = new SDL2Surface( pProps );
            break;

        default:
            osre_info( Tag, "Enum value not handled." );
            break;
    }
    OSRE_ASSERT( nullptr != pSurface);

    return pSurface;
}

//-------------------------------------------------------------------------------------------------
AbstractRenderContext *PlatformPluginFactory::createRenderContext( PluginType type ) {
    AbstractRenderContext *renderCtx( nullptr );
    switch( type ) {
#ifdef OSRE_WINDOWS
        case Platform::WindowsPlugin:
            renderCtx = new Win32RenderContext();
            break;
#endif // OSRE_WINDOWS

        case Platform::SDL2Plugin:
            renderCtx = new SDL2RenderContext();
            break;

        default:
            osre_error( Tag, "Enum value not handled." );
            break;
    }
    OSRE_ASSERT(nullptr != renderCtx);

    return renderCtx;
}

//-------------------------------------------------------------------------------------------------
AbstractTimer *PlatformPluginFactory::createTimer( PluginType type ) {
    AbstractTimer *pTimer( nullptr );
    switch( type ) {
#ifdef OSRE_WINDOWS
        case Platform::WindowsPlugin:
            pTimer = new Win32Timer();
            break;
#endif

        case Platform::SDL2Plugin:
            pTimer = new SDL2Timer();
            break;

        default:
            break;
    }
    return pTimer;
}

//-------------------------------------------------------------------------------------------------
AbstractThreadFactory *PlatformPluginFactory::createThreadFactory( PluginType type ) {
    AbstractThreadFactory *instance( nullptr );
    switch( type ) {
#ifdef OSRE_WINDOWS
        case Platform::WindowsPlugin:
        instance = new Win32ThreadFactory();
        break;
#endif // OSRE_WINDOWS

    case Platform::SDL2Plugin:
        instance = new SDL2ThreadFactory();
        break;

    default:
        break;
    }

    osre_info( Tag, "Set thread factory." );
    AbstractThreadFactory::setInstance( instance );

    return instance;
}

//-------------------------------------------------------------------------------------------------
Common::AbstractLogStream *PlatformPluginFactory::createPlatformLogStream() {
    Common::AbstractLogStream *stream( nullptr );
#ifdef OSRE_WINDOWS
    stream = new Win32DbgLogStream;
#endif // OSRE_WINDOWS

    return stream;
}


//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace OSRE
