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

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace Platform {

// Forward declarations
class AbstractSurface;

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
    bool create( AbstractSurface *surface );

    /// @brief  Destroys the context.
    /// @return true if creation was successful.
    bool destroy();

    /// @brief  Activates the context.
    /// @return true if creation was successful.
    bool activate();

    /// @brief  Updates the context.
    /// @return true if creation was successful.
    bool update();

protected:
    virtual bool onCreate( AbstractSurface *pSurface ) = 0;
    virtual bool onDestroy() = 0;
    virtual bool onUpdate() = 0;
    virtual bool onActivate() = 0;

protected:
    /// @brief  The default class constructor.
    AbstractRenderContext();
};

inline
AbstractRenderContext::AbstractRenderContext( ) {
    // empty
}

inline
AbstractRenderContext::~AbstractRenderContext( ) {
    // empty
}

inline
bool AbstractRenderContext::create( AbstractSurface *surface ) {
    return onCreate( surface );
}

inline
bool AbstractRenderContext::destroy() {
    return onDestroy();
}

inline
bool AbstractRenderContext::activate() {
    return onActivate();
}

inline
bool AbstractRenderContext::update() {
    return onUpdate();
}

} // Namespace Platform
} // Namespace OSRE
