/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/Logger.h>
#include <osre/Properties/Settings.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractTimer.h>
#include <osre/Common/TObjPtr.h>
#include <osre/Common/ArgumentParser.h>
#include <osre/Assets/AssetRegistry.h>

#include "RenderTestSuite.h"

#include <iostream>

using namespace ::OSRE;
using namespace ::OSRE::RenderTest;
using namespace ::OSRE::Platform;

int main( int argc, char *argv[] ) {
    Common::ArgumentParser argParser( argc, argv, "api:media", "The render API:The media to load" );
    String renderAPI( "opengl" );
    //String renderAPI( "vulkan" );
    if ( argParser.hasArgument( "api" ) ) {
        renderAPI = argParser.getArgument( "api" ); 
    }

#ifdef _WIN32
    String mediaPath( "../../media/" );
#else
    String mediaPath( "../media/" );
#endif
    if ( argParser.hasArgument( "media" ) ) {
        mediaPath = argParser.getArgument( "media" );
    }
    RenderTestSuite::getInstance()->setMediaPath( mediaPath );

    RenderTestSuite *rtSuite = RenderTestSuite::create( "tests" );
    rtSuite->setup( renderAPI );
    RenderTestSuite::getInstance()->setRenderAPI( renderAPI );
    
    AbstractTimer *timer( rtSuite->getTimer() );
    if( nullptr == timer ) {
        return 1;
    }

    while( rtSuite->update() ) {
        rtSuite->startTests();
    }

    RenderTestSuite::kill();

    return 0;
}
