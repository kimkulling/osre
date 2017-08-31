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

#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace OSRE {
namespace UI {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

ButtonBase::ButtonBase( const String &name, Widget *parent )
: Widget( name, parent )
, m_label() {
    static_cast<void>( StyleProvider::getCurrentStyle() );
    if ( nullptr != parent ) {
        setStackIndex(parent->getStackIndex() + 1);
    }
    m_callbacks[ 0 ] = nullptr;
    m_callbacks[ 1 ] = nullptr;
}

ButtonBase::~ButtonBase() {
    // empty
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

void ButtonBase::registerCallback( ButtonState state, UIFunctor *functor ) {
    m_callbacks[ state ] = functor;
}

void ButtonBase::setId( ui32 id ) {
    m_id = id;
}

i32 ButtonBase::getId() const {
    return m_id;
}

ButtonBase *ButtonBase::createBaseButton(const String &name, Widget *parent) {
    return new ButtonBase(name, parent);
}

void ButtonBase::onRender( TargetGeoArray &targetGeoArray, RenderBackend::RenderBackendService *rbSrv ) {
    const Style &activeStyle = StyleProvider::getCurrentStyle();
    const Rect2ui &rect( getRect() );

    Geometry *geo = UIRenderUtils::createRectFromStyle( WidgetType::Button, rect, activeStyle, getStackIndex() );
    rbSrv->attachGeo( geo, 0 );

    targetGeoArray.add( geo );
}

void ButtonBase::onButtonPressed() {
    if ( m_callbacks[ 0 ] ) {
        (*m_callbacks[ 0 ])( m_id, nullptr );
    }
    Widget::requestRedraw();
}

void ButtonBase::onButtonReleased() {
    Widget::requestRedraw();
}

} // Namespace UI
} // Namespace OSRE
