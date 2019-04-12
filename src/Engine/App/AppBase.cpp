/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/TObjPtr.h>
#include <osre/IO/IOService.h>
#include <osre/Properties/Settings.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractTimer.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/AbstractPlatformEventQueue.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/Pipeline.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/View.h>
#include <osre/Scene/World.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/UI/Screen.h>
#include <osre/UI/UiItemFactory.h>
#include <osre/UI/UiRenderer.h>

// private includes
#include "src/Engine/Platform/PlatformPluginFactory.h"

namespace OSRE {
namespace App {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::UI;
using namespace ::OSRE::IO;

const String API_Arg = "api";
const String Tag     = "AppBase";

class MouseEventListener : public Platform::OSEventListener {
public:
    MouseEventListener()
    : OSEventListener( "App/MouseEventListener" )
    , m_uiScreen() {
        // empty
    }

    ~MouseEventListener() {
        // empty
    }

    void setScreen( UI::Screen *screen ) {
        m_uiScreen = screen;
    }

    UI::Screen *getScreen() const {
        return m_uiScreen.getPtr();
    }

    void onOSEvent( const Event &osEvent, const EventData *data ) override {
        if (!m_uiScreen.isValid()) {
            return;
        }

        osre_debug( Tag, "listener called" );
        if ( osEvent == MouseButtonDownEvent ) {
            MouseButtonEventData *mouseBtnData( ( MouseButtonEventData* )data );
            const Point2ui pt( mouseBtnData->m_AbsX, mouseBtnData->m_AbsY );
            m_uiScreen->mouseDown( pt, nullptr );
        }
    }

private:
    Common::TObjPtr<UI::Screen> m_uiScreen;
};

AppBase::AppBase( i32 argc, c8 *argv[], const String &supportedArgs, const String &desc )
: m_state( State::Uninited )
, m_argParser( argc, argv, supportedArgs, desc )
, m_settings( nullptr )
, m_platformInterface( nullptr )
, m_timer( nullptr )
, m_rbService( nullptr )
, m_world( nullptr )
, m_uiScreen( nullptr )
, m_uiRenderer( nullptr )
, m_mouseEvListener( nullptr )
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

bool AppBase::initWindow( ui32 x, ui32 y, ui32 width, ui32 height, const String &title, bool fullscreen,
        RenderBackendType renderer ) {
    OSRE_ASSERT( nullptr != m_settings );

    m_settings->setInt( Properties::Settings::WinX, x );
    m_settings->setInt( Properties::Settings::WinY, y );
    m_settings->setInt( Properties::Settings::WinWidth, width );
    m_settings->setInt( Properties::Settings::WinHeight, height );
    m_settings->setString( Properties::Settings::WindowsTitle, title );
    m_settings->setBool( Properties::Settings::FullScreen, fullscreen );

    if (renderer == RenderBackendType::OpenGLRenderBackend) {
        m_settings->setString( Properties::Settings::RenderAPI, "opengl" );
    } else if (renderer == RenderBackendType::DX11Backend) {
        m_settings->setString( Properties::Settings::RenderAPI, "dx11" );
    } else if (renderer == RenderBackendType::VulkanRenderBackend) {
        m_settings->setString( Properties::Settings::RenderAPI, "vulkan" );
    }

    return onCreate( m_settings );
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

    onUpdate();
}

void AppBase::requestNextFrame() {
    OSRE_ASSERT( nullptr != m_uiRenderer );
    OSRE_ASSERT( nullptr != m_rbService );

    if (nullptr == m_world) {
        return;
    }

    m_world->draw( m_rbService );
    if (nullptr != m_uiScreen) {
        m_uiRenderer->render( m_uiScreen, m_rbService );
    }
    m_rbService->update();
}

bool AppBase::handleEvents() {
    if( nullptr == m_platformInterface ) {
        osre_debug( Tag, "AppBase::PlatforInterface not in proper state: not nullptr." );
        return false;
    }

    bool retCode = m_platformInterface->update();

    if (shutdownRequested()) {
        return false;
    }
    
    return retCode;
}

Properties::Settings *AppBase::getSettings() const {
    return m_settings;
}

Scene::Stage *AppBase::createStage( const String &name ) {
    if ( nullptr == m_world ) {
        osre_debug( Tag, "No world to add state to." );
        return nullptr;
    }

    String stageName(name);
    if (stageName.empty()) {
        stageName = "new_stage";
    }

    Scene::Stage *stage( new Scene::Stage(stageName, m_rbService ) );
    if ( nullptr != stage ) {
        m_world->addStage( stage );
    }

    return stage;
}

bool AppBase::setActiveStage( Scene::Stage *stage ) {
    if ( nullptr == m_world ) {
        osre_debug( Tag, "No world to activate state to." );
        return false;
    }
    
    return m_world->setActiveStage( stage );
}

bool AppBase::setActiveView(Scene::View *view) {
    if (nullptr == m_world) {
        osre_debug(Tag, "No world to activate state to.");
        return false;
    }
    return m_world->setActiveView(view);
}

bool AppBase::activateStage( const String &name ) {
    if ( nullptr == m_world ) {
        osre_debug( Tag, "No world to activate state to." );
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

    UI::Screen *newScreen = ( Screen* ) UiItemFactory::getInstance()->create( WidgetType::Screen, name, nullptr );
    setUIScreen( newScreen );

    return newScreen;
}

void AppBase::setUIScreen( UI::Screen *uiScreen ) {
    if ( m_uiScreen != uiScreen ) {
        m_uiScreen = uiScreen;
        AbstractWindow *surface( m_platformInterface->getRootWindow() );
        if ( nullptr != surface ) {
            m_uiScreen->setSurface( surface );
            m_mouseEvListener->setScreen( m_uiScreen );
            m_rbService->setUiScreen( uiScreen );
        }
    }
}

Platform::AbstractTimer *AppBase::getActiveTimer() const {
    return m_timer;
}

RenderBackend::RenderBackendService *AppBase::getRenderBackendService() const {
    return m_rbService;
}

Platform::AbstractWindow *AppBase::getRootWindow() const {
    if ( nullptr == m_platformInterface ) {
        return nullptr;
    }

    return m_platformInterface->getRootWindow();
}

void AppBase::setWindowsTitle( const String &title ) {
    if ( nullptr == m_platformInterface ) {
        return;
    }

    AbstractWindow *rs = m_platformInterface->getRootWindow();
    if ( nullptr != rs ) {
        rs->setWindowsTitle( title );
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
	if (nullptr == m_platformInterface) {
		osre_error(Tag, "Pointer to platform interface is nullptr.");
		return false;
	}

    if( !m_platformInterface->open() ) {
		osre_error(Tag, "Error while opening platform interface.");
		return false;
    }

    // register any available platform-specific log streams
    Common::AbstractLogStream *stream = Platform::PlatformPluginFactory::createPlatformLogStream();
    if( nullptr != stream ) {
        Logger::getInstance()->registerLogStream( stream );
    }

    // create the render back-end
    m_rbService = new RenderBackend::RenderBackendService();
    m_rbService->setSettings(m_settings, false);
    if( !m_rbService->open() ) {
        m_rbService->release();
        m_rbService = nullptr;
        return false;
    }
    m_platformInterface->getPlatformEventHandler()->setRenderBackendService( m_rbService );
    
    // enable render-back-end
    if( m_platformInterface ) {
        RenderBackend::CreateRendererEventData *data = new RenderBackend::CreateRendererEventData( m_platformInterface->getRootWindow() );
        data->m_pipeline = createDefaultPipeline();
        m_rbService->sendEvent( &RenderBackend::OnCreateRendererEvent, data );
    }
    m_timer = Platform::PlatformInterface::getInstance()->getTimer();

    // create our world
    Scene::RenderMode mode = static_cast<Scene::RenderMode>( m_settings->get( Properties::Settings::RenderMode ).getInt() );
    m_world = new Scene::World( "world", mode );
    
    ServiceProvider::create( m_rbService );

    // Setup onMouse event-listener
    AbstractPlatformEventQueue *evHandler = m_platformInterface->getPlatformEventHandler();
    if ( nullptr != evHandler ) {
        TArray<const Common::Event*> eventArray;
        eventArray.add( &MouseButtonDownEvent );
        eventArray.add( &MouseButtonUpEvent );
        m_mouseEvListener = new MouseEventListener;
        evHandler->registerEventListener( eventArray, m_mouseEvListener );
    }

    IO::IOService::create();

    m_uiRenderer = new UI::UiRenderer;

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

    delete m_uiScreen;
    delete m_uiRenderer;

    delete m_world;
    m_world = nullptr;

    osre_debug( Tag, "Set application state to destroyed." );
    m_state = State::Destroyed;
    Logger::kill();

    return true;
}

static const i64 Conversion2Micro = 1000;

void AppBase::onUpdate() {
    i64 microsecs = m_timer->getMilliCurrentSeconds() * Conversion2Micro;
    Time dt( microsecs );
    if ( nullptr != m_world ) {
        m_world->update( dt );
    }

    if (nullptr != m_uiRenderer && nullptr != m_uiScreen) {
        m_uiRenderer->render(m_uiScreen, m_rbService);
    }
}

const ArgumentParser &AppBase::getArgumentParser() const {
    return m_argParser;
}

RenderBackend::Pipeline *AppBase::createDefaultPipeline() {
    Pipeline *pipeline = new Pipeline;
    PipelinePass *renderPass = new PipelinePass(RenderPassId, nullptr);
    CullState cullState( CullState::CullMode::CCW, CullState::CullFace::Front );
    renderPass->setCullState( cullState );
    pipeline->addPass(renderPass);
    
    return pipeline;
}

} // Namespace App
} // Namespace OSRE
