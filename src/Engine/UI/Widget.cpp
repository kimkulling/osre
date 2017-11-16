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
#include <osre/UI/Widget.h>
#include <osre/UI/Screen.h>
#include <osre/Debugging/osre_debugging.h>

namespace OSRE {
namespace UI {

using namespace ::OSRE::Common;

static const String Tag = "StyleProvider";

StyleProvider *StyleProvider::s_instance = nullptr;

Style &StyleProvider::getCurrentStyle() {
    if ( nullptr == s_instance ) {
        s_instance = new StyleProvider;
    }
    return s_instance->m_activeStyle;
}

void StyleProvider::setStyle( const Style &newStyle ) {
    if ( nullptr == s_instance ) {
        static_cast<void>( StyleProvider::getCurrentStyle() );
    }
    if ( nullptr == s_instance ) {
        osre_debug( Tag, "Singleton instance is nullptr." );
        return;
    }

    if ( newStyle != s_instance->m_activeStyle ) {
        s_instance->m_activeStyle = newStyle;
    }
}

StyleProvider::StyleProvider()
: m_activeStyle() {
    // empty
}

StyleProvider::~StyleProvider() {
    // empty
}

Rect2ui WidgetCoordMapping::s_dim = Rect2ui( 0, 0, 0, 0 );

void WidgetCoordMapping::init( const Rect2ui &dim ) {
    s_dim = dim;
}

const Rect2ui &WidgetCoordMapping::getDimension() {
    return s_dim;
}

void WidgetCoordMapping::mapPosToWorld( ui32 x, ui32 y, f32 &mappedX, f32 &mappedY ) {
    mapPosToWorld( getDimension(), x, y, mappedX, mappedY );
}

void WidgetCoordMapping::mapPosToWorld( const Rect2ui &rect, ui32 x, ui32 y, f32 &mappedX, f32 &mappedY ) {
    mappedX = mappedY = 0.0f;

    // Used UI-axis from (-1 | -1 ) to ( 1 | 1 )
    const ui32 w( rect.m_width );
    const ui32 h( rect.m_height );

    if ( 0.0f != w ) {
        mappedX = ( static_cast<f32>( x ) / static_cast<f32>( w ) * 2.0f ) - 1.0f;
    }
    if ( 0.0f != h ) {
        mappedY = 1.0f - ( static_cast<f32>( y ) / static_cast<f32>( h ) * 2.0f );
    }
}

Widget::Widget( const String &name, Widget *parent )
: Object( name )
, m_id( 99999999 )
, m_parent( nullptr )
, m_children()
, m_properties()
, m_rect( 0, 0, 1, 1 )
, m_stackIndex( 1 )
, m_redrawRequest( true )
, m_isVisible( true ) {
    Widget::setParent( parent );
}

Widget::~Widget() {
    // empty
}

void Widget::setParent( Widget *parent ) {
    if ( parent == m_parent ) {
        return;
    }

    if ( nullptr != m_parent ) {
        m_parent->removeChildWidget( this );
    }
    m_parent = parent;
    m_parent->addChildWidget( this );
}

Widget *Widget::getParent() const {
    return m_parent;
}

bool Widget::addChildWidget( Widget *child ) {
    if ( nullptr == child ) {
        return false;
    }

    m_children.add( child );
    child->get();

    return true;
}

bool Widget::removeChildWidget( Widget *child ) {
    if ( nullptr == child ) {
        return false;
    }

    bool ok( false );
    CPPCore::TArray<Widget*>::Iterator it = m_children.find( child );
    if ( m_children.end() != it ) {
        (*it)->release();
        m_children.remove( it );
        ok = true;
    }

    return ok;
}

ui32 Widget::getNumChildren() const {
    return m_children.size();
}

Widget *Widget::getChildWidgetAt( ui32 idx ) const {
    if ( idx >= m_children.size() ) {
        return nullptr;
    }
    return m_children[ idx ];
}

Widget &Widget::setRect( ui32 x, ui32 y, ui32 w, ui32 h ) {
    Rect2ui newRect( x,y,w,h );
    if ( m_rect != newRect ) {
        m_rect = newRect;
        requestRedraw();
    }
    
    return *this;
}

const Rect2ui &Widget::getRect() const {
    return m_rect;
}

void Widget::requestRedraw() {
    m_redrawRequest = true;
}

void Widget::redrawDone() {
    m_redrawRequest = false;
}

bool Widget::redrawRequested() const {
    return m_redrawRequest;
}

void Widget::setProperty( UiProperty *prop ) {
    if ( !hasProperty( prop->m_name ) ) {
       m_properties.add( prop );
       return;
    }

    for ( ui32 i = 0; i < m_properties.size(); ++i ) {
        if ( m_properties[ i ]->m_name == prop->m_name ) {
            m_properties[ i ] = prop;
        }
    }
}

UiProperty *Widget::getProperty( const String &name ) const {
    for ( ui32 i = 0; i < m_properties.size(); ++i ) {
        if ( m_properties[ i ]->m_name == name ) {
            return m_properties[ i ];
        }
    }

    return nullptr;
}

bool Widget::hasProperty( const String &name ) const {
    return ( getProperty( name ) != nullptr );
}

void Widget::setStackIndex(i32 index) {
    m_stackIndex = index;
}

i32 Widget::getStackIndex() const {
    return m_stackIndex;
}

void Widget::setVisible( bool visible ) {
    if ( visible == m_isVisible ) {
        return;
    }

    m_isVisible = visible;

}

bool Widget::isVisible() const {
    return m_isVisible;
}

void Widget::render( TargetGeoArray &targetGeoArray, RenderBackend::RenderBackendService *rbSrv ) {
    if ( nullptr == rbSrv ) {
        return;
    }

    if ( !isVisible() ) {
        return;
    }

    if ( redrawRequested() ) {
        onRender( targetGeoArray, rbSrv );
        redrawDone();
    }

    const ui32 numChildren( getNumChildren() );
    for ( ui32 i = 0; i < numChildren; i++ ) {
        Widget *child = getChildWidgetAt( i );
        if ( nullptr != child ) {
            child->render( targetGeoArray, rbSrv );
        }
    }
}

void Widget::mouseDown( const Point2ui &pt ) {
    onMouseDown( pt );
}

void Widget::mouseUp( const Point2ui &pt ) {
    onMouseUp( pt );
}

void Widget::onMouseDown( const Point2ui &pt ) {
    for ( ui32 i = 0; i < getNumChildren(); i++ ) {
        Widget *child( getChildWidgetAt( i ) );
        const Rect2ui &r = child->getRect();
        if ( r.isIn( pt ) ) {
            child->onMouseDown( pt );
        }
    }
}

void Widget::onMouseUp( const Point2ui &pt ) {

}

void Widget::setId( ui32 id ) {
    m_id = id;
}

i32 Widget::getId() const {
    return m_id;
}

} // Namespace UI
} // Namespace OSRE
