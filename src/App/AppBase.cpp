#include <osre/App/AppBase.h>
#include <osre/Common/ArgumentParser.h>
#include <osre/Properties/ConfigurationMap.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractTimer.h>
#include <osre/Threading/TaskScheduler.h>
#include <osre/RenderBackend/RenderBackendService.h>

// private includes
#include <src/Platform/PlatformPluginFactory.h>

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
    Properties::ConfigurationMap m_config;
    Platform::PlatformInterface *m_platformInterface;
    Platform::AbstractTimer *m_timer;
    RenderBackend::RenderBackendService *m_rbService;

    
    Impl( i32 argc, c8 *argv[], const String &supportedArgs )
    : m_state( Uninited )
    , m_timediff( 0.0 )
    , m_argParser( argc, argv, supportedArgs )
    , m_config()
    , m_platformInterface( nullptr )
    , m_timer( nullptr )
    , m_rbService( nullptr ) {
        m_config.setString( Properties::ConfigurationMap::RenderAPI, "opengl" );
        m_config.setBool( Properties::ConfigurationMap::PollingMode, true );

#ifdef OSRE_WINDOWS
        //pConfig->setInt( Properties::ConfigurationMap::PlatformPlugin, static_cast<i32>( Platform::SDL2Plugin) );
        m_config.setInt( Properties::ConfigurationMap::PlatformPlugin, static_cast< i32 >( Platform::WindowsPlugin ) );
#else
        m_config.setInt( Properties::ConfigurationMap::PlatformPlugin, static_cast< i32 >( Platform::SDL2Plugin ) );
#endif 
    }

    ~Impl(){
        // empty
    }
};

AppBase::AppBase( i32 argc, c8 *argv[], const String &supportedArgs )
: m_impl( new Impl( argc, argv, supportedArgs ) ) {
    // empty
}

AppBase::~AppBase() {
    delete m_impl;
    m_impl = nullptr;
}

bool AppBase::create() {
    return onCreate();
}

bool AppBase::destroy() {
    return onDestroy();
}

void AppBase::update() {
    m_impl->m_timediff = m_impl->m_timer->getTimeDiff();
}

void AppBase::requestNextFrame() {
    m_impl->m_rbService->update( m_impl->m_timediff );

}

bool AppBase::handleEvents() {
    if( nullptr == m_impl->m_platformInterface ) {
        osre_debug( Tag, "AppBase::PlatforInterface not in proper state: not nullptr." );
        return false;
    }

    return m_impl->m_platformInterface->update( m_impl->m_timediff );

}

bool AppBase::onCreate() {
    if( m_impl->m_state != Impl::Uninited ) {
        osre_debug( Tag, "AppBase::State not in proper state: Uninited." );
        return false;
    }

    // create the platform abstraction
    m_impl->m_platformInterface = Platform::PlatformInterface::create( &m_impl->m_config );
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

    // create task manager
    Threading::TaskScheduler::setInstance( Threading::TaskScheduler::create() );

    m_impl->m_rbService = new RenderBackend::RenderBackendService();
    if( !m_impl->m_rbService->open() ) {
        m_impl->m_rbService->release();
        m_impl->m_rbService = nullptr;
    }

    // enable render backend
    if( m_impl->m_platformInterface ) {
        RenderBackend::CreateRendererEventData *data = new RenderBackend::CreateRendererEventData( m_impl->m_platformInterface->getRootSurface() );
        m_impl->m_rbService->sendEvent( &RenderBackend::OnCreateRendererEvent, data );
    }

    // store timer instance
    m_impl->m_timer = Platform::PlatformInterface::getInstance()->getTimer();

    // set application state to Created
    m_impl->m_state = Impl::Created;
    
    return true;
}

bool AppBase::onDestroy() {
    if( m_impl->m_state != Impl::Running ) {
        return false;
    }

    if( m_impl->m_platformInterface ) {
        Platform::PlatformInterface::destroy();
        m_impl->m_platformInterface = nullptr;
    }

    m_impl->m_state = Impl::Destroyed;

    return true;
}

}
}
