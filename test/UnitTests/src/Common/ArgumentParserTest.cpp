/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "osre_testcommon.h"

#include "Common/ArgumentParser.h"

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::Common;

class ArgumentParserTest : public ::testing::Test {
    // empty
};

TEST_F( ArgumentParserTest, createTest ) {
    bool ok( true );
    try {
        i32 argc = 2;
        const c8 *ppArgv[] = { "testApp", "--api" };
        ArgumentParser testParser( argc, ppArgv, "api", "bla" );
    } catch( ... ) {
        ok = false;
    }
    EXPECT_TRUE( ok );
}

TEST_F( ArgumentParserTest, parseArgTest ) {
    static const String SupportedArgs = "help:api:gen_project:asset_path";
    static const String Descs = "Shows the help:The render API:Generates a template project:Path to media";

    i32 argc = 2;
    const c8 *ppArgv[] = { "testApp", "--api" };
    ArgumentParser testParser( argc, ppArgv, SupportedArgs, Descs );
    EXPECT_TRUE( testParser.hasValidArgs() );
}

TEST_F(ArgumentParserTest, getArgTest) {
    static const String SupportedArgs = "help:api:gen_project:asset_path";
    static const String Descs = "Shows the help:The render API:Generates a template project:Path to media";

    i32 argc = 3;
    const c8 *ppArgv[] = { "testApp", "--api", "opengl" };
    ArgumentParser testParser(argc, ppArgv, SupportedArgs, Descs);

    bool ok = testParser.hasArgument("api");
    EXPECT_TRUE(ok);

    String value = testParser.getArgument("api");
    EXPECT_EQ(value, "opengl");

    ok = testParser.hasArgument("apo");
    EXPECT_FALSE(ok);
    value = testParser.getArgument("apo");
    EXPECT_TRUE(value.empty());
}


}
}

