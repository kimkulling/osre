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
#include <osre/App/ServiceProvider.h>
#include <osre/Properties/Settings.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractTimer.h>
#include <osre/Platform/AbstractSurface.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/Parameter.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/World.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/UI/Screen.h>

// private includes
#include <src/Engine/Platform/PlatformPluginFactory.h>

namespace OSRE {
namespace App {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;

const String API_Arg = "api";
const String Tag     = "AppBase";

AppBase::AppBase( i32 argc, c8 *argv[], const String &supportedArgs, const String &desc )
: m_state( State::Uninited )
, m_timediff( 0.0 )
, m_argParser( argc, argv, supportedArgs, desc )
, m_settings( nullptr )
, m_platformInterface( nullptr )
, m_timer( nullptr )
, m_rbService( nullptr )
, m_world( nullptr )
, m_uiScreen( nullptr )
, m_shutdownRequested( false ) {
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

AppBase::~AppBase() {
    delete m_settings;
    m_settings = nullptr;
}

bool AppBase::create( Properties::Settings *config ) {
    return onCreate( config  );
}

bool AppBase::destroy() {
    return onDestroy();
}

void AppBase::update() {
    if( m_state == State::Created ) {
        m_state = State::Running;
        osre_debug( Tag, "Set application state to running." );
    }

    m_timediff = m_timer->getTimeDiff();

    onUpdate( m_timediff );
}

void AppBase::requestNextFrame() {
    m_rbService->update( m_timediff );
}

bool AppBase::handleEvents() {
    if( nullptr == m_platformInterface ) {
        osre_debug( Tag, "AppBase::PlatforInterface not in proper state: not nullptr." );
        return false;
    }

    return m_platformInterface->update( m_timediff );
}

Properties::Settings *AppBase::getSettings() const {
    return m_settings;
}

Scene::Stage *AppBase::createStage( const String &name ) {
    if ( name.empty() ) {
        return nullptr;
    }

    Scene::Stage *stage( new Scene::Stage( name, m_rbService ) );
    if ( nullptr != stage ) {
        m_world->addStage( stage );
    }

    return stage;
}

bool AppBase::setActiveStage( Scene::Stage *stage ) {
    if ( nullptr == m_world ) {
        return false;
    }
    return m_world->setActiveStage( stage );
}

bool AppBase::activateStage( const String &name ) {
    if ( nullptr == m_world ) {
        return false;
    }

    return m_world->setActiveStage( name );
}
void AppBase::requestShutdown() {
    m_shutdownRequested = true;
}

bool AppBase::shutdownRequested() const {
    return m_shutdownRequested;
}

UI::Screen *AppBase::createScreen( const String &name ) {
    if ( name.empty() ) {
        return nullptr;
    }

    AbstractSurface *surface( m_platformInterface->getRootSurface() );
    i32 w( 100 ), h( 100 );
    if ( nullptr != surface ) {
        w = surface->getProperties()->m_width;
        h = surface->getProperties()->m_height;
    }
    UI::Screen *newScreen = new UI::Screen( name, nullptr, w, h );
    m_uiScreen = newScreen;

    return newScreen;
}

void AppBase::setUIScreen( UI::Screen *uiScreen ) {
    if ( m_uiScreen != uiScreen ) {
        m_uiScreen = uiScreen;
    }
}

bool AppBase::onCreate( Properties::Settings *config ) {
    if ( m_state != State::Uninited ) {
        osre_debug( Tag, "AppBase::State not in proper state: Uninited." );
        return false;
    }

    // create the platform abstraction
    if( nullptr != config && config != m_settings ) {
        delete m_settings;
        m_settings = config;
    }

    // create the asset registry
    Assets::AssetRegistry *registry( Assets::AssetRegistry::create() );
    OSRE_ASSERT( nullptr!=registry );
    if ( nullptr==registry ) {
        osre_debug( Tag, "Cannot create asset registry." );
    }

    // create the platform interface instance
    m_platformInterface = Platform::PlatformInterface::create( m_settings );
    if( m_platformInterface ) {
        if( !m_platformInterface->open() ) {
            return false;
        }
    }

    // register any available platform-specific log streams
    Common::AbstractLogStream *stream = Platform::PlatformPluginFactory::createPlatformLogStream();
    if( nullptr != stream ) {
        Logger::getInstance()->registerLogStream( stream );
    }

    // create the render backend
    m_rbService = new RenderBackend::RenderBackendService();
    if( !m_rbService->open() ) {
        m_rbService->release();
        m_rbService = nullptr;
        return false;
    }

    if ( nullptr == RenderBackend::ParameterRegistry::create( m_rbService ) ) {
        m_rbService->release();
        return false;
    }

    // enable render-backend
    if( m_platformInterface ) {
        RenderBackend::CreateRendererEventData *data = new RenderBackend::CreateRendererEventData( m_platformInterface->getRootSurface() );
        m_rbService->sendEvent( &RenderBackend::OnCreateRendererEvent, data );
    }
    m_timer = Platform::PlatformInterface::getInstance()->getTimer();

    // create our world
    Scene::RenderMode mode = static_cast<Scene::RenderMode>( m_settings->get( Properties::Settings::RenderMode ).getInt() );
    m_world = new Scene::World( "world", mode );
    
    ServiceProvider::create( m_rbService );

    // set application state to "Created"
    osre_debug( Tag, "Set application state to Created." );
    m_state = State::Created;
    
    return true;
}

bool AppBase::onDestroy() {
    if ( m_state != State::Running ) {
        osre_debug( Tag, "AppBase::State not in proper state: Running." );
        return false;
    }

    Assets::AssetRegistry::destroy();
    ServiceProvider::destroy();

    if( m_platformInterface ) {
        Platform::PlatformInterface::destroy();
        m_platformInterface = nullptr;
    }

    delete m_world;
    m_world = nullptr;

    osre_debug( Tag, "Set application state to destroyed." );
    m_state = State::Destroyed;
    Logger::kill();

    return true;
}

void AppBase::onUpdate( d32 timetick ) {
    if ( nullptr != m_world ) {
        m_world->update( m_rbService );
    }

    if ( nullptr != m_uiScreen ) {
        m_uiScreen->render( m_rbService );
    }
}

const ArgumentParser &AppBase::getArgumentParser() const {
    return m_argParser;
}

} // Namespace App
} // Namespace OSRE
