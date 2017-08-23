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

RectUI WidgetCoordMapping::s_dim = RectUI( 0, 0, 0, 0 );

void WidgetCoordMapping::init( const RectUI &dim ) {
    s_dim = dim;
}

const RectUI &WidgetCoordMapping::getDimension() {
    return s_dim;
}

void WidgetCoordMapping::mapPosToWorld( ui32 x, ui32 y, f32 &mappedX, f32 &mappedY ) {
    mapPosToWorld( getDimension(), x, y, mappedX, mappedY );
}

void WidgetCoordMapping::mapPosToWorld( const RectUI &rect, ui32 x, ui32 y, f32 &mappedX, f32 &mappedY ) {
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
, m_parent( nullptr )
, m_children()
, m_rect( 0, 0, 1, 1 )
, m_stackIndex( 1 )
, m_redrawRequest( true ) {
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
    RectUI newRect( x,y,w,h );
    if ( m_rect != newRect ) {
        m_rect = newRect;
        requestRedraw();
    }
    
    return *this;
}

const RectUI &Widget::getRect() const {
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

void Widget::setStackIndex(i32 index) {
    m_stackIndex = index;
}

i32 Widget::getStackIndex() const {
    return m_stackIndex;
}

void Widget::render( TargetGeoArray &targetGeoArray, RenderBackend::RenderBackendService *rbSrv ) {
    if ( nullptr == rbSrv ) {
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

} // Namespace UI
} // Namespace OSRE
