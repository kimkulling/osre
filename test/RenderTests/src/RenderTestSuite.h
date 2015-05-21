#pragma once

#include "AbstractTestFixure.h"

#include <cppcore/Container/TArray.h>
#include <osre/Common/Types.h>

namespace OSRE {

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
///	@class		::ZFXCE2::RenderTest::RenderTestSuite
///	@ingroup	RenderTest
///
///	@brief 
//-------------------------------------------------------------------------------------------------
class RenderTestSuite : public AbstractTestFixture {
public:
    static RenderTestSuite *create(const String &suiteName, const String &renderAPI);
    static RenderTestSuite *getInstance();
    static void kill();
    virtual bool setup();
    virtual bool teardown();
    void attachRenderTest(AbstractRenderTest *pRenderTest);
    ui32 getNumRenderTests() const;
    void startTests();
    void showTestReport();
    void setRenderAPI( const String &renderAPI );
    const String &getRenderAPI() const;
    void setMediaPath( const String &mediaPath );
    const String &getMediaPath() const;
    Platform::AbstractTimer *getTimer( ) const;
    bool update( d32 timediff );
    bool clearTestEnv();
    bool requestNextTest( ui32 &next );

protected:
    void addFailureLog(const String &logEntry);

private:
    RenderTestSuite();
    RenderTestSuite(const String &suiteName, const String &renderAPI);
    RenderTestSuite &operator = ( const RenderTestSuite & );
    virtual ~RenderTestSuite();

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

//-------------------------------------------------------------------------------------------------

} // Namespace RenderTest
} // Namespace OSRE
