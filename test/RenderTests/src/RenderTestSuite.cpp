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
#include "RenderTestSuite.h"
#include "AbstractRenderTest.h"

#include <osre/Properties/Settings.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractPlatformEventHandler.h>
#include <osre/Platform/AbstractTimer.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/IO/IOService.h>

#include <iostream>
#include <cassert>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Platform;
using namespace ::CPPCore;

// static member initialization
RenderTestSuite *RenderTestSuite::s_pInstance = nullptr;
static const     String Tag                   = "RenderTestSuite";
static const     ui32 AllTestsDone            = 999999;

// our base keyboard event listener, for switching to the next test case
class KeyboardEventListener : public Platform::OSEventListener {
public:
    KeyboardEventListener( RenderTestSuite *renderTestSuite )
    : OSEventListener( "rendertest/keyboardeventlistener" )
    , m_testSuite( renderTestSuite ) {
        OSRE_ASSERT( nullptr != renderTestSuite );
    }

    ~KeyboardEventListener() {
        // empty
    }

    void onOSEvent( const Event &osEvent, const EventData *data ) {
         if( osEvent == KeyboardButtonDownEvent ) {
            if( data ) {
                bool result( false );
                ui32 next( 0 );
                Key key = reinterpret_cast< const KeyboardButtonEventData* >( data )->m_Key;
                if( key == KEY_SPACE ) {
                    result = m_testSuite->requestNextTest( next );
                    if ( !result ) {
                        osre_info( Tag, "All tests done." );
                    }
                }
            }
        }
    }

private:
    RenderTestSuite *m_testSuite;
};

RenderTestSuite *RenderTestSuite::create(const String &suiteName ) {
    if (!s_pInstance) {
        s_pInstance = new RenderTestSuite( suiteName );
    }

    return s_pInstance;
}

RenderTestSuite *RenderTestSuite::getInstance() {
    if (!s_pInstance) {
        // in case of an invalid call use opengl backend as the fallback
        static_cast<void>( RenderTestSuite::create("tests" ) );
        osre_error( Tag, "RenderTestSuite::getInstance called before RenderTestSuite::create, using fallback backend" );
    }

    return s_pInstance;
}

bool RenderTestSuite::setup( const String &API ) {
    // get configuration parameter
    setRenderAPI( API );
    Properties::Settings *settings = new Properties::Settings;
    settings->setString( Properties::Settings::RenderAPI, m_renderAPI );
    settings->setBool( Properties::Settings::PollingMode, true );

#ifdef OSRE_WINDOWS
    //pConfig->setInt( Properties::ConfigurationMap::PlatformPlugin, static_cast<i32>( Platform::SDL2Plugin) );
    settings->setInt( Properties::Settings::PlatformPlugin, static_cast< i32 >( Platform::PluginType::WindowsPlugin ) );
#else
    settings->setInt( Properties::Settings::PlatformPlugin, static_cast<i32>( Platform::PluginType::SDL2Plugin) );
#endif

    // create the platform abstraction
    m_pPlatformInterface = Platform::PlatformInterface::create( settings );
    if( m_pPlatformInterface ) {
        if( !m_pPlatformInterface->open() ) {
            return false;
        }
    }

    IO::IOService *ioSrv = IO::IOService::create();
    if ( !ioSrv->open() ) {
        ioSrv->release();
        return false;
    }
    IO::IOService::setInstance( ioSrv );

    m_pRenderBackendServer = new RenderBackendService();
    m_pRenderBackendServer->setSettings( settings, false );
    if ( !m_pRenderBackendServer->open()) {
        m_pRenderBackendServer->release();
        m_pRenderBackendServer = nullptr;
    }

    if( m_pPlatformInterface ) {
        CreateRendererEventData *data = new CreateRendererEventData( m_pPlatformInterface->getRootSurface() );
        data->m_defaultFont = m_pPlatformInterface->getDefaultFontName();
        m_pRenderBackendServer->sendEvent( &OnCreateRendererEvent, data );
    }

    m_pTimer = PlatformInterface::getInstance()->getTimer();
    
    Assets::AssetRegistry *registry( Assets::AssetRegistry::create() );
    if ( nullptr!=registry ) {
#ifdef OSRE_WINDOWS
        registry->registerAssetPath( "assets", getMediaPath() );
#else
        registry->registerAssetPath( "assets", getMediaPath() );
#endif 
    }

    return true;
}

bool RenderTestSuite::teardown() {
    if( m_pRenderBackendServer ) {
        m_pRenderBackendServer->close();
        m_pRenderBackendServer->release();
        m_pRenderBackendServer = nullptr;
    }

    IO::IOService::getInstance()->close();
    IO::IOService::getInstance()->release();

    if ( m_pTimer ) {
        m_pTimer->release();
        m_pTimer = nullptr;
    }
    
    if( m_pPlatformInterface ) {
        PlatformInterface::destroy();
        m_pPlatformInterface = nullptr;
    }

    osre_info( Tag, " RenderTestSuite::teardown()" );

    return true;
}

void RenderTestSuite::kill() {
    OSRE_ASSERT( nullptr!=s_pInstance );
    if ( s_pInstance ) {
        s_pInstance->teardown();
        delete s_pInstance;
        s_pInstance = nullptr;
        Logger::kill();
    }
}

void RenderTestSuite::attachRenderTest( AbstractRenderTest *pRenderTest ) {
    OSRE_ASSERT( nullptr!=pRenderTest );

    m_attachedRenderTests.add( pRenderTest );
}

ui32 RenderTestSuite::getNumRenderTests() const {
    return m_attachedRenderTests.size();
}

void RenderTestSuite::startTests() {
    if (m_attachedRenderTests.isEmpty()) {
        return;
    }
}

void RenderTestSuite::showTestReport() {
    if (m_FailureLog.isEmpty()) {
        return;
    }

    std::cout << "Errors:" << std::endl << "=======" << std::endl;
    for ( TArray<String>::Iterator it = m_FailureLog.begin(); it != m_FailureLog.end(); ++it) {
        String msg(*it);
        std::cout << msg << std::endl;
    }
}

void RenderTestSuite::setRenderAPI(const String &renderAPI) {
    m_renderAPI = renderAPI;
}

const String &RenderTestSuite::getRenderAPI() const {
    return m_renderAPI;
}

void RenderTestSuite::setMediaPath( const String &mediaPath ) {
    m_mediaPath = mediaPath;
}

const String &RenderTestSuite::getMediaPath() const {
    return m_mediaPath;
}

AbstractTimer *RenderTestSuite::getTimer() const {
    return m_pTimer;
}

bool RenderTestSuite::update( d32 timediff ) {
    if( !m_pPlatformInterface ) {
        return false;
    }

    TArray<const Common::Event*> eventArray;
    eventArray.add( &KeyboardButtonDownEvent );    
    m_pListener = new KeyboardEventListener( this );
    AbstractPlatformEventHandler *pEvHandler = m_pPlatformInterface->getPlatformEventHandler( );
    pEvHandler->registerEventListener( eventArray, m_pListener );

    while( m_pPlatformInterface->update( timediff ) ) {
        if( m_activeTestIdx == AllTestsDone ) {
            break;
        }

        if( m_pActiveRenderTest != m_attachedRenderTests[ m_activeTestIdx ] ) {
            if( m_pActiveRenderTest ) {
                m_pActiveRenderTest->destroy( m_pRenderBackendServer );
                clearTestEnv();
            }
            
            m_pActiveRenderTest = m_attachedRenderTests[ m_activeTestIdx ];
            if( m_pActiveRenderTest ) {
                m_pActiveRenderTest->create( m_pRenderBackendServer );
            }
        }
        
        if( m_pActiveRenderTest ) {
            m_pActiveRenderTest->setup( m_pRenderBackendServer );
            if( !m_pActiveRenderTest->render( timediff, m_pRenderBackendServer ) ) {
                addFailureLog( "Error : Cannot render test " + m_pActiveRenderTest->getTestName( ) + "\n" );
            }
            m_pRenderBackendServer->update( timediff );

            m_pActiveRenderTest->teardown( m_pRenderBackendServer );
        }
    }
    if( m_pActiveRenderTest ) {
        m_pActiveRenderTest->destroy( m_pRenderBackendServer );
        clearTestEnv();
    }

    pEvHandler->unregisterEventListener( eventArray, m_pListener );

    return false;
}

bool RenderTestSuite::clearTestEnv() {
    m_pRenderBackendServer->sendEvent( &OnClearSceneEvent, nullptr );

    return true;
}

bool RenderTestSuite::requestNextTest( ui32 &next ) {
    ++m_activeTestIdx;
    if( m_activeTestIdx >= m_attachedRenderTests.size() ) {
        m_activeTestIdx = AllTestsDone;
        next = AllTestsDone;
        return false;
    }
    next = m_activeTestIdx;

    return true;
}

void RenderTestSuite::addFailureLog( const String &logEntry ) {
    if (!logEntry.empty()) {
        m_FailureLog.add(logEntry);
    }
}

RenderTestSuite::RenderTestSuite( const String &suiteName )
: AbstractTestFixture( suiteName )
, m_pActiveRenderTest( nullptr )
, m_activeTestIdx( 0 )
, m_attachedRenderTests()
, m_FailureLog()
, m_pPlatformInterface( nullptr )
, m_pListener( nullptr )
, m_pTimer( nullptr )
, m_pRenderBackendServer( nullptr )
, m_renderAPI( "none" )
, m_mediaPath() {
    OSRE_ASSERT( !suiteName.empty() );
}

RenderTestSuite::~RenderTestSuite() {
    delete m_pListener;
    m_pListener = nullptr;

    for( ui32 i = 0; i < m_attachedRenderTests.size(); i++ ) {
        delete m_attachedRenderTests[ i ];
    }
}

} // Namespace RenderTest
} // Namespace OSRE
 
