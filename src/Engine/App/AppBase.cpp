/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/App/AppBase.h>
#include <osre/Common/ArgumentParser.h>
#include <osre/Properties/Settings.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractTimer.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/World.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/Assets/AssetRegistry.h>

// private includes
#include <src/Engine/Platform/PlatformPluginFactory.h>

namespace OSRE {
namespace App {

using namespace ::OSRE::Common;

const String API_Arg = "api";
const String Tag     = "AppBase";

struct AppBase::Impl {
    enum State {
        Uninited,
        Created,
        Running,
        Destroyed
    };
    State m_state;
    d32 m_timediff;
    ArgumentParser m_argParser;
    Properties::Settings *m_settings;
    Platform::PlatformInterface *m_platformInterface;
    Platform::AbstractTimer *m_timer;
    RenderBackend::RenderBackendService *m_rbService;
    Scene::World *m_world;
    
    Impl( i32 argc, c8 *argv[], const String &supportedArgs, const String &desc )
    : m_state( Uninited )
    , m_timediff( 0.0 )
    , m_argParser( argc, argv, supportedArgs, desc )
    , m_settings( nullptr )
    , m_platformInterface( nullptr )
    , m_timer( nullptr )
    , m_rbService( nullptr )
    , m_world( nullptr ) {
        m_settings = new Properties::Settings;
        m_settings->setString( Properties::Settings::RenderAPI, "opengl" );
        m_settings->setBool( Properties::Settings::PollingMode, true );

#ifdef OSRE_WINDOWS
        //pConfig->setInt( Properties::ConfigurationMap::PlatformPlugin, static_cast<i32>( Platform::SDL2Plugin) );
        m_settings->setInt( Properties::Settings::PlatformPlugin, static_cast< i32 >( Platform::PluginType::WindowsPlugin ) );
#else
        m_settings->setInt( Properties::Settings::PlatformPlugin, static_cast< i32 >( Platform::PluginType::SDL2Plugin ) );
#endif 
    }

    ~Impl(){
        m_settings = nullptr;
    }
};

AppBase::AppBase( i32 argc, c8 *argv[], const String &supportedArgs, const String &desc )
: m_impl( new Impl( argc, argv, supportedArgs, desc ) ) {
    // empty
}

AppBase::~AppBase() {
    delete m_impl;
    m_impl = nullptr;
}

bool AppBase::create( Properties::Settings *config ) {
    return onCreate( config  );
}

bool AppBase::destroy() {
    return onDestroy();
}

void AppBase::update() {
    OSRE_ASSERT( nullptr != m_impl );

    if( m_impl->m_state == Impl::Created ) {
        m_impl->m_state = Impl::Running;
        osre_debug( Tag, "Set application state to running." );
    }

    m_impl->m_timediff = m_impl->m_timer->getTimeDiff();

    onUpdate();
}

void AppBase::requestNextFrame() {
    OSRE_ASSERT( nullptr != m_impl );

    m_impl->m_rbService->update( m_impl->m_timediff );
}

bool AppBase::handleEvents() {
    OSRE_ASSERT( nullptr!=m_impl );

    if( nullptr == m_impl->m_platformInterface ) {
        osre_debug( Tag, "AppBase::PlatforInterface not in proper state: not nullptr." );
        return false;
    }

    return m_impl->m_platformInterface->update( m_impl->m_timediff );
}

Properties::Settings *AppBase::getSettings() const {
    if( nullptr == m_impl ) {
        return nullptr;
    }

    return m_impl->m_settings;
}

Scene::Stage *AppBase::createStage( const String &name ) {
    OSRE_ASSERT( nullptr!=m_impl );

    if ( name.empty() ) {
        return nullptr;
    }

    Scene::Stage *stage( new Scene::Stage( name, m_impl->m_rbService ) );
    if ( nullptr != stage ) {
        m_impl->m_world->addStage( stage );
    }

    return stage;
}

bool AppBase::activateStage( const String &name ) {
    OSRE_ASSERT( nullptr!=m_impl );

    if ( nullptr == m_impl->m_world ) {
        return false;
    }

    return m_impl->m_world->setActiveStage( name );
}

bool AppBase::onCreate( Properties::Settings *config ) {
    OSRE_ASSERT( nullptr!=m_impl );

    if ( m_impl->m_state!=Impl::Uninited ) {
        osre_debug( Tag, "AppBase::State not in proper state: Uninited." );
        return false;
    }

    // create the platform abstraction
    if( nullptr != config && config != m_impl->m_settings ) {
        delete m_impl->m_settings;
        m_impl->m_settings = config;
    }

    // create the asset registry
    Assets::AssetRegistry *registry( Assets::AssetRegistry::create() );
    OSRE_ASSERT( nullptr!=registry );
    if ( nullptr==registry ) {
        osre_debug( Tag, "Cannot create asset registry." );
    }

    // create the platform interface instance
    m_impl->m_platformInterface = Platform::PlatformInterface::create( m_impl->m_settings );
    if( m_impl->m_platformInterface ) {
        if( !m_impl->m_platformInterface->open() ) {
            return false;
        }
    }

    // register any available platform-specific log streams
    Common::AbstractLogStream *stream = Platform::PlatformPluginFactory::createPlatformLogStream();
    if( nullptr != stream ) {
        Logger::getInstance()->registerLogStream( stream );
    }

    // create the render backend
    m_impl->m_rbService = new RenderBackend::RenderBackendService();
    if( !m_impl->m_rbService->open() ) {
        m_impl->m_rbService->release();
        m_impl->m_rbService = nullptr;
    }

    // enable render-backend
    if( m_impl->m_platformInterface ) {
        RenderBackend::CreateRendererEventData *data = new RenderBackend::CreateRendererEventData( m_impl->m_platformInterface->getRootSurface() );
        m_impl->m_rbService->sendEvent( &RenderBackend::OnCreateRendererEvent, data );
    }
    m_impl->m_timer = Platform::PlatformInterface::getInstance()->getTimer();

    // create our world
    m_impl->m_world = new Scene::World( "world" );
    
    // set application state to "Created"
    osre_debug( Tag, "Set application state to Created." );
    m_impl->m_state = Impl::Created;
    
    return true;
}

bool AppBase::onDestroy() {
    OSRE_ASSERT( nullptr!=m_impl );

    if ( m_impl->m_state!=Impl::Running ) {
        osre_debug( Tag, "AppBase::State not in proper state: Running." );
        return false;
    }

    Assets::AssetRegistry::destroy();

    if( m_impl->m_platformInterface ) {
        Platform::PlatformInterface::destroy();
        m_impl->m_platformInterface = nullptr;
    }

    delete m_impl->m_world;
    m_impl->m_world = nullptr;

    osre_debug( Tag, "Set application state to destroyed." );
    m_impl->m_state = Impl::Destroyed;
    Logger::kill();

    return true;
}

void AppBase::onUpdate() {
    OSRE_ASSERT( nullptr != m_impl );

    m_impl->m_world->update( m_impl->m_rbService );
}

ArgumentParser &AppBase::getArgumentParser() const {
    return m_impl->m_argParser;
}

} // Namespace App
} // Namespace OSRE
