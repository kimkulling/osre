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
#include <osre/Platform/PlatformInterface.h>
#include <osre/Common/EventTriggerer.h>
#include <src/Platform/PlatformPluginFactory.h>

#ifdef CE_WINDOWS
#   include <src/Platform/win32/Win32Surface.h>
#   include <src/Platform/win32/Win32Eventhandler.h>
#   include <src/Platform/win32/Win32Timer.h>
#   include <src/Platform/win32/Win32RenderContext.h>
#   include <src//Platform/win32/Win32ThreadFactory.h>
#endif // CE_WINDOWS
#include <src/Platform/sdl2/SDL2Surface.h>
#include <src/Platform/sdl2/SDL2EventHandler.h>
#include <src/Platform/sdl2/SDL2RenderContext.h>
#include <src/Platform/sdl2/SDL2Timer.h>
#include <src/Platform/sdl2/SDL2ThreadFactory.h>

#include <GL/glew.h>
#include <SDL.h>

namespace OSRE {
namespace Platform {

using namespace ::OSRE::Common;
using namespace ::OSRE::Properties;

PlatformInterface *PlatformInterface::s_instance( nullptr );

static const String PlatformPluginName[ MaxPlugin ] = {
#ifdef CE_WINDOWS
    "WindowsPlugin",
#endif // CE_WINDOWS
    "SDL2Plugin"
};


//-------------------------------------------------------------------------------------------------
PlatformInterface::PlatformInterface( const ConfigurationMap *config )
: AbstractService( "platform/platforminterface" )
, m_pConfiguration( config )
#ifdef CE_WINDOWS
, m_type( WindowsPlugin )
#else
, m_type( SDL2Plugin )
#endif // CE_WINDOWS
, m_pRootSurface( nullptr )
, m_pOSEventHandler( nullptr )
, m_pTimer( nullptr ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
PlatformInterface::~PlatformInterface() {
    // empty
}

//-------------------------------------------------------------------------------------------------
PlatformInterface *PlatformInterface::create( const ConfigurationMap *pConfiguration ) {
    if( nullptr == s_instance ) {
        s_instance = new PlatformInterface( pConfiguration );
    }

    return s_instance;
}

//-------------------------------------------------------------------------------------------------
void PlatformInterface::destroy( ) {
    if( nullptr == s_instance ) {
        return;
    }

    s_instance->close();
    delete s_instance;
    s_instance = nullptr;
}

//-------------------------------------------------------------------------------------------------
PlatformInterface *PlatformInterface::getInstance( ) {
    return s_instance;
}

//-------------------------------------------------------------------------------------------------
AbstractPlatformEventHandler *PlatformInterface::getPlatformEventHandler() const {
    return m_pOSEventHandler;
}

//-------------------------------------------------------------------------------------------------
AbstractRenderContext *PlatformInterface::getRenderContext() const {
    return m_pRenderContext;
}

//-------------------------------------------------------------------------------------------------
AbstractSurface *PlatformInterface::getRootSurface() const {
    return m_pRootSurface;
}

//-------------------------------------------------------------------------------------------------
AbstractTimer *PlatformInterface::getTimer() const {
    return m_pTimer;
}

//-------------------------------------------------------------------------------------------------
PluginType PlatformInterface::getOSPluginType() {
#ifdef CE_WINDOWS
    return WindowsPlugin;
#else 
    return SDL2Plugin;
#endif // CE_WINDOWS
}

//-------------------------------------------------------------------------------------------------
String PlatformInterface::getOSPluginName( PluginType type ) {
    String name( "None" );
    switch( type ) {
#ifdef CE_WINDOWS
        case WindowsPlugin:
            name = PlatformPluginName[ WindowsPlugin ];
            break;
#endif // CE_WINDOWS

        case SDL2Plugin:
            name = PlatformPluginName[ SDL2Plugin ];
            break;
        default:
            break;
    }

    return name;
}

//-------------------------------------------------------------------------------------------------
bool PlatformInterface::onOpen() {
    if( !m_pConfiguration ) {
        assert( nullptr != m_pConfiguration );
        osre_debug( "Invalid pointer to configuration." );
        return false;
    }

    ConfigurationMap::WorkingModeType appType = 
        ( ConfigurationMap::WorkingModeType ) m_pConfiguration->get( ConfigurationMap::AppType ).getInt();

    SurfaceProperties *pProps( nullptr );
    bool polls( false );
    if( appType == ConfigurationMap::GfxApp ) {
        // get the configuration values for the window
        pProps = new SurfaceProperties;
        bool fullscreen = false;
        pProps->m_x = m_pConfiguration->get( ConfigurationMap::WinX ).getInt();
        pProps->m_y = m_pConfiguration->get( ConfigurationMap::WinY ).getInt();
        pProps->m_width = m_pConfiguration->get( ConfigurationMap::WinWidth ).getInt();
        pProps->m_height = m_pConfiguration->get( ConfigurationMap::WinHeight ).getInt();
        pProps->m_colordepth = m_pConfiguration->get( ConfigurationMap::BPP ).getInt();
        pProps->m_depthbufferdepth = m_pConfiguration->get( ConfigurationMap::DepthBufferDepth ).getInt();
        pProps->m_stencildepth = m_pConfiguration->get( ConfigurationMap::StencilBufferDepth ).getInt();
        pProps->m_fullscreen = fullscreen;
        pProps->m_title = m_pConfiguration->get( ConfigurationMap::WindowsTitle ).getString();
        polls = m_pConfiguration->get( ConfigurationMap::PollingMode ).getBool();
    }

    ce_string appName = "ZFXCE2";
    m_type = static_cast<PluginType>( m_pConfiguration->get( ConfigurationMap::PlatformPlugin ).getInt( ) );

    PlatformPluginFactory::init( m_type );
    ce_log( "Platform plugin created for " + PlatformInterface::getOSPluginName( m_type ) );

    PlatformPluginFactory::createThreadFactory( m_type );

    bool result( true );
    if( appType == ConfigurationMap::GfxApp ) {
        result = setupGfx( pProps, polls );
    }

    return result;
}

//-------------------------------------------------------------------------------------------------
bool PlatformInterface::onClose( ) {
    PlatformPluginFactory::release( m_type );

    delete m_pOSEventHandler;
    m_pOSEventHandler = nullptr;

    if( nullptr != m_pRenderContext ) {
        m_pRenderContext->destroy();
        delete m_pRenderContext;
        m_pRenderContext = nullptr;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
bool PlatformInterface::onUpdate( d32 timediff ) {
    Common::Event ev( "none" );
    return m_pOSEventHandler->onEvent( ev, nullptr );
}

//-------------------------------------------------------------------------------------------------
bool PlatformInterface::setupGfx( SurfaceProperties *props, bool polls ) {
    // create the root surface
    m_pRootSurface = PlatformPluginFactory::createSurface( m_type, props );
    if( !m_pRootSurface->create() ) {
        delete m_pRootSurface;
        ce_error( "Error while creating platform root surface." );

        m_pRootSurface = nullptr;
        return false;
    }

    // install the platform event handler
    m_pOSEventHandler = PlatformPluginFactory::createPlatformEventHandler( m_type, m_pRootSurface );
    if( !m_pOSEventHandler ) {
        ce_error( "Error while creating platform event handler." );
        m_pRootSurface->destroy();
        m_pRootSurface = nullptr;
        return false;
    }
    m_pOSEventHandler->enablePolling( polls );
    m_pTimer = PlatformPluginFactory::createTimer( m_type );

    // setup the render context
    m_pRenderContext = PlatformPluginFactory::createRenderContext( m_type );

    return true;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
