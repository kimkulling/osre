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

#include <osre/Common/Types.h>

namespace OSRE {
    namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@struct		::ZFXCE2::Platform::SurfaceProperties
///	@ingroup	Infrastructure
///
///	@brief  This struct stores all surface related information.
//-------------------------------------------------------------------------------------------------
struct SurfaceProperties {
    ui32 m_x;                   ///< Upper left x coordinate.
    ui32 m_y;                   ///< Upper left y coordinate.
    ui32 m_width;               ///< Width of the surface.
    ui32 m_height;              ///< Height of the surface.
    ui32 m_colordepth;          ///< Color depth.
    ui32 m_depthbufferdepth;    ///< Z-Depth.
    ui32 m_stencildepth;        ///< Stencil depth.
    String m_title;             ///< Window title.
    bool m_fullscreen;          ///< true for full screen.
    bool m_open;                ///< Window is open flag.
};

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::AbstractSurface
///	@ingroup	Infrastructure
///
///	@brief  This abstract class declares the interface for a render surface.
//-------------------------------------------------------------------------------------------------
class AbstractSurface {
public:
    enum SurfaceFlagType {
        SF_PropertiesClean = 0,
        SF_WinTitleDirty   = ( 1 << 1 )
    };

public:
    /// @brief  The class constructor.
    /// @param  props       [in] The surface properties.
    AbstractSurface( SurfaceProperties *props );

    /// @brief  The class destructor, virtual.
    virtual ~AbstractSurface();

    /// @brief  Will create the surface.
    /// @return true will be returned, when creation was successful.
    bool create();

    /// @brief  Will destroy the surface.
    /// @return true will be returned, when creation was successful.
    bool destroy();

    /// @brief  Call this to signal an updated property.
    /// @return true will be returned, when creation was successful.
    bool updateProperties();

    /// @brief  Setter to update all properties. updateProperties must be called to 
    ///         apply changed properties.
    /// @param  props       [in] The new properties.
    void setProperties( SurfaceProperties *props );

    /// @brief  Returns a non-const pointer to the window properties. setFlags must 
    ///         be called manually followed by updateProperties to apply changes.
    /// @return Pointer showing to the surface properties.
    SurfaceProperties *getProperties();
    
    /// @brief  Setting of a new flag.
    /// @param  flags       [in] The new flags.
    void setFlags( SurfaceFlagType flags );

    /// @brief  Returns the surface flags.
    /// @return The surface flags.
    ui32 getFlags() const;

protected:
    /// @brief  Callback to override on creation.
    virtual bool onCreate() = 0;
    
    /// @brief  Callback to override on destroying.
    virtual bool onDestroy() = 0;
    
    /// @brief  Callback to override on updating.
    virtual bool onUpdateProperies() = 0;

private:
    ui32 m_flags;
    SurfaceProperties *m_pProperties;
    bool m_isCreated;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
