/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
///	@brief  This struct stores all information regarding any resolution.
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT Resolution {
    enum ResRequest {
        Res640x480,
        Res800x600,
        Res1024x768,
        Res1280x720,
        Res1176x664,
        Res1768x992,
        Res1920x1080
    };

    ui32 m_width;   ///< The resolution in x.
    ui32 m_height;  ///< The resolution in y.

    explicit Resolution( ResRequest req );
    Resolution();
    ~Resolution();
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This struct stores all surface related information.
//-------------------------------------------------------------------------------------------------
struct WindowsProperties {
    ui32   m_x;                   ///< Upper left x coordinate.
    ui32   m_y;                   ///< Upper left y coordinate.
    ui32   m_width;               ///< Width of the surface.
    ui32   m_height;              ///< Height of the surface.
    uc8    m_colordepth;          ///< Color depth.
    uc8    m_depthbufferdepth;    ///< Z-Depth.
    uc8    m_stencildepth;        ///< Stencil depth.
    String m_title;               ///< Window title.
    bool   m_fullscreen;          ///< true for full screen.
    bool   m_resizable;           ///< true, if window wan be resized.
    bool   m_childWindow;         ///< true, if the window is a child window, for embedding
    bool   m_open;                ///< Window is open flag.

    /// Will return the dimension as a rectangle.
    void getDimension(Rect2ui &rect);
};

inline
void WindowsProperties::getDimension(Rect2ui &rect) {
    rect.set(m_x, m_y, m_width, m_height);
}

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This abstract class declares the interface for a render surface.
/// 
/// A render surface could be a window on a desktop system for instance or a rectangle on an 
/// embedded device.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractWindow {
public:
    /// @brief  The Windows flags.
    enum SurfaceFlagType {
        SF_PropertiesClean = 0,
        SF_WinTitleDirty   = ( 1 << 1 ),
        SF_WinResize       = ( 1 << 2 ),
        SF_ChildWindow     = ( 1 << 3 )
    };

    /// @brief  The class constructor.
    /// @param  props       [in] The surface properties.
    explicit AbstractWindow( WindowsProperties *props );

    /// @brief  The class destructor, virtual.
    virtual ~AbstractWindow();

    /// @brief  Will create the surface.
    /// @return true will be returned, when creation was successful.
    virtual bool create();

    /// @brief  Will destroy the surface.
    /// @return true will be returned, when creation was successful.
    virtual bool destroy();

    /// @brief  Call this to signal an updated property.
    /// @return true will be returned, when creation was successful.
    virtual bool updateProperties();

    /// @brief  Setter to update all properties. updateProperties must be called to 
    ///         apply changed properties.
    /// @param  props       [in] The new properties.
    virtual void setProperties( WindowsProperties *props );

    /// @brief  Returns a non-const pointer to the window properties. setFlags must 
    ///         be called manually followed by updateProperties to apply changes.
    /// @return Pointer showing to the surface properties.
    virtual WindowsProperties *getProperties();
    
    /// @brief  Setting of a new flag.
    /// @param  flags       [in] The new flags.
    virtual void setFlags( SurfaceFlagType flags );

    /// @brief  Returns the surface flags.
    /// @return The surface flags.
    virtual ui32 getFlags() const;

    /// @brief  Will set the windows title.
    /// @param  title       [in] The new windows title.
    virtual void setWindowsTitle( const String &title ) = 0;

    /// @
    virtual void resize( ui32 x, ui32 y, ui32 w, ui32 h );

    virtual void getWindowsRect(Rect2ui &rect) const;

protected:
    /// @brief  Callback to override on creation.
    virtual bool onCreate() = 0;
    /// @brief  Callback to override on destroying.
    virtual bool onDestroy() = 0;
    /// @brief  Callback to override on updates.
    virtual bool onUpdateProperies() = 0;
    /// @brief  The onResize callback.
    virtual void onResize( ui32 x, ui32 y, ui32 w, ui32 h ) = 0;

private:
    ui32 m_flags;
    WindowsProperties *m_properties;
    bool m_isCreated;
};

} // Namespace Platform
} // Namespace OSRE
