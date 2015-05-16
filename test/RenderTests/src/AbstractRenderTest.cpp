#include "AbstractRenderTest.h"

#include <osre/RenderBackend/RenderCommon.h>
#include <cassert>

namespace OSRE {
namespace RenderTest {

using namespace ::OSRE::RenderBackend;

//-------------------------------------------------------------------------------------------------
AbstractRenderTest::AbstractRenderTest( const String &renderTestName )
: m_RenderTestName( renderTestName )
, m_State( 0 ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
AbstractRenderTest::~AbstractRenderTest() {
    // empty
}

//---------------------------------------------------------------------------------------------
bool AbstractRenderTest::create( RenderBackendService *pRenderBackendSrv ) {
    assert( nullptr != pRenderBackendSrv );

    return onCreate( pRenderBackendSrv );
}

//---------------------------------------------------------------------------------------------
bool AbstractRenderTest::destroy( RenderBackendService *pRenderBackendSrv ) {
    assert( nullptr != pRenderBackendSrv );

    return onDestroy( pRenderBackendSrv );
}

//---------------------------------------------------------------------------------------------
bool AbstractRenderTest::render( d32 timediff, RenderBackendService *pRenderBackendSrv ) {
    assert( nullptr != pRenderBackendSrv );

    return onRender( timediff, pRenderBackendSrv );
}

//---------------------------------------------------------------------------------------------
bool AbstractRenderTest::onCreate( RenderBackendService *pRenderBackendSrv ) {
    return true;
}

//---------------------------------------------------------------------------------------------
bool AbstractRenderTest::onDestroy( RenderBackendService *pRenderBackendSrv ) {
    return true;
}

//---------------------------------------------------------------------------------------------
void AbstractRenderTest::setup( RenderBackendService *pRenderBackendSrv ) {
    // empty
}

//---------------------------------------------------------------------------------------------
void AbstractRenderTest::teardown( RenderBackendService *pRenderBackendSrv ) {
    // empty
}

//---------------------------------------------------------------------------------------------
bool AbstractRenderTest::updateSnaptShot() {
    if( !AbstractRenderTest::hasState( AbstractRenderTest::ScreenShotGeneated ) ) {
        String name = getTestName();
        IO::Uri screenshot( "file://./" + name + ".jpg" );
        IO::AbstractFileSystem *pFS = IO::IOService::getInstance()->getFileSystem( "file" );

        if( !pFS ) {
            AbstractRenderTest::setState( AbstractRenderTest::ScreenShotGeneated );
            return false;
        }

        IO::Stream *pFileStream = pFS->open( screenshot, IO::Stream::WriteAccess );
        if( pFileStream ) {
            //pRenderDevice->makeScreenshot( pFileStream );
            pFS->close( &pFileStream );
        }
        pFS->release();

        AbstractRenderTest::setState( AbstractRenderTest::ScreenShotGeneated );
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
const String &AbstractRenderTest::getTestName() const {
    return m_RenderTestName;
}

//-------------------------------------------------------------------------------------------------
void AbstractRenderTest::setState( TestState state ) {
    m_State |= state;
}

//-------------------------------------------------------------------------------------------------
bool AbstractRenderTest::hasState( TestState state ) const {
    if( m_State & state ) {
        return true;
    } else {
        return false;
    }
}

//-------------------------------------------------------------------------------------------------
RenderBackend::Material *AbstractRenderTest::createMaterial( const String &VsSrc, 
                                                             const String &FsSrc ) {
    Material *mat      = new Material;
    mat->m_numTextures = 0;
    mat->m_type        = ShaderMaterial;
    mat->m_pShader     = new Shader;
    mat->m_pShader->m_src[ SH_VertexShaderType ] = VsSrc;
    mat->m_pShader->m_src[ SH_FragmentShaderType ] = FsSrc;

    return mat;
}

//-------------------------------------------------------------------------------------------------

} // Namespace RenderTest
} // Namespace OSRE
