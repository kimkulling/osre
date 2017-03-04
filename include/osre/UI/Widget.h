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

#include <osre/Common/Object.h>

namespace OSRE {

namespace RenderBackend {
    class RenderBackendService;
    struct Geometry;
}

namespace UI {

class Screen;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT Style {
    enum class ColorTable {
        FGColorPanel = 0,
        BGColorPanel,
        FGColorWidget,
        BGColorWidget,
        TextColor,
        Max
    };

    CPPCore::TArray<Color4> m_properties;

    Style()
    : m_properties() {
        // color panel
        m_properties.add( Color4( 1.f, 1.f, 1.f, 1.f ) );
        m_properties.add( Color4( 0.9f, 0.9f, 0.9f, 1.f ) );
        // color button
        m_properties.add( Color4( 1.f, 1.f, 1.f, 1.f ) );
        m_properties.add( Color4( 0.5f, 0.5f, 0.5f, 1.f ) );
        // color text
        m_properties.add( Color4( 1.f, 1.f, 1.f, 1.f ) );
    }

    bool operator == ( const Style &rhs ) const {
        bool equal( true );
        for ( ui32 i = 0; i < (ui32) ColorTable::Max; i++ ) {
            if ( m_properties[ i ] != rhs.m_properties[ i ] ) {
                equal = false;
                break;
            }
        }
        return equal;
    }

    bool operator != ( const Style &rhs ) const {
        return !( *this == rhs );
    }
};

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
///	@brief  
//-------------------------------------------------------------------------------------------------
struct OSRE_EXPORT WidgetCoordMapping {
    static void init( const RectUI &dim );
    static const RectUI &getDimension();
    static void mapPosToWorld( ui32 x, ui32 y, f32 &mappedX, f32 &mappedY );
    static void mapPosToWorld( const RectUI &rect, ui32 x, ui32 y, f32 &mappedX, f32 &mappedY );

private:
    static RectUI s_dim;
};

enum class WidgetType {
    Panel,
    Button
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Widget : public Common::Object {
public:
    typedef CPPCore::TArray<RenderBackend::Geometry*> TargetGeoArray;
    
    virtual ~Widget();
    virtual void setParent( Widget *parent );
    virtual Widget *getParent() const;
    virtual bool addChildWidget( Widget *child );
    virtual bool removeChildWidget( Widget *child );
    virtual ui32 getNumChildren() const;
    virtual Widget *getChildWidgetAt( ui32 idx ) const;
    virtual void setRect( ui32 x, ui32 y, ui32 w, ui32 h );
    virtual const RectUI &getRect() const;
    virtual void requestRedraw();
    virtual void redrawDone();
    virtual bool redrawRequested() const;
    virtual void render( TargetGeoArray &targetGeoArray, RenderBackend::RenderBackendService *rbSrv );

protected:
    Widget( const String &name, Widget *parent );
    virtual void onRender( TargetGeoArray &targetGeoArray, RenderBackend::RenderBackendService *rbSrv ) = 0;

private:
    Widget *m_parent;
    CPPCore::TArray<Widget*> m_children;
    RectUI m_rect;
    ui32 m_z;
    bool m_redrawRequest;
};

} // Namespace UI
} // Namespace OSRE
