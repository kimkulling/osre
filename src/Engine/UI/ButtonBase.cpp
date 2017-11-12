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
#include <osre/UI/ButtonBase.h>
#include <osre/Platform/AbstractSurface.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>

#include "UIRenderUtils.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#   define GLM_ENABLE_EXPERIMENTAL
#endif // GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace OSRE {
namespace UI {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

ButtonBase::FunctorContainer::FunctorContainer()
: m_used( false )
, m_callback() {
    // empty
}

ButtonBase::FunctorContainer::~FunctorContainer() {
    // empty
}

ButtonBase::ButtonBase( const String &name, Widget *parent )
: Widget( name, parent )
, m_label()
, m_image()
, m_callback( nullptr )
, m_geo( nullptr ) {
    static_cast<void>( StyleProvider::getCurrentStyle() );
    if ( nullptr != parent ) {
        setStackIndex(parent->getStackIndex() + 1);
    }
    m_callback = new FunctorContainer[ MaxStates ];
}

ButtonBase::~ButtonBase() {
    delete[] m_callback;
    m_callback = nullptr;
}

void ButtonBase::setLabel( const String &label ) {
    if ( m_label != label ) {
        m_label = label;
        Widget::requestRedraw();
    }
}

const String &ButtonBase::getLabel() const {
    return m_label;
}

void ButtonBase::setImage( const String &name ) {
    m_image = name;
}

const String &ButtonBase::getImage() const {
    return m_image;
}

void ButtonBase::registerCallback( ButtonState state, UIFunctor functor ) {
    m_callback[ state ].m_used = true;
    m_callback[ state ].m_callback = functor;
    functor.incRef();
}

ButtonBase *ButtonBase::createBaseButton(const String &name, Widget *parent) {
    return new ButtonBase(name, parent);
}

void ButtonBase::onRender( TargetGeoArray &targetGeoArray, RenderBackend::RenderBackendService *rbSrv ) {
    const Style &activeStyle = StyleProvider::getCurrentStyle();
    const Rect2ui &rect( getRect() );

    const String &label( getLabel() );

    if ( !label.empty() ) {
//        PlatformInterface::getInstance();
//        Rect2ui textBox = UIRenderUtils::computeTextBox( label, )
    }

    if ( nullptr == m_geo ) {
        m_geo = UIRenderUtils::createRectFromStyle( WidgetType::Button, rect, activeStyle, getStackIndex() );
        rbSrv->attachGeo( m_geo, 0 );
        targetGeoArray.add( m_geo );
    }
}

void ButtonBase::onMouseDown(const Point2ui &pt) {
    if ( m_callback[ 0 ].m_used ) {
        const FunctorContainer &ct( m_callback[ 0 ] );
        ct.m_callback( Widget::getId(), nullptr );
    }
    Widget::onMouseDown(pt);
    Widget::requestRedraw();
}

void ButtonBase::onMouseUp(const Point2ui &pt) {
    Widget::onMouseUp(pt);
    Widget::requestRedraw();
}

} // Namespace UI
} // Namespace OSRE
