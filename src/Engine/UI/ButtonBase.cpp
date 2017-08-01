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

#include <osre/Scene/MaterialBuilder.h>

#include "UIRenderUtils.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

namespace OSRE {
namespace UI {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

ButtonBase::ButtonBase( const String &name, Widget *parent )
: Widget( name, parent )
, m_label() {
    static_cast<void>( StyleProvider::getCurrentStyle() );
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

void ButtonBase::onRender( TargetGeoArray &targetGeoArray, RenderBackend::RenderBackendService *rbSrv ) {
    const Style &activeStyle = StyleProvider::getCurrentStyle();
    const RectUI &rect( getRect() );

    Geometry *geo = UIRenderUtils::createRectFromStyle( WidgetType::Button, rect, activeStyle );
    /*AttachGeoEventData *attachGeoEvData = new AttachGeoEventData;
    attachGeoEvData->m_numGeo = 1;
    attachGeoEvData->m_geo = new Geometry*[ 1 ];
    attachGeoEvData->m_geo[ 0 ] = geo;
    rbSrv->sendEvent( &OnAttachSceneEvent, attachGeoEvData );*/
    rbSrv->attachGeo( geo, 0 );

    /*m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.001f, glm::vec3( 1, 1, 0 ) );

    UniformVar *parameter = UniformVar::create( "MVP", ParameterType::PT_Mat4 );
    m_transformMatrix.update();
    ::memcpy( parameter->m_data.m_data, m_transformMatrix.getMVP(), sizeof( glm::mat4 ) );

    geo->m_material->m_parameters = parameter;
    geo->m_material->m_numParameters++;*/

    targetGeoArray.add( geo );
}

} // Namespace UI
} // Namespace OSRE
