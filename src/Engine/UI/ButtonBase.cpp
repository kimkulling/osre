#include <osre/UI/ButtonBase.h>
#include <osre/Platform/AbstractSurface.h>

namespace OSRE {
namespace UI {

using namespace ::OSRE::Common;

StyleProvider *StyleProvider::s_instance = nullptr;

Style &StyleProvider::getCurrentStyle() {
    if ( nullptr == s_instance ) {
        s_instance = new StyleProvider;
    }
    return s_instance->m_activeStyle;
}

StyleProvider::StyleProvider()
: m_activeStyle() {
    // empty
}

StyleProvider::~StyleProvider() {
    // empty
}

Widget::Widget( const String &name, Widget *parent )
: Object( name )
, m_parent( parent )
, m_children()
, m_rect( 0,0,1,1 )
, m_z( 1 ) {
    //
}

Widget::~Widget() {

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

void Widget::setRect( ui32 x, ui32 y, ui32 w, ui32 h ) {
    RectUI newRect( x,y,w,h );
    if ( m_rect != newRect ) {
        m_rect = newRect;
        requestRedraw();
    }
}

void Widget::requestRedraw() {
    m_redrawRequest = true;
}

bool Widget::redrawRequested() const {
    return m_redrawRequest;
}

Screen::Screen( const String &name, Widget *parent )
: Widget( name, parent )
, m_surface( nullptr ) {

}

Screen::~Screen() {
}

void Screen::setSurface( Platform::AbstractSurface *surface ) {
    m_surface = surface;
}

void Screen::render() {
    if ( nullptr == m_surface ) {
        return;
    }
}

Panel::Panel( const String &name, Widget *parent )
: Widget( name, parent ) {
}

Panel::~Panel() {

}

void Panel::render() {
}

ButtonBase::ButtonBase( const String &name, Widget *parent )
: Widget( name, parent ) {
    static_cast<void>( StyleProvider::getCurrentStyle() );
}

ButtonBase::~ButtonBase() {
    // empty
}

void ButtonBase::render() {
    // empty
}

}
}
