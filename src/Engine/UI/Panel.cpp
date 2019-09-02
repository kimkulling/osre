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
#include <osre/UI/Panel.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Scene/MaterialBuilder.h>

#include "UIRenderUtils.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#   define GLM_ENABLE_EXPERIMENTAL
#endif // GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

namespace OSRE {
namespace UI {

using namespace ::OSRE::RenderBackend;

Panel::Panel( const String &name, ui32 flags, Widget *parent )
: Widget( name, parent )
, m_angle( 0.02f )
, m_flags( flags )
, m_headline()
, m_transformMatrix() {
    // empty
}

Panel::~Panel() {
    // empty
}

ui32 Panel::getFlags() const {
    return m_flags;
}

bool Panel::isEnabled( ui32 flag ) const {
    return m_flags & static_cast<ui32>( flag );
}

void Panel::setHeadline( const String &headline ) {
    if ( m_headline != headline ) {
        m_headline = headline;
        Widget::requestRedraw();
    }
}

const String &Panel::getHeadline() const {
    return m_headline;
}

void Panel::onLayout() {

}

void Panel::onRender( UiRenderCmdCache &renderCmdCache, RenderBackendService* ) {
    const Style &activeStyle = StyleProvider::getCurrentStyle();
    const Rect2ui &rect( getRect() );
    UiVertexCache vertexCache;
    UiIndexCache indexCache;
    UIRenderUtils::createRectFromStyle( WidgetType::Panel, rect, activeStyle, vertexCache, indexCache );

    UiRenderCmd *cmd( new UiRenderCmd );
    cmd->m_vc = vertexCache;
    cmd->m_ic = indexCache;

    renderCmdCache.add( cmd );
}

} // Namespace UI
} // Namespace OSRE
