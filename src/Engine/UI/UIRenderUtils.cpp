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
#include "UIRenderUtils.h"
#include <osre/UI/StyleProvider.h>
#include <osre/UI/Widget.h>
#include <osre/Scene/MaterialBuilder.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Mesh.h>

#include <cppcore/Memory/MemUtils.h>

namespace OSRE {
namespace UI {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

UIRenderUtils::UIRenderUtils() {
    // empty
}

UIRenderUtils::~UIRenderUtils() {
    // empty
}

static const ui16 RectIndices[6] = {
    0, 1, 2, // first triangle
    2, 1, 3  // second triangle
};

static f32 getZbyStackIndex(ui32 stackIndex ) {
    const f32 result = (f32)stackIndex * -0.1f;
    return result;
}

void UIRenderUtils::drawRectFromStyle( const Rect2ui &rect, const Style &style, UiVertexCache &vertexCache,
        UiIndexCache &indexCache, ui32 stackIndex, WidgetType type ) {

    f32 x1, y1, x2, y2;
    WidgetCoordMapping::mapPosToWorld( rect.getX1(), rect.getY1(), x1, y1 );
    WidgetCoordMapping::mapPosToWorld( rect.getX2(), rect.getY2(), x2, y2 );

    // setup triangle vertices
    RenderVert vertices[ 4 ];
    vertices[ 0 ].position = glm::vec3( x1, y1, getZbyStackIndex( stackIndex ) );
    vertices[ 1 ].position = glm::vec3( x1, y2, getZbyStackIndex( stackIndex ) );
    vertices[ 2 ].position = glm::vec3( x2, y1, getZbyStackIndex( stackIndex ) );
    vertices[ 3 ].position = glm::vec3( x2, y2, getZbyStackIndex( stackIndex ) );

    Color4 col;
    switch (type) {
    case WidgetType::Panel:
        col = style.StyleColors[ ( ui32 )Style::ColorTable::BGColoPanel ];
        break;
    case WidgetType::Button:
    default:
        col = style.StyleColors[ ( ui32 )Style::ColorTable::BGColorWidget ];
        break;
    }

    vertices[ 0 ].color0 = glm::vec3( col.m_r, col.m_g, col.m_b );
    vertices[ 1 ].color0 = glm::vec3( col.m_r, col.m_g, col.m_b );
    vertices[ 2 ].color0 = glm::vec3( col.m_r, col.m_g, col.m_b );
    vertices[ 3 ].color0 = glm::vec3( col.m_r, col.m_g, col.m_b );

    vertices[ 0 ].tex0 = glm::vec2( 0, 0 );
    vertices[ 1 ].tex0 = glm::vec2( 0, 1 );
    vertices[ 2 ].tex0 = glm::vec2( 1, 0 );
    vertices[ 3 ].tex0 = glm::vec2( 1, 1 );

    size_t vertOffset = vertexCache.numVertices();
    vertexCache.increaseSize(4);
    for ( ui32 i = 0; i < 4; ++i ) {
        vertexCache.add( vertices[ i ] );
    }

    indexCache.increaseSize( 6 );
    for ( ui32 i = 0; i < 6; ++i ) {
        ui16 index = static_cast<ui16>(vertOffset) + RectIndices[i];
        indexCache.add( index );
    }
}

Rect2ui UIRenderUtils::drawTextBox( const String &text, f32 textSize ) {
    ui32 width = 0, height = static_cast<ui32>(textSize);
    for ( ui32 i = 0; i < text.size(); ++i ) {
        if ( text[ i ] == '\n' ) {
            height += static_cast<ui32>( textSize );
        }
        width += static_cast<ui32>( textSize );
    }
    const Rect2ui box( 0, 0, width, height );
    
    return box;
}

RenderBackend::Mesh *UIRenderUtils::createGeoFromCache( UiVertexCache &vertexCache, UiIndexCache &indexCache, Texture* texture ) {
    Mesh *mesh = Mesh::create( 1 );

    mesh->m_vertextype = VertexType::RenderVertex;
    mesh->m_indextype = IndexType::UnsignedShort;

    if (vertexCache.sizeInBytes() > 0) {
        mesh->m_vb = BufferData::alloc(BufferType::VertexBuffer, vertexCache.sizeInBytes(), BufferAccessType::ReadOnly);
        mesh->m_vb->copyFrom(&vertexCache.m_cache[0], vertexCache.sizeInBytes());

        mesh->m_ib = BufferData::alloc(BufferType::IndexBuffer, indexCache.sizeInBytes(), BufferAccessType::ReadOnly);
        mesh->m_ib->copyFrom(&indexCache.m_cache[0], indexCache.sizeInBytes());
    }

    // use default ui material
    mesh->m_material = Scene::MaterialBuilder::createBuildinUiMaterial();
    if ( nullptr != texture ) {
        mesh->m_material->m_numTextures = 1;
        mesh->m_material->m_textures = new Texture *[mesh->m_material->m_numTextures];
        mesh->m_material->m_textures[0] = texture;
    }

    mesh->m_numPrimGroups = 1;
    mesh->m_primGroups = new PrimitiveGroup[mesh->m_numPrimGroups];
    for (size_t i = 0; i < mesh->m_numPrimGroups; ++i) {
        mesh->m_primGroups[i].m_indexType = IndexType::UnsignedShort;
        mesh->m_primGroups[i].m_numIndices = (ui32)indexCache.numIndices();
        mesh->m_primGroups[i].m_primitive = PrimitiveType::TriangleList;
        mesh->m_primGroups[i].m_startIndex = 0;
    }

    return mesh;
}

} // Namespace UI
} // Namespace OSRE
