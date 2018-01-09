/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#pragma once

#include "AbstractTestFixure.h"

#include <cppcore/Container/TArray.h>
#include <osre/Common/osre_common.h>

namespace OSRE {

// Forward declaration
namespace Platform {
    class AbstractTimer;
    class PlatformInterface;
}

namespace RenderBackend {
    class RenderBackendService;
}

namespace RenderTest {

class AbstractRenderTest;
class KeyboardEventListener;

//-------------------------------------------------------------------------------------------------
///	@ingroup	RenderTest
///
///	@brief 
//-------------------------------------------------------------------------------------------------
class RenderTestSuite : public AbstractTestFixture {
public:
    static RenderTestSuite *create(const String &suiteName );
    static RenderTestSuite *getInstance();
    static void kill();
    bool setup( const String &API ) final;
    bool teardown() final;
    void attachRenderTest(AbstractRenderTest *pRenderTest);
    ui32 getNumRenderTests() const;
    void startTests();
    void showTestReport();
    void setRenderAPI( const String &renderAPI );
    const String &getRenderAPI() const;
    void setMediaPath( const String &mediaPath );
    const String &getMediaPath() const;
    Platform::AbstractTimer *getTimer( ) const;
    bool update();
    bool clearTestEnv();
    bool requestNextTest( ui32 &next );
    RenderTestSuite() = delete;
    RenderTestSuite &operator = (const RenderTestSuite &) = delete;
    virtual ~RenderTestSuite() final;

protected:
    void addFailureLog(const String &logEntry);

private:
    RenderTestSuite( const String &suiteName );

private:
    static RenderTestSuite *s_pInstance;
    AbstractRenderTest *m_pActiveRenderTest;
    ui32 m_activeTestIdx;
    CPPCore::TArray<AbstractRenderTest*> m_attachedRenderTests;
    CPPCore::TArray<String> m_FailureLog;
    Platform::PlatformInterface *m_pPlatformInterface;
    KeyboardEventListener *m_pListener;
    Platform::AbstractTimer *m_pTimer;
    RenderBackend::RenderBackendService *m_pRenderBackendServer;
    String m_renderAPI;
    String m_mediaPath;
};

} // Namespace RenderTest
} // Namespace OSRE
