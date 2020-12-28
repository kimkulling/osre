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
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/MaterialBuilder.h>
#include <osre/UI/StyleProvider.h>
#include <osre/UI/Widget.h>

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

static const ui16 RectIndices[12] = {
    0, 1, 2, // first triangle
    2, 1, 3, // second triangle
    4, 5, 6, // third triangle
    6, 5, 7  // fourth triangle
};

static f32 getZbyStackIndex(f32 stackIndex) {
    const f32 result = stackIndex * -0.1f;
    return result;
}

static void drawFilledRect(RenderVert *vertices, ui32 x1, ui32 y1, ui32 x2, ui32 y2, f32 stackIndex, Color4 col) {
    if (nullptr == vertices) {
        return;
    }

    f32 screenX1 = 0, screenY1 = 0, screenX2 = 0, screenY2 = 0;
    WidgetCoordMapping::mapPosToWorld(x1, y1, screenX1, screenY1);
    WidgetCoordMapping::mapPosToWorld(x2, y2, screenX2, screenY2);

    // setup triangle vertices
    const f32 z = getZbyStackIndex(stackIndex);
    vertices[0].position = glm::vec3(screenX1, screenY1, z);
    vertices[1].position = glm::vec3(screenX1, screenY2, z);
    vertices[2].position = glm::vec3(screenX2, screenY1, z);
    vertices[3].position = glm::vec3(screenX2, screenY2, z);

    vertices[0].color0 = glm::vec3(col.m_r, col.m_g, col.m_b);
    vertices[1].color0 = glm::vec3(col.m_r, col.m_g, col.m_b);
    vertices[2].color0 = glm::vec3(col.m_r, col.m_g, col.m_b);
    vertices[3].color0 = glm::vec3(col.m_r, col.m_g, col.m_b);

    vertices[0].tex0 = glm::vec2(0, 0);
    vertices[1].tex0 = glm::vec2(0, 1);
    vertices[2].tex0 = glm::vec2(1, 0);
    vertices[3].tex0 = glm::vec2(1, 1);
}

void UIRenderUtils::createRectFromStyle(const Rect2ui &rect, const Style &style, UiVertexCache &vertexCache,
        UiIndexCache &indexCache, ui32 stackIndex, WidgetType type) {
    Color4 col;
    switch (type) {
        case WidgetType::Panel:
            col = style.StyleColors[(ui32)Style::ColorTable::BGColoPanel];
            break;
        case WidgetType::Button:
        default:
            col = style.StyleColors[(ui32)Style::ColorTable::BGColorWidget];
            break;
    }

    static const size_t NumQuadVertices = 4;
    RenderVert vertices[NumQuadVertices];
    drawFilledRect(vertices, rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2(), static_cast<f32>(stackIndex), col);

    size_t vertOffset = vertexCache.numVertices();
    vertexCache.increaseSize(NumQuadVertices);
    for (ui32 i = 0; i < NumQuadVertices; ++i) {
        vertexCache.add(vertices[i]);
    }
    static const size_t NumQuadIndices = 6;

    indexCache.increaseSize(NumQuadIndices);
    for (ui32 i = 0; i < NumQuadIndices; ++i) {
        ui16 index = static_cast<ui16>(vertOffset) + RectIndices[i];
        indexCache.add(index);
    }
}

void UIRenderUtils::createBorderRectFromStyle(const Rect2ui &rect, const Style &style, UiVertexCache &vertexCache,
        UiIndexCache &indexCache, ui32 stackIndex, WidgetType type) {
    RenderVert vertices[8];
    Color4 col, borderCol(0.4f, 0.4f, 0.4f, 0.4f);
    switch (type) {
        case WidgetType::Panel:
            col = style.StyleColors[(ui32)Style::ColorTable::BGColoPanel];
            break;
        case WidgetType::Button:
        default:
            col = style.StyleColors[(ui32)Style::ColorTable::BGColorWidget];
            break;
    }

    ui32 borderWidth = 2;
    drawFilledRect(vertices, rect.getX1(), rect.getY1(), rect.getX2(), rect.getY2(), static_cast<f32>(stackIndex), borderCol);
    drawFilledRect(&vertices[4], rect.getX1() + borderWidth, rect.getY1() + borderWidth, rect.getX2() - borderWidth, rect.getY2() - borderWidth, static_cast<f32>(stackIndex) + 0.5f, col);

    size_t vertOffset = vertexCache.numVertices();
    vertexCache.increaseSize(8);
    for (ui32 i = 0; i < 8; ++i) {
        vertexCache.add(vertices[i]);
    }

    indexCache.increaseSize(12);
    for (ui32 i = 0; i < 12; ++i) {
        ui16 index = static_cast<ui16>(vertOffset) + RectIndices[i];
        indexCache.add(index);
    }
}

Rect2ui UIRenderUtils::createTextBox(const String &text, f32 textSize) {
    ui32 width = 0, height = static_cast<ui32>(textSize);
    for (ui32 i = 0; i < text.size(); ++i) {
        if (text[i] == '\n') {
            height += static_cast<ui32>(textSize);
        }
        width += static_cast<ui32>(textSize);
    }
    const Rect2ui box(0, 0, width, height);

    return box;
}

RenderBackend::Mesh *UIRenderUtils::createGeoFromCache(UiVertexCache &vertexCache, UiIndexCache &indexCache, Material *material) {
    Mesh *mesh = Mesh::create(1);
    if (nullptr == mesh) {
        return nullptr;
    }

    mesh->m_vertextype = VertexType::RenderVertex;
    mesh->m_indextype = IndexType::UnsignedShort;

    if (vertexCache.sizeInBytes() > 0) {
        mesh->m_vb = BufferData::alloc(BufferType::VertexBuffer, vertexCache.sizeInBytes(), BufferAccessType::ReadOnly);
        mesh->m_vb->copyFrom(&vertexCache.m_cache[0], vertexCache.sizeInBytes());

        mesh->m_ib = BufferData::alloc(BufferType::IndexBuffer, indexCache.sizeInBytes(), BufferAccessType::ReadOnly);
        mesh->m_ib->copyFrom(&indexCache.m_cache[0], indexCache.sizeInBytes());
    }

    // use default ui material
    mesh->m_material = material;
    if (nullptr == material) {
        mesh->m_material = Scene::MaterialBuilder::createBuildinUiMaterial();
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
