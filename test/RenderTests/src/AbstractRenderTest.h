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

#include <osre/IO/AbstractFileSystem.h>

namespace OSRE {

// Forward declarations
namespace RenderBackend {
    class RenderBackendService;
    struct Material;
}

namespace RenderTest {

//-------------------------------------------------------------------------------------------------
///	@ingroup	RenderTest
///
///	@brief	The abstract base interface for a render test. Render tests shall be used to define a 
///	reference rendering behavior for the validation.
//-------------------------------------------------------------------------------------------------
class AbstractRenderTest {
public:
    ///	@brief	Destructor, virtual.
    virtual ~AbstractRenderTest();

    /// @brief  Will create the rendertest.
    /// @param  rbSrv       [in] The render backend to use.
    /// @return true if successful.
    bool create( RenderBackend::RenderBackendService *rbSrv );
    
    /// @brief  Will destroy the rendertest.
    /// @param  rbSrv       [in] The render backend to use.
    /// @return true if successful.
    bool destroy( RenderBackend::RenderBackendService *rbSrv );
    
    /// @brief  Will render the rendertest.
    /// @param  timediff    [in] The timediff since the last frame.
    /// @param  rbSrv       [in] The render backend to use.
    /// @return true if successful.
    bool render( d32 timediff, RenderBackend::RenderBackendService *rbSrv );

    ///	@brief	Will create the render data.
    ///	@return	true if creation was successful, false if not.
    virtual void setup( RenderBackend::RenderBackendService *rbSrv );

    ///	@brief	Will destroy the render data.
    ///	@return	true if destroying was successful, false if not.
    virtual void teardown( RenderBackend::RenderBackendService *rbSrv );

public:
    ///	@brief	Returns the name of the render test.
    ///	@return	The name of the render test.
    const String &getTestName() const;

    /// @brief  Creates a new material based on vertex- and fragment-shader.
    /// @param  VsSrc       [in] The vertex-shader-code.
    /// @param  FsSrc       [in] The fragment-shader-code.
    /// @return The new created material.
    RenderBackend::Material *createMaterial( const String &VsSrc, const String &FsSrc );

protected:
    /// @brief  The class constructor.
    /// @param  renderTestName  [in] The name for the render test.
    AbstractRenderTest( const String &renderTestName );

    /// @brief  The onCreate-callback.
    /// @param  rbSrv   [in] The render backend to use.
    /// @return true if successful.
    virtual bool onCreate( RenderBackend::RenderBackendService *rbSrv );

    /// @brief  The onDestroy-callback.
    /// @param  rbSrv   [in] The render backend to use.
    /// @return true if successful.
    virtual bool onDestroy( RenderBackend::RenderBackendService *rbSrv );

    /// @brief  The onRender-callback.
    /// @param  timediff    [in] The timediff since the last frame.
    /// @param  rbSrv       [in] The render backend to use.
    /// @return true if successful.
    virtual bool onRender( d32 timediff, RenderBackend::RenderBackendService *rbSrv );

private:
    AbstractRenderTest &operator = ( const AbstractRenderTest & ) = delete;

private:
    const String m_renderTestName;
};

} // Namespace RenderTest
} // Namespace OSRE
