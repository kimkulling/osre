/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "Common/osre_common.h"

namespace OSRE {
namespace Platform {

class AbstractPlatformEventQueue;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This struct stores all information regarding any resolution.
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT Resolution {
    enum class ResRequest {
        Res640x480,
        Res800x600,
        Res1024x768,
        Res1280x720,
        Res1176x664,
        Res1768x992,
        Res1920x1080
    };

    ui32 m_width; ///< The resolution in x.
    ui32 m_height; ///< The resolution in y.

    explicit Resolution(ResRequest req);
    Resolution();
    ~Resolution() = default;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This struct stores all surface related information.
//-------------------------------------------------------------------------------------------------
struct WindowsProperties {
    Rect2ui mRect;          ///< The window rectangle.
    uc8 m_colordepth;       ///< Color depth.
    uc8 m_depthbufferdepth; ///< Z-Depth.
    uc8 m_stencildepth;     ///< Stencil depth.
    String m_title;         ///< Window title.
    bool m_fullscreen;      ///< true for full screen.
    bool m_resizable;       ///< true, if window wan be resized.
    bool m_childWindow;     ///< true, if the window is a child window, for embedding
    bool m_open;            ///< Window is open flag.

    /// @brief Will return the dimension as a rectangle.
    /// @param[out] rect  The window rect.
    void getDimension(Rect2ui &rect);

    /// @brief WIll set the new rectangle geometry.
    /// @param[in] rect   The new rectangle.
    void setRect(const Rect2ui &rect);
};

inline void WindowsProperties::getDimension(Rect2ui &rect) {
    rect = mRect;
}

inline void WindowsProperties::setRect(const Rect2ui& rect) {
    mRect = rect;
}

/// @brief This enum is used to describe the mouse cursor.
enum class DefaultMouseCursorType {
    Invali = -1,
    ComonCursor,
    SelectCursor,
    WaitCursor,
    Count
};

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
    enum class SurfaceFlagType {
        Invalid,                        ///< Marks an invalid enum
        SF_PropertiesClean = 0,         ///<
        SF_WinTitleDirty = (1 << 1),    ///<
        SF_WinResize = (1 << 2),        ///<
        SF_ChildWindow = (1 << 3)       ///<
    };

    /// @brief  Will describe the show-state for the window.
    enum class ShowState {
        Invalid = -1,                   ///< Marks an invalid enum
        Visible,                        ///< Window is visible.
        Hidden,                         ///< Window is hidden.
        Count                           ///< Number of enums
    };

    /// @brief  The class constructor.
    /// @param  props       [in] The surface properties.
    explicit AbstractWindow(WindowsProperties *props, AbstractWindow *parent = nullptr);

    /// @brief  The class destructor, virtual.
    virtual ~AbstractWindow();

    virtual void setParent(AbstractWindow *parent);
    virtual AbstractWindow *getParent() const;
    /// @brief  Will create the surface.
    /// @return true will be returned, when creation was successful.
    virtual bool create();

    /// @brief  Will return true, if the window is already created.
    /// @return The creation state.
    virtual bool isCeated() const;

    /// @brief  Will destroy the surface.
    /// @return true will be returned, when creation was successful.
    virtual bool destroy();

    /// @brief  Call this to signal an updated property.
    /// @return true will be returned, when creation was successful.
    virtual bool updateProperties();

    /// @brief  Setter to update all properties. updateProperties must be called to
    ///         apply changed properties.
    /// @param  props       [in] The new properties.
    virtual void setProperties(WindowsProperties *props);

    /// @brief  Returns a non-const pointer to the window properties. setFlags must
    ///         be called manually followed by updateProperties to apply changes.
    /// @return Pointer showing to the surface properties.
    virtual WindowsProperties *getProperties();

    virtual void setEventQueue(AbstractPlatformEventQueue *queue);
    virtual AbstractPlatformEventQueue *getEventQueue() const;

    /// @brief  Setting of a new flag.
    /// @param  flags       [in] The new flags.
    virtual void setFlags(SurfaceFlagType flags);

    /// @brief  Returns the surface flags.
    /// @return The surface flags.
    virtual ui32 getFlags() const;

    /// @brief  Will set the windows title.
    /// @param  title       [in] The new windows title.
    virtual void setWindowsTitle(const String &title) = 0;

    /// @brief Will set the windows show state
    /// @param[in]  showState The new show.state
    virtual void showWindow(ShowState showState) = 0;

    /// @brief  Will resize the window.
    /// @param  x   [in] The x-coordinate of the upper left edge-
    /// @param  y   [in] The y-coordinate of the upper left edge.
    /// @param  w   [in] The windows width.
    /// @param  h   [in] The windows height.
    virtual void resize(ui32 x, ui32 y, ui32 w, ui32 h);

    /// @brief  Will return the windows rectangle.
    /// @param  rect    [out] The windows rect.
    virtual void getWindowsRect(Rect2ui &rect) const;

    /// @brief Will set the mouse cursor.
    /// @param ct The mouse cursor type to use.
    virtual void setWindowsMouseCursor(DefaultMouseCursorType ct) = 0;

    // Not used
    AbstractWindow() = delete;

protected:
    /// @brief  Callback to override on creation.
    virtual bool onCreate() = 0;
    /// @brief  Callback to override on destroying.
    virtual bool onDestroy() = 0;
    /// @brief  Callback to override on updates.
    virtual bool onUpdateProperies() = 0;
    /// @brief  The onResize callback.
    virtual void onResize(ui32 x, ui32 y, ui32 w, ui32 h) = 0;
    /// @brief 
    void setShowState(ShowState showState);
    /// @brief 
    ShowState getShowState() const;

private:
    AbstractWindow *mParent;
    AbstractPlatformEventQueue *mQueue;
    ui32 mFlags;
    ShowState mShowState;
    WindowsProperties *mProperties;
    bool mIsCreated;
};

inline void AbstractWindow::setParent(AbstractWindow* parent) {
    mParent = parent;
}

inline AbstractWindow* AbstractWindow::getParent() const {
    return mParent;
}

inline void AbstractWindow::setEventQueue(AbstractPlatformEventQueue* queue) {
    mQueue = queue;
}

inline AbstractPlatformEventQueue* AbstractWindow::getEventQueue() const {
    return mQueue;
}

inline void AbstractWindow::setShowState( ShowState showState ) {
    mShowState = showState;
}

inline AbstractWindow::ShowState AbstractWindow::getShowState() const {
    return mShowState;
}

} // Namespace Platform
} // Namespace OSRE
