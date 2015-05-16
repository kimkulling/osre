#pragma once

/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-2015, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions
and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
and the following disclaimer in the documentation and/or other materials provided with the
distribution.

* Neither the name of the ZFXCE2 team, nor the names of its contributors may be used to endorse or
promote products derived from this software without specific prior written permission of the
ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
#ifndef ZFXCE2_RT_RENDERTESTSUITE_H_INC
#define ZFXCE2_RT_RENDERTESTSUITE_H_INC

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

#endif // ZFXCE2_RT_RENDERTESTSUITE_H_INC
