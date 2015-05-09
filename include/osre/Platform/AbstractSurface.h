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
