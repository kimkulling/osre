#include <osre/App/AppBase.h>
#include <osre/Common/ArgumentParser.h>
#include <osre/Properties/Settings.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractTimer.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Scene/Stage.h>

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
    Properties::Settings *m_settings;
    Platform::PlatformInterface *m_platformInterface;
    Platform::AbstractTimer *m_timer;
    RenderBackend::RenderBackendService *m_rbService;
    Scene::Stage *m_stage;
    
    Impl( i32 argc, c8 *argv[], const String &supportedArgs, const String &desc )
    : m_state( Uninited )
    , m_timediff( 0.0 )
    , m_argParser( argc, argv, supportedArgs, desc )
    , m_settings( nullptr )
    , m_platformInterface( nullptr )
    , m_timer( nullptr )
    , m_rbService( nullptr )
    , m_stage( nullptr ) {
        m_settings = new Properties::Settings;
        m_settings->setString( Properties::Settings::RenderAPI, "opengl" );
        m_settings->setBool( Properties::Settings::PollingMode, true );

#ifdef OSRE_WINDOWS
        //pConfig->setInt( Properties::ConfigurationMap::PlatformPlugin, static_cast<i32>( Platform::SDL2Plugin) );
        m_settings->setInt( Properties::Settings::PlatformPlugin, static_cast< i32 >( Platform::WindowsPlugin ) );
#else
        m_settings->setInt( Properties::Settings::PlatformPlugin, static_cast< i32 >( Platform::SDL2Plugin ) );
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
    if( m_impl->m_state == Impl::Created ) {
        m_impl->m_state = Impl::Running;
        osre_debug(Tag, "Set application state to running." );
    }

    m_impl->m_timediff = m_impl->m_timer->getTimeDiff();

    onUpdate();
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

Properties::Settings *AppBase::getSettings() const {
    if( nullptr == m_impl ) {
        return nullptr;
    }

    return m_impl->m_settings;
}

Scene::Stage *AppBase::createStage( const String &name ) {
    if( name.empty() ) {
        return nullptr;
    }

    m_impl->m_stage = new Scene::Stage( "HelloWorld", m_impl->m_rbService );
    
    return m_impl->m_stage;
}

bool AppBase::onCreate( Properties::Settings *config ) {
    if( m_impl->m_state != Impl::Uninited ) {
        osre_debug( Tag, "AppBase::State not in proper state: Uninited." );
        return false;
    }

    // create the platform abstraction
    if( nullptr != config ) {
        delete m_impl->m_settings;
        m_impl->m_settings = config;
    }

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

    // store timer instance
    m_impl->m_timer = Platform::PlatformInterface::getInstance()->getTimer();

    // set application state to Created
    osre_debug( Tag, "Set application state to Created." );
    m_impl->m_state = Impl::Created;
    
    return true;
}

bool AppBase::onDestroy() {
    if( m_impl->m_state != Impl::Running ) {
        osre_debug( Tag, "AppBase::State not in proper state: Running." );
        return false;
    }

    if( m_impl->m_platformInterface ) {
        Platform::PlatformInterface::destroy();
        m_impl->m_platformInterface = nullptr;
    }


    osre_debug( Tag, "Set application state to destroyed." );
    m_impl->m_state = Impl::Destroyed;
    Logger::kill();

    return true;
}

void AppBase::onUpdate() {

}

}
}
