/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-214, ZFXCE2 Development Team
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
#pragma once

#include <osre/Platform/PluginType.h>

namespace OSRE {
namespace Platform {

class AbstractPlatformEventHandler;
class AbstractSurface;
class AbstractRenderContext;
class AbstractTimer;
class AbstractThreadFactory;

struct SurfaceProperties;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::PlatformPluginFactory
///	@ingroup	Infrastructure
///
///	@brief  This class is used to create all platform-specific instances.
//-------------------------------------------------------------------------------------------------
struct PlatformPluginFactory {
    /// @brief  Will init the factory.
    static bool init( PluginType type );

    /// @brief  Will release the factory.
    static bool release( PluginType type );

    /// @brief  Creates a platform-specific event handler instance.
    static AbstractPlatformEventHandler *createPlatformEventHandler( PluginType type, AbstractSurface *rootSurface );

    /// @brief  Creates a platform-specific surface instance.
    static AbstractSurface *createSurface( PluginType type, SurfaceProperties *pProps );

    /// @brief  Creates a platform-specific render context.
    static AbstractRenderContext *createRenderContext( PluginType type );

    /// @brief  Creates a platform-specific timer instance.
    static AbstractTimer *createTimer( PluginType type );

    /// @brief  Creates a platform-specific thread factory instance.
    static AbstractThreadFactory *createThreadFactory( PluginType type );
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
