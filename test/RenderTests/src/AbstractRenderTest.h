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
#pragma once

#include <osre/IO/AbstractFileSystem.h>

namespace OSRE {

namespace RenderBackend {
    class RenderBackendService;
    struct Material;
}

namespace RenderTest {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::RenderTest::AbstractRenderTest
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
