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

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This struct stores all surface related information.
//-------------------------------------------------------------------------------------------------
struct SurfaceProperties {
    ui32   m_x;                   ///< Upper left x coordinate.
    ui32   m_y;                   ///< Upper left y coordinate.
    ui32   m_width;               ///< Width of the surface.
    ui32   m_height;              ///< Height of the surface.
    ui32   m_colordepth;          ///< Color depth.
    ui32   m_depthbufferdepth;    ///< Z-Depth.
    ui32   m_stencildepth;        ///< Stencil depth.
    String m_title;               ///< Window title.
    bool   m_fullscreen;          ///< true for full screen.
    bool   m_resizable;           ///< true, if window wan be resized.
    bool   m_open;                ///< Window is open flag.
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This abstract class declares the interface for a render surface.
/// 
/// A render surface could be a window on a desktop system for instance or a rectangle on an 
/// embedded device.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractSurface {
public:
    enum SurfaceFlagType {
        SF_PropertiesClean = 0,
        SF_WinTitleDirty   = ( 1 << 1 )
    };

    /// @brief  The class constructor.
    /// @param  props       [in] The surface properties.
    explicit AbstractSurface( SurfaceProperties *props );

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

    virtual void setWindowsTitle( const String &title ) = 0;

protected:
    /// @brief  Callback to override on creation.
    virtual bool onCreate() = 0;
    
    /// @brief  Callback to override on destroying.
    virtual bool onDestroy() = 0;
    
    /// @brief  Callback to override on updates.
    virtual bool onUpdateProperies() = 0;

private:
    ui32 m_flags;
    SurfaceProperties *m_pProperties;
    bool m_isCreated;
};

} // Namespace Platform
} // Namespace OSRE
