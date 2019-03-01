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

#include <osre/UI/UICommon.h>
#include <osre/Common/Object.h>
#include <osre/Common/TFunctor.h>
#include <cppcore/Common/Variant.h>

namespace OSRE {

// Forward declarations
namespace RenderBackend {
    class RenderBackendService;
    class FontBase;
    class Mesh;
}

namespace UI {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT StyleProvider {
public:
    static Style &getCurrentStyle();
    static void setStyle( const Style &newStyle );

private:
    StyleProvider();
    ~StyleProvider();

private:
    static StyleProvider *s_instance;
    Style m_activeStyle;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This static helper struct encapsulate the coordinate transformation from screen 
/// coordinates from the current viewport to the world-coordinates of the scene and back. To use it 
/// you have to reinitialize in on every resize event from the viewport.
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT WidgetCoordMapping {
    /// @brief  Will initialize the translation for ui-coordinates.
    /// @param  viewport    [in] The dimension of the current viewport.
    static void init( const Rect2ui &viewport );

    /// @brief  Will return the current dimension of the viewport.
    /// @return The dimension of the current viewport.
    static const Rect2ui &getDimension();

    /// @brief  Will map an absolution screen position to the world coordinates of the scene.  
    /// @param  x       [in] The x-coordinate in the viewport.
    /// @param  y       [in] The y-coordinate in the viewport.
    /// @param  mappedX [out] The mapped x-coordinate in the scene.
    /// @param  mappedY [out] The mapped y-coordinate in the scene.
    static void mapPosToWorld( ui32 x, ui32 y, f32 &mappedX, f32 &mappedY );

    /// @brief  Will map screen coordinates as an array to the world coordinates of the scene.  
    /// @param  x           [in] The x-coordinate array in the viewport.
    /// @param  y           [in] The y-coordinate array in the viewport.
    /// @param  numPoints   [in] The number of points in the array.
    /// @param  mappedX     [out] The mapped array x-coordinate in the scene.
    /// @param  mappedY     [out] The mapped array y-coordinate in the scene.
    static void mapPosArrayToWorld( ui32 *x, ui32 *y, ui32 numPoints, f32 *mappedX, f32 *mappedY );

    /// @brief  Will map screen coordinate to the world coordinates of the scene.  
    /// @param  viewport    [in] The dimension of the current viewport.
    /// @param  x          [in] The x-coordinate in the viewport.
    /// @param  y           [in] The y-coordinate in the viewport.
    /// @param  mappedX     [out] The mapped x-coordinate in the scene.
    /// @param  mappedY     [out] The mapped y-coordinate in the scene.
    static void mapPosToWorld( const Rect2ui &viewport, ui32 x, ui32 y, f32 &mappedX, f32 &mappedY );

private:
    static Rect2ui s_dim;
};

/// This enum is used to describe the widget type.
enum class WidgetType {
    Button,
    Text,
    Image,
    Panel,
    Screen
};

using UiFunctor = Common::Functor<void, ui32, void *>;

/// @brief  Description of a single 
struct UiProperty {
    String m_name;
    CPPCore::Variant m_data;
};

using RenderBackend::UiVertexCache;
using RenderBackend::UiIndexCache;

struct UiRenderCmd {
    UiVertexCache            m_vc;  ///< Will store all vertices
    UiIndexCache             m_ic;  ///< Will store all indices
    RenderBackend::Material *m_mat; ///< Will store the material

    UiRenderCmd();
    ~UiRenderCmd();

    OSRE_NON_COPYABLE( UiRenderCmd )
};

using UiRenderCmdCache = CPPCore::TArray<UiRenderCmd*>;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Widget : public Common::Object {
public:
    enum WidgetState {
        Pressed = 0,
        Released = 1,
        MaxStates,

        InvalidWidgetState
    };

    virtual ~Widget();
    virtual void setParent( Widget *parent );
    virtual Widget *getParent() const;
    virtual bool addChildWidget( Widget *child );
    virtual bool removeChildWidget( Widget *child );
    virtual bool hasChild(Widget *child);
    virtual ui32 getNumChildren() const;
    virtual Widget *getChildWidgetAt( ui32 idx ) const;
    virtual Widget &setRect( ui32 x, ui32 y, ui32 w, ui32 h );
    virtual const Rect2ui &getRect() const;
    virtual void requestRedraw();
    virtual void redrawDone();
    virtual bool redrawRequested() const;
    virtual void setProperty( UiProperty *prop );
    virtual UiProperty *getProperty( const String &name ) const;
    virtual bool hasProperty( const String &name ) const;
    virtual void setStackIndex( i32 index );
    virtual i32 getStackIndex() const;
    virtual void setVisible( bool visible );
    virtual bool isVisible() const;
    virtual void render( UiRenderCmdCache &renderCmdCache, RenderBackend::RenderBackendService *rbSrv );
    virtual void mouseDown( const Point2ui &pt, void *data);
    virtual void mouseUp( const Point2ui &pt, void *data);
    virtual void setId( ui32 id );
    virtual i32 getId() const;
    virtual void resize( ui32 x, ui32 y, ui32 w, ui32 h );

protected:
    Widget( const String &name, Widget *parent );
    void checkChildren(const Point2ui &pt, void *data, WidgetState state );
    virtual void onResize( ui32 x, ui32 y, ui32 w, ui32 h );
    virtual void onRender( UiRenderCmdCache &renderCmdCache, RenderBackend::RenderBackendService *rbSrv ) = 0;
    virtual void onMouseDown( const Point2ui &pt, void *data);
    virtual void onMouseUp( const Point2ui &pt, void *data);

private:
    Widget *m_parent;
    ui32 m_id;
    CPPCore::TArray<Widget*> m_children;
    CPPCore::TArray<UiProperty*> m_properties;
    Rect2ui m_rect;
    i32 m_stackIndex;
    bool m_redrawRequest;
    bool m_isVisible;
};

inline
void Widget::resize( ui32 x, ui32 y, ui32 w, ui32 h ) {
    onResize( x, y , w, h );
}

} // Namespace UI
} // Namespace OSRE
