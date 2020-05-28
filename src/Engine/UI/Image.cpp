/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/UI/Image.h>
#include <osre/Scene/MeshBuilder.h>

namespace OSRE {
namespace UI {

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;

Image::Image( const String &name, Widget *parent )
: Widget( name, parent ) 
, m_imageUri() {
    // empty
}

Image::~Image() {
    // empty
}

void Image::setUri( const IO::Uri &imageUri ) {
    if ( m_imageUri == imageUri ) {
        return;
    }
    
    m_imageUri = imageUri;
    Widget::requestRedraw();
}

const IO::Uri &Image::getUri() const {
    return m_imageUri;
}

void Image::onLayout() {
    if (m_imageUri.isEmpty()) {
        return;
    }

    if ( LayoutPolicy::Auto == getLayoutPolicy() ) {
        
    } else if (LayoutPolicy::Fixed == getLayoutPolicy()) {

    }
}

void Image::onRender( UiRenderCmdCache &renderCmdCache, RenderBackendService * ) {
    MeshBuilder builder;
    const Rect2ui &imageRect = Widget::getRect();
    builder.allocUiQuad( imageRect, renderCmdCache.m_vc, renderCmdCache.m_ic );
}

} // Namespace UI
} // Namespace OSRE
