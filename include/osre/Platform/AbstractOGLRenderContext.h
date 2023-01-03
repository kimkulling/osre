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

#include <osre/Platform/PlatformCommon.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This abstract class declares the interface for a render context. Override this for your 
/// own implementation.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractOGLRenderContext {
public:
    /// @brief  The class destructor, virtual.
    virtual ~AbstractOGLRenderContext() = default;

    /// @brief  Creates the context.
    /// @param  surface     [in] The render surface.
    /// @return true if creation was successful.
    bool create( AbstractWindow *surface );

    /// @brief  Destroys the context.
    /// @return true if creation was successful.
    bool destroy();

    /// @brief  Activates the context.
    /// @return true if creation was successful.
    bool activate();

    /// @brief  Will return true, id the render context is the active one.
    /// @return true, if the render context is active.
    bool isActive() const;

    /// @brief  Updates the context.
    /// @return true if creation was successful.
    bool update();

    /// @brief  Returns the root render surface.
    /// @return The root render surface.
    AbstractWindow *getRenderSurface() const;

protected:
    /// @brief The callbacks.
    virtual bool onCreate( AbstractWindow *pSurface ) = 0;
    virtual bool onDestroy() = 0;
    virtual bool onUpdate() = 0;
    virtual bool onActivate() = 0;

protected:
    /// @brief  The default class constructor.
    AbstractOGLRenderContext();

private:
    bool mIsActive;
    AbstractWindow *mRootRenderSurface;
};

inline AbstractOGLRenderContext::AbstractOGLRenderContext() : mIsActive( false ), mRootRenderSurface( nullptr ) {}

inline bool AbstractOGLRenderContext::create( AbstractWindow *surface ) {
    mRootRenderSurface = surface;
    return onCreate( surface );
}

inline bool AbstractOGLRenderContext::destroy() {
    return onDestroy();
}

inline bool AbstractOGLRenderContext::activate() {
    mIsActive = onActivate();
    return mIsActive;
}

inline bool AbstractOGLRenderContext::isActive() const {
    return mIsActive;
}

inline bool AbstractOGLRenderContext::update() {
    return onUpdate();
}

inline AbstractWindow *AbstractOGLRenderContext::getRenderSurface() const {
    return mRootRenderSurface;
}

} // namespace Platform
} // namespace OSRE
