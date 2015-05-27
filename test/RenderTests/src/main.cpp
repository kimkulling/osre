#include <osre/Common/Logger.h>
#include <osre/Properties/ConfigurationMap.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractTimer.h>
#include <osre/Common/TObjPtr.h>
#include <osre/Common/ArgumentParser.h>

#include "RenderTestSuite.h"

using namespace ::OSRE;
using namespace ::OSRE::RenderTest;
using namespace ::OSRE::Platform;

int main( int argc, char *argv[] ) {
    Common::ArgumentParser argParser( argc, argv, "api:media", "The render API:The media to load" );
    String renderAPI( "opengl" );
    if ( argParser.hasArgument( "api" ) ) {
        renderAPI = argParser.getArgument( "api" ); 
    }

    String mediaPath( "../../Media/" );
    if( argParser.hasArgument( "media" ) ) {
        mediaPath = argParser.getArgument( "media" );
    }

    RenderTestSuite *rtSuite = RenderTestSuite::create( "tests", renderAPI );
    RenderTestSuite::getInstance()->setRenderAPI( renderAPI );
    RenderTestSuite::getInstance( )->setMediaPath( mediaPath );
    
    AbstractTimer *timer( rtSuite->getTimer() );
    if( nullptr == timer ) {
        return 1;
    }

    while( rtSuite->update( timer->getTimeDiff( ) ) ) {
        rtSuite->startTests();
    }

    RenderTestSuite::kill();

    return 0;
}
