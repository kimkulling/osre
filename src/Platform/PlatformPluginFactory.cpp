/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-214, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions
and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
and the following disclaimer in the documentation and/or other materials provided with the
distribution.

* Neither the name of the ZFXCE2 team, nor the names of its contributors may be used to endorse or
promote products derived from this software without specific prior written permission of the
ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
#include <src/Platform/PlatformPluginFactory.h>
#include <osre/Common/Logger.h>
#ifdef _WIN32
#   include <src/Platform/win32/Win32Surface.h>
#   include <src/Platform/win32/Win32Eventhandler.h>
#   include <src/Platform/win32/Win32Timer.h>
#   include <src/Platform/win32/Win32RenderContext.h>
#   include <src//Platform/win32/Win32ThreadFactory.h>
#endif
#include <src/Platform/sdl2/SDL2Surface.h>
#include <src/Platform/sdl2/SDL2EventHandler.h>
#include <src/Platform/sdl2/SDL2RenderContext.h>
#include <src/Platform/sdl2/SDL2Timer.h>
#include <src/Platform/sdl2/SDL2ThreadFactory.h>
#include <src/Platform/sdl2/SDL2Initializer.h>

namespace OSRE {
namespace Platform {

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
#ifdef _WIN32
        case Platform::WindowsPlugin: {
                Win32Surface *win32Surface = ( Win32Surface* ) rootSurface;
                if( win32Surface ) {
                    pEventHandler = new Win32Eventhandler;
                    Win32Eventhandler::registerEventServer( ( Win32Eventhandler* ) pEventHandler, win32Surface->getHWnd() );
                }
            }
            break;
#endif // CE_WINDOWS

        case Platform::SDL2Plugin:
            pEventHandler = new SDL2EventHandler();
            break;

        default:
            break;
    }

    return pEventHandler;
}

//-------------------------------------------------------------------------------------------------
AbstractSurface *PlatformPluginFactory::createSurface( PluginType type, SurfaceProperties *pProps ) {
    AbstractSurface *pSurface( nullptr );
    switch( type ) {
#ifdef _WIN32
        case Platform::WindowsPlugin:
            pSurface = new Win32Surface( pProps );
            break;
#endif // CE_WINDOWS

        case Platform::SDL2Plugin:
            pSurface = new SDL2Surface( pProps );
            break;

        default:
            osre_log( "Enum value not handled." );
            break;
    }

    return pSurface;
}

//-------------------------------------------------------------------------------------------------
AbstractRenderContext *PlatformPluginFactory::createRenderContext( PluginType type ) {
    AbstractRenderContext *renderCtx( nullptr );
    switch( type ) {
#ifdef _WIN32
        case Platform::WindowsPlugin:
            renderCtx = new Win32RenderContext();
            break;
#endif // CE_WINDOWS

        case Platform::SDL2Plugin:
            renderCtx = new SDL2RenderContext();
            break;

        default:
            osre_error( "Enum value not handled." );
            break;
    }

    return renderCtx;
}

//-------------------------------------------------------------------------------------------------
AbstractTimer *PlatformPluginFactory::createTimer( PluginType type ) {
    AbstractTimer *pTimer( nullptr );
    switch( type ) {
#ifdef _WIN32
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
#ifdef _WIN32
        case Platform::WindowsPlugin:
        instance = new Win32ThreadFactory();
        break;
#endif // CE_WINDOWS

    case Platform::SDL2Plugin:
        instance = new SDL2ThreadFactory();
        break;

    default:
        break;
    }

    osre_log( " set thread factory." );
    AbstractThreadFactory::setInstance( instance );

    return instance;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
