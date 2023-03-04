/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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

// Forward declarations ---------------------------------------------------------------------------
namespace Platform {
    class AbstractWindow;
}

namespace RenderBackend {
    class RenderBackendService;
    class Material;

} // namespace RenderBackend

namespace RenderTest {

//-------------------------------------------------------------------------------------------------
///	@ingroup	RenderTest
///
///	@brief	The abstract base interface for a render test. 
/// Render tests shall be used to define reference rendering behavior for validation. Each test 
/// will follow this state machine:
/// Inited -> Created -> Rendering -> Destroyed
///  \          |          |            /
///   \         |          |           /
///    +--------+----------+----------+
///                   |
///                 Error
/// 
/// In the beginning the render-tests is in init state. All registered render-tests will be initialized 
/// after their creation. When the render-window is ready and the GPU-driver is initiated the test 
/// framework will loop though all render-tests. For each test the 
/// create-method shall try to generate all GPU-bound resources. If this was successful the 
/// render-loop will be started. By pressing a button the rendering will be stopped and the 
/// resources will be released by the destroy-method.
//-------------------------------------------------------------------------------------------------
class AbstractRenderTest {
public:
    ///	@brief	The class destructor, virtual.
    virtual ~AbstractRenderTest() = default;

    /// @brief  Will create the render-test. 
    /// 
    /// When creating your own render test place all creation code for any GPU-based resource here. 
    /// Examples could be resources like shaders, GPU-buffers or any other GPU-bounded resource.
    ///
    /// @param[in] rbSrv    The render backend to use.
    /// @return true if successful, false in case of an error. Typical error will be shader compilation
    ///         errors or a not available GPU-feature.
    bool create(RenderBackend::RenderBackendService *rbSrv);

    /// @brief  Will destroy the rendertest.
    /// @param[in] rbSrv    The render backend to use.
    /// @return true if successful.
    bool destroy(RenderBackend::RenderBackendService *rbSrv);

    /// @brief  Will render the rendertest.
    /// @param[in] rbSrv    The render backend to use.
    /// @return true if successful.
    bool render(RenderBackend::RenderBackendService *rbSrv);

    ///	@brief	Will create the render data.
    /// @param[in] rbSrv    The render backend to use.
    ///	@return	true if creation was successful, false if not.
    virtual void setup(RenderBackend::RenderBackendService *rbSrv);

    ///	@brief	Will destroy the render data.
    /// @param[in] rbSrv    The render backend to use.
    ///	@return	true if destroying was successful, false if not.
    virtual void teardown(RenderBackend::RenderBackendService *rbSrv);

    ///	@brief	Returns the name of the render test.
    ///	@return	The name of the render test.
    const String &getTestName() const;

    /// @brief  Creates a new material based on vertex- and fragment-shader.
    /// @param[in] matName  The material name.
    /// @param[in] VsSrc    The vertex-shader-code.
    /// @param[in] FsSrc    The fragment-shader-code.
    /// @return The new created material.
    RenderBackend::Material *createMaterial(const String &matName, const String &VsSrc, const String &FsSrc);

    /// @brief  Will return the active window.
    /// @return A pointer sowing to the active window.
    Platform::AbstractWindow *getWindow() const;

protected:
    /// @brief  The class constructor.
    /// @param[in] renderTestName   The name for the render test.
    explicit AbstractRenderTest(const String &renderTestName);

    /// @brief  The onCreate-callback.
    /// @param[in] rbSrv    The render backend to use.
    /// @return true if successful.
    virtual bool onCreate(RenderBackend::RenderBackendService *rbSrv);

    /// @brief  The onDestroy-callback.
    /// @param[in] rbSrv    The render backend to use.
    /// @return true if successful.
    virtual bool onDestroy(RenderBackend::RenderBackendService *rbSrv);

    /// @brief  The onRender-callback.
    /// @param[in] rbSrv    The render backend to use.
    /// @return true if successful.
    virtual bool onRender(RenderBackend::RenderBackendService *rbSrv);

    // Avoid copying
    AbstractRenderTest() = delete;
    AbstractRenderTest(const AbstractRenderTest &) = delete;
    AbstractRenderTest &operator=(const AbstractRenderTest &) = delete;

private:
    const String mRenderTestName;
    Platform::AbstractWindow *mWindow;
};

} // namespace RenderTest
} // namespace OSRE
