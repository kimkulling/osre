/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
class OSRE_EXPORT AbstractRenderContext {
public:
    /// @brief  The class destructor, virtual.
    virtual ~AbstractRenderContext();

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
    AbstractRenderContext();

private:
    bool m_isActive;
    AbstractWindow *m_rootRenderSurface;
};

inline
AbstractRenderContext::AbstractRenderContext() 
: m_isActive( false )
, m_rootRenderSurface( nullptr ) {
    // empty
}

inline
AbstractRenderContext::~AbstractRenderContext() {
    // empty
}

inline
bool AbstractRenderContext::create( AbstractWindow *surface ) {
    m_rootRenderSurface = surface;
    return onCreate( surface );
}

inline
bool AbstractRenderContext::destroy() {
    return onDestroy();
}

inline
bool AbstractRenderContext::activate() {
    m_isActive = onActivate();
    return m_isActive;
}

inline
bool AbstractRenderContext::isActive() const {
    return m_isActive;
}

inline
bool AbstractRenderContext::update() {
    return onUpdate();
}

inline
AbstractWindow *AbstractRenderContext::getRenderSurface() const {
    return m_rootRenderSurface;
}

} // Namespace Platform
} // Namespace OSRE
