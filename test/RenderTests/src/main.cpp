/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Common/Logger.h"
#include "Properties/Settings.h"
#include "Platform/PlatformInterface.h"
#include "Platform/AbstractTimer.h"
#include "Common/TObjPtr.h"
#include "Common/ArgumentParser.h"
#include "App/AssetRegistry.h"

#include "RenderTestSuite.h"

#include <iostream>

using namespace ::OSRE;
using namespace ::OSRE::RenderTest;
using namespace ::OSRE::Platform;

int main( int argc, char *argv[] ) {
    Common::ArgumentParser argParser( argc, (const char**) argv, 
        "api:media:test", 
        "The render API:The media to load:A dedicated test" );
    String renderAPI( "opengl" );
    if ( argParser.hasArgument( "api" ) ) {
        renderAPI = argParser.getArgument( "api" ); 
    }

#ifdef _WIN32
    String mediaPath( "../../assets/" );
#else
    String mediaPath( "../assets/" );
#endif
    if ( argParser.hasArgument( "media" ) ) {
        mediaPath = argParser.getArgument( "media" );
    }
    String test = "";
    if (argParser.hasArgument("test")) {
        test = argParser.getArgument("test");
        std::cout << "Selected test: " << test << "\n";
    } else {
        std::cout << "Selected test: all\n";
    }

    RenderTestSuite::getInstance()->setMediaPath(mediaPath);

    RenderTestSuite *rtSuite = RenderTestSuite::create("tests");
    rtSuite->setup( renderAPI );
    RenderTestSuite::getInstance()->setRenderAPI(renderAPI);
    if (!test.empty()) {
        RenderTestSuite::getInstance()->setSelectedTest(test);
    }
    
    AbstractTimer *timer = rtSuite->getTimer();
    if (timer == nullptr) {
        std::cerr << "Error: No valid timer instance, nullptr detected.\n";
        return 1;
    }

    while (rtSuite->update()) {
        rtSuite->startTests();
    }

    RenderTestSuite::kill();

    MemoryStatistics::showStatistics();

    return 0;
}


