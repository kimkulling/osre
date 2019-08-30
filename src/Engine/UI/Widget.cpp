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
#include <osre/UI/Widget.h>
#include <osre/UI/Canvas.h>
#include <osre/Debugging/osre_debugging.h>

namespace OSRE {
namespace UI {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

static const c8 *Tag = "StyleProvider";

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
    const Rect2ui &r(getDimension());
    mapPosToWorld( r, x, y, mappedX, mappedY );
}

void WidgetCoordMapping::mapPosArrayToWorld( ui32 *x, ui32 *y, ui32 numPoints, f32 *mappedX, f32 *mappedY ) {
    if ( 0 == numPoints ) {
        return;
    }

    OSRE_ASSERT( nullptr != x );
    OSRE_ASSERT( nullptr != y );
    OSRE_ASSERT( nullptr != mappedX );
    OSRE_ASSERT( nullptr != mappedY );
    for ( ui32 i = 0; i < numPoints; ++i ) {
        mapPosToWorld(s_dim, x[i], y[i], mappedX[i], mappedY[i] );
    }
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

UiRenderCmd::UiRenderCmd()
: m_vc()
, m_ic()
, m_mat( nullptr ) {
    // empty
}

UiRenderCmd::~UiRenderCmd() {
    m_mat = nullptr;
}

Widget::Widget( const String &name, Widget *parent )
: Object( name )
, m_parent( nullptr )
, m_children()
, m_properties()
, m_rect( 0, 0, 1, 1 )
, m_stackIndex( 1 )
, m_dirtyState( 0 )
, m_isVisible( true )
, m_isActive( true ) {
    Widget::setParent( parent );
    Widget::requestRedraw();
    Widget::requestLayouting();
}

Widget::~Widget() {
    // empty
}

void Widget::setParent( Widget *parent ) {
    if ( parent == m_parent ) {
        return;
    }

    if ( nullptr != m_parent ) {
        m_parent->removeWidget( this );
    }
    m_parent = parent;
    m_parent->addWidget( this );
}

Widget *Widget::getParent() const {
    return m_parent;
}

bool Widget::addWidget( Widget *child ) {
    if ( nullptr == child ) {
        return false;
    }

    if (!hasWidget(child)) {
        m_children.add(child);
        child->get();
    } else {
        osre_debug(Tag, "Tried to add child again.");
    }

    return true;
}

bool Widget::removeWidget( Widget *child ) {
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

bool Widget::hasWidget(Widget *child) {
    if (nullptr == child) {
        return false;
    }
    
    CPPCore::TArray<Widget*>::Iterator it = m_children.find(child);
    if (m_children.end() != it) {
        return true;
    }

    return false;
}

size_t Widget::getNumWidgets() const {
    return m_children.size();
}

Widget *Widget::getWidgetAt( size_t idx ) const {
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
    m_dirtyState |= RedrawRequest;
}

void Widget::redrawDone() {
    m_dirtyState = m_dirtyState & ~RedrawRequest;
}

bool Widget::redrawRequested() const {
    return m_dirtyState & RedrawRequest;
}

void Widget::requestLayouting() {
    m_dirtyState |= LayourRequest;
}

void Widget::layoutingDone() {
    m_dirtyState = m_dirtyState & ~LayourRequest;
}

bool Widget::layoutingRequested() const {
    return m_dirtyState & LayourRequest;
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
    if (name.empty()) {
        return nullptr;
    }

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
    requestRedraw();
}

bool Widget::isVisible() const {
    return m_isVisible;
}

void Widget::render( UiRenderCmdCache &renderCmdCache, RenderBackendService *rbSrv ) {
    if ( nullptr == rbSrv ) {
        return;
    }

    if ( !isVisible() ) {
        return;
    }

    if ( redrawRequested() ) {
        onRender( renderCmdCache, rbSrv );
        redrawDone();
    }

    const size_t numChildren( getNumWidgets() );
    for (size_t i = 0; i < numChildren; i++ ) {
        Widget *child = getWidgetAt( i );
        if ( nullptr != child ) {
            child->render( renderCmdCache, rbSrv );
        }
    }
}

void Widget::mouseDown( const Point2ui &pt, void *data) {
    onMouseDown( pt, data );
}

void Widget::mouseUp( const Point2ui &pt, void *data) {
    onMouseUp( pt, data );
}

void Widget::layout() {
    if (layoutingRequested()) {
        onLayout();
        layoutingDone();
    }

    const size_t numChildren( getNumWidgets() );
    for (ui32 i = 0; i < numChildren; ++i) {
        Widget *child = getWidgetAt(i);
        if (nullptr != child) {
            child->layout();
        }
    }
}

void Widget::setState(WidgetState state) {
    m_state = state;
}

WidgetState Widget::getWidgetState() const {
    return m_state;
}

void Widget::checkChildren( const Point2ui &pt, void *data, WidgetState state) {
    for (ui32 i = 0; i < getNumWidgets(); ++i ) {
        Widget *child(getWidgetAt(i));
        const Rect2ui &r = child->getRect();
        if (r.isIn(pt)) {
            if (state == WidgetState::Pressed) {
                child->onMouseDown(pt, data);
            } else if (state == WidgetState::Released) {
                child->onMouseUp(pt, data);
            }
        }
    }
}

void Widget::onMouseDown( const Point2ui &pt, void *data) {
    checkChildren(pt, data, WidgetState::Pressed);
}

void Widget::onMouseUp( const Point2ui &pt, void *data) {
    checkChildren(pt, data, WidgetState::Released);
}


void Widget::onResize( ui32 x, ui32 y, ui32 w, ui32 h ) {
    const Rect2ui &r( getRect() );
    if ( x != r.m_x1 || y != r.m_y1 || w != r.m_width || h != r.m_height ) {
        Widget::setRect( x, y, w, h );
        Widget::requestRedraw();
    }
}

} // Namespace UI
} // Namespace OSRE
