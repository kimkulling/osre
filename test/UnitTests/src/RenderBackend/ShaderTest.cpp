/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "RenderBackend/Shader.h"
#include "IO/Uri.h"

namespace OSRE {
namespace UnitTest {

using namespace ::OSRE::RenderBackend;

class ShaderTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

class TestShaderLoader : public ShaderLoader {
public:
    int mCalled;

    TestShaderLoader() :
            ShaderLoader(),
            mCalled(0) {
        // empty
    }

    ~TestShaderLoader() override {
        // empty
    }

    size_t load(const IO::Uri&, Shader*) override {
        mCalled++;
        return 0;
    }
    
    bool unload(Shader*) override {
        mCalled--;
        return true;
    }
};

TEST_F(ShaderTest, create_success) {
    IO::Uri location("file:\\temp");
    ShaderResource res("test", location);
    
    Shader *shader = res.getRes();
    EXPECT_EQ(nullptr, shader);
    ShaderLoader *loader = new TestShaderLoader();
    res.load(*loader);
    shader = res.getRes();
    EXPECT_NE(nullptr, shader);
}

} // namespace UnitTest
} // namespace OSRE