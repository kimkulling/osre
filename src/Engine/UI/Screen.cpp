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
#include <osre/UI/Screen.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/RenderBackend/RenderCommon.h>

#include "UIRenderUtils.h"

namespace OSRE {
namespace UI {

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Platform;

Screen::Screen( const String &name, Widget *parent, i32 width, i32 height )
: Widget( name, parent )
, m_surface( nullptr )
, m_width( width )
, m_height( height ) {
    // empty
}

Screen::~Screen() {
    // empty
}

void Screen::setSurface( AbstractWindow *surface ) {
    if ( surface == m_surface ) {
        return;
    }
    
    m_surface = surface;
    if ( nullptr == surface ) {
        return;
    }

    WindowsProperties *props( surface->getProperties() );
    if ( nullptr == props ) {
        return;
    }
    const ui32 x( props->m_x );
    const ui32 y( props->m_y );
    const ui32 w( props->m_width );
    const ui32 h( props->m_height );
    Rect2ui dim( x, y, w, h );
    WidgetCoordMapping::init( dim );
}

const RenderBackend::TransformMatrixBlock &Screen::getMVP() const {
    return m_transformMatrix;
}

void Screen::onLayout() {

}

void Screen::onRender( UiRenderCmdCache &renderCmdCache, RenderBackendService *rbSrv ) {
    if ( nullptr == m_surface ) {
        return;
    }

    // set 2D render mode
    m_transformMatrix.m_projection = glm::ortho( 0, m_width, m_height, 0 );
    m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.01f, glm::vec3( 1, 1, 0 ) );
    m_transformMatrix.update();
    const ui32 numChildren( getNumWidgets() );
    if ( 0 == numChildren ) {
        return;
    }

    for ( ui32 i=0; i<numChildren; ++i ) {
        Widget *currentChild( getWidgetAt( i ) );
        if ( nullptr == currentChild ) {
            continue;
        }

        currentChild->render( renderCmdCache, rbSrv );
    }
}

void Screen::onResize( ui32 x, ui32 y, ui32 w, ui32 h ) {
    Rect2ui dim( x, y, w, h );
    WidgetCoordMapping::init( dim );
}

} // Namespace UI
} // Namespace OSRE
