/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Platform/PlatformInterface.h>
#include <src/Engine/Platform/PlatformPluginFactory.h>
#include <osre/Common/EventTriggerer.h>
#include <osre/Properties/Settings.h>
#include <osre/UI/UiItemFactory.h>
#ifdef OSRE_WINDOWS
#   include <src/Engine/Platform/win32/Win32OGLRenderContext.h>
#endif // OSRE_WINDOWS
#include "Engine/Platform/sdl2/SDL2Window.h"
#include "Engine/Platform/sdl2/SDL2EventQueue.h"
#include <src/Engine/Platform/sdl2/SDL2OGLRenderContext.h>
#include <src/Engine/Platform/sdl2/SDL2Timer.h>
#include <src/Engine/Platform/sdl2/SDL2SystemInfo.h>

#include <GL/glew.h>
#include <SDL.h>

namespace OSRE {
namespace Platform {

using namespace ::OSRE::Common;
using namespace ::OSRE::Properties;
using namespace ::OSRE::UI;

PlatformInterface *PlatformInterface::s_instance( nullptr );

ApplicationContext::ApplicationContext(const Settings* config)
: m_config(config)
#ifdef OSRE_WINDOWS
, m_type(PluginType::WindowsPlugin)
#else
, m_type(PluginType::SDL2Plugin)
#endif // OSRE_WINDOWS
, m_rootSurface(nullptr)
, m_oseventHandler(nullptr)
, m_renderContext(nullptr)
, m_timer(nullptr)
, m_dynLoader(nullptr)
, m_systemInfo(nullptr) {
    // empty
}

ApplicationContext::~ApplicationContext() {
    m_config = nullptr;
}

static const String PlatformPluginName[ static_cast<i32>( PluginType::MaxPlugin ) ] = {
#ifdef OSRE_WINDOWS
    "WindowsPlugin"
#else
    "SDL2Plugin"
#endif // OSRE_WINDOWS
};

static const c8 *Tag = "PlatformInterface";

PlatformInterface::PlatformInterface( const Settings *config )
: AbstractService( "platform/platforminterface" )
, m_context( nullptr ) {
    m_context = new ApplicationContext(config);
}

PlatformInterface::~PlatformInterface() {
    delete m_context;
    m_context = nullptr;
}

PlatformInterface *PlatformInterface::create( const Settings *config ) {
    if( nullptr == s_instance ) {
        s_instance = new PlatformInterface( config );
    }

    return s_instance;
}

void PlatformInterface::destroy( ) {
    if( nullptr == s_instance ) {
        return;
    }

    s_instance->close();
    delete s_instance;
    s_instance = nullptr;
}

PlatformInterface *PlatformInterface::getInstance( ) {
    return s_instance;
}

AbstractPlatformEventQueue *PlatformInterface::getPlatformEventHandler() const {
    return m_context->m_oseventHandler;
}

AbstractOGLRenderContext *PlatformInterface::getRenderContext() const {
    return m_context->m_renderContext;
}

AbstractWindow *PlatformInterface::getRootWindow() const {
    return m_context->m_rootSurface;
}

AbstractTimer *PlatformInterface::getTimer() const {
    return m_context->m_timer;
}

AbstractDynamicLoader *PlatformInterface::getDynamicLoader() const {
    return m_context->m_dynLoader;
}

AbstractSystemInfo *PlatformInterface::getSystemInfo() const {
    return m_context->m_systemInfo;
}

const String &PlatformInterface::getDefaultFontName() const {
    if ( nullptr == m_context->m_config ) {
        static const String dummy( "none" );
        return dummy;
    }

    static const String font(m_context->m_config->get( Settings::DefaultFont ).getString() );
    return font;
}

PluginType PlatformInterface::getOSPluginType() {
#ifdef OSRE_WINDOWS
    return PluginType::WindowsPlugin;
#else 
    return PluginType::SDL2Plugin;
#endif // OSRE_WINDOWS
}

String PlatformInterface::getOSPluginName( PluginType type ) {
    String name( "None" );
    switch( type ) {
#ifdef OSRE_WINDOWS
        case PluginType::WindowsPlugin:
            name = PlatformPluginName[ static_cast<i32>( PluginType::WindowsPlugin ) ];
            break;
#else
        case PluginType::SDL2Plugin:
            name = PlatformPluginName[ static_cast<i32>( PluginType::SDL2Plugin ) ];
            break;
#endif // OSRE_WINDOWS
        default:
            break;
    }

    return name;
}

bool PlatformInterface::onOpen() {
    if( !m_context->m_config ) {
        assert( nullptr != m_context->m_config );
        osre_debug( Tag, "Invalid pointer to configuration." );
        return false;
    }

    Settings::WorkingModeType appType = 
        static_cast<Settings::WorkingModeType>(m_context->m_config->get( Settings::AppType ).getInt() );

    WindowsProperties *props( nullptr );
    bool polls( false );
    const Properties::Settings* config = m_context->m_config;
    if( appType == Settings::GfxApp ) {
        // get the configuration values for the window
        props = new WindowsProperties;
        bool fullscreen = false;
        props->m_x = config->get( Settings::WinX ).getInt();
        props->m_y = config->get( Settings::WinY ).getInt();
        props->m_width = config->get( Settings::WinWidth ).getInt();
        props->m_height = config->get( Settings::WinHeight ).getInt();
        props->m_colordepth = config->get( Settings::BPP ).getByte();
        props->m_depthbufferdepth = config->get( Settings::DepthBufferDepth ).getByte();
        props->m_stencildepth = config->get( Settings::StencilBufferDepth ).getByte();
        props->m_fullscreen = fullscreen;
        props->m_resizable = config->get( Settings::WindowsResizable ).getBool();
        props->m_childWindow = config->get(Settings::ChildWindow).getBool();
        props->m_title = config->get( Settings::WindowsTitle ).getString();
        polls = config->get( Settings::PollingMode ).getBool();
    }

    String appName = "My OSRE-Application";

    PlatformPluginFactory::init();
#ifdef OSRE_WINDOWS
    osre_info( Tag, "Platform plugin created for Windows.");
#else
    osre_info(Tag, "Platform plugin created for Linux.");
#endif
    m_context->m_dynLoader = PlatformPluginFactory::createDynmicLoader();
    bool result( true );
    if( appType == Settings::GfxApp ) {
        result = setupGfx( props, polls );
    }

    m_context->m_systemInfo = PlatformPluginFactory::createSystemInfo();

    return result;
}

bool PlatformInterface::onClose( ) {
    PlatformPluginFactory::release();

    delete m_context->m_oseventHandler;
    m_context->m_oseventHandler = nullptr;

    if( nullptr != m_context->m_renderContext ) {
        m_context->m_renderContext->destroy();
        delete m_context->m_renderContext;
        m_context->m_renderContext = nullptr;
    }

    return true;
}

bool PlatformInterface::onUpdate() {
    if (nullptr == m_context->m_oseventHandler) {
        return false;
    }

    return m_context->m_oseventHandler->update();
}

bool PlatformInterface::setupGfx( WindowsProperties *props, bool polls ) {
    // create the root surface
    m_context->m_rootSurface = PlatformPluginFactory::createSurface( props );
    if( !m_context->m_rootSurface->create() ) {
        delete m_context->m_rootSurface;
        osre_error( Tag, "Error while creating platform root surface." );

        m_context->m_rootSurface = nullptr;
        return false;
    }

    // install the platform event handler
    m_context->m_oseventHandler = PlatformPluginFactory::createPlatformEventHandler(m_context->m_rootSurface );
    if( !m_context->m_oseventHandler ) {
        osre_error( Tag, "Error while creating platform event handler." );
        m_context->m_rootSurface->destroy();
        m_context->m_rootSurface = nullptr;
        return false;
    }
    m_context->m_oseventHandler->enablePolling( polls );
    m_context->m_timer = PlatformPluginFactory::createTimer();

    // setup the render context
    m_context->m_renderContext = PlatformPluginFactory::createRenderContext();

    UiItemFactory::createInstance( getRootWindow() );
        
    return true;
}

} // Namespace Platform
} // Namespace OSRE
