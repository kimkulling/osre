/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/RenderBackend/TextRenderer.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/RenderBackend/RenderBackendService.h>

namespace OSRE {
namespace RenderBackend {

using namespace OSRE::Scene;

TextRenderer::TextRenderer( RenderBackendService *rb )
: m_data( nullptr )
, m_textMap()
, m_rb( rb ) {
	// empty
}

TextRenderer::~TextRenderer() {
    // empty
}

void TextRenderer::drawText(f32 x, f32 y, f32 scale, const String &text, bool isDynamic ) {
	GeometryBuilder geoBuilder;
	if ( !isDynamic ) {
        const ui32 hashId( CPPCore::Hash::toHash( text.c_str(), TextHashMap::InitSize ) );
        StaticGeometry *geo( nullptr );
        if ( !m_textMap.hasKey( hashId ) ) {
            geo = geoBuilder.allocTextBox( x, y, scale, text );
            m_textMap.insert( hashId, geo );
        } else {
            m_textMap.getValue( hashId, geo );
        }
        AttachGeoEventData *attachGeoEvData = new AttachGeoEventData;
        Scene::GeometryBuilder myBuilder;
        attachGeoEvData->m_numGeo = 1;
        attachGeoEvData->m_geo = geo;

        m_rb->sendEvent( &OnAttachSceneEvent, attachGeoEvData );
	}
}

} // Namespace RenderBackend
} // Namespace OSRE
