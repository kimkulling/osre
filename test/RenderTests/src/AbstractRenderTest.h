#pragma once

#include <osre/IO/IOService.h>
#include <osre/IO/Stream.h>
#include <osre/IO/AbstractFileSystem.h>

namespace OSRE {

namespace RenderBackend {
    class RenderBackendService;

    struct Material;
}

namespace RenderTest {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderTest::AbstractRenderTest
///	@ingroup	RenderTest
///
///	@brief	The abstract base interface for a render test. Render tests shall be used to define a 
///	reference rendering behavior for the validation.
//-------------------------------------------------------------------------------------------------
class AbstractRenderTest {
public:
    /// @brief  This enum descripes the possible test states of the render test.
    enum TestState {
        ScreenShotGeneated = 0x01
    };

public:
    ///	@brief	Destructor, virtual.
    virtual ~AbstractRenderTest();

    bool create( RenderBackend::RenderBackendService *pRenderBackendSrv );
    bool destroy( RenderBackend::RenderBackendService *pRenderBackendSrv );
    bool render( d32 timediff, RenderBackend::RenderBackendService *pRenderBackendSrv );

    ///	@brief	Will create the render data.
    ///	@return	true if creation was successful, false if not.
    virtual void setup( RenderBackend::RenderBackendService *pRenderBackendSrv );

    ///	@brief	Will destroy the render data.
    ///	@return	true if destroying was successful, false if not.
    virtual void teardown( RenderBackend::RenderBackendService *pRenderBackendSrv );


    ///	@brief	Will update a snapshot of the rendered test result.
    ///	@return	true, if snapshot was generated.
    bool updateSnaptShot();

public:
    ///	@brief	Returns the name of the render test.
    ///	@return	The name of the render test.
    const String &getTestName() const;

    void setState( TestState state );

    bool hasState( TestState state ) const;

    RenderBackend::Material *createMaterial( const String &VsSrc, const String &FsSrc );

protected:
    AbstractRenderTest( const String &rRenderTestName );
    AbstractRenderTest &operator = ( const AbstractRenderTest &rOther );
    virtual bool onCreate( RenderBackend::RenderBackendService *pRenderBackendSrv );
    virtual bool onDestroy( RenderBackend::RenderBackendService *pRenderBackendSrv );
    virtual bool onRender( d32 timediff, RenderBackend::RenderBackendService *pRenderBackendSrv ) = 0;

private:
    const String m_RenderTestName;
    ui32 m_State;
};

//-------------------------------------------------------------------------------------------------

} // Namespace RenderTest
} // Namespace OSRE
