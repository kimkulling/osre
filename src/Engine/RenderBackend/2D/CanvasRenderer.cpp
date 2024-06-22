/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "CanvasRenderer.h"
#include "RenderBackend/Mesh.h"
#include "RenderBackend/Shader.h"
#include <cppcore/Memory/TPoolAllocator.h>

namespace OSRE {
namespace RenderBackend {

using namespace cppcore;

struct DrawCmd {
    PrimitiveType mPrimType;
    size_t NumVertices;
    ColorVert *Vertices;
    size_t NumIndices;
    ui32 *Indices;

    DrawCmd() = default;
    ~DrawCmd() = default;
};

inline void clip(const Rect2i &resolution, i32 x, i32 y, i32 &x_out, i32 &y_out) {
    x_out = x;
    y_out = y;
    if (x < resolution.getX1()) {
        x_out = resolution.getX1();
    }

    if (x > resolution.getX2()) {
        x_out = resolution.getX2();
    }

    if (y < resolution.getY1()) {
        y_out = resolution.getY1();
    }

    if (y > resolution.getY2()) {
        y_out = resolution.getY2();
    }
}

static TPoolAllocator<DrawCmd> sAllocator;

DrawCmd *alloc() {
    if (sAllocator.capacity() == 0) {
        sAllocator.reserve(1024);
    }

    DrawCmd *dc = sAllocator.alloc();
    return dc;
}

void dealloc(DrawCmd *cmd) {
    if (cmd == nullptr) {
        return;
    }

    delete cmd;
}

CanvasRenderer::CanvasRenderer(i32 numLayers) :
        mDirty(true), mPenColor(1, 1, 1), mResolution(), mActiveLayer(0), mNumLayers(numLayers), mMesh(nullptr) {
    // empty
}

CanvasRenderer::~CanvasRenderer() {
    // empty
}

void CanvasRenderer::preRender(RenderBackendService *rbSrv) {
    osre_assert(rbSrv != nullptr);

    //rbSrv->
}

void CanvasRenderer::render(RenderBackendService *rbSrv) {
    osre_assert(rbSrv != nullptr);

    if (!isDirty()) {
        return;
    }

    if (mMesh == nullptr) {
        mMesh = new Mesh("2d", VertexType::ColorVertex, IndexType::UnsignedInt);
    }

    for (size_t i=0; i<mDrawCmdArray.size(); ++i) {
        const auto &dc = *mDrawCmdArray[i];
        if (dc.Vertices == nullptr) {
            continue;
        }
        const ui32 last = static_cast<ui32>(mMesh->getLastIndex());
        mMesh->attachVertices(dc.Vertices, dc.NumVertices);
        mMesh->attachIndices(dc.Indices, dc.NumIndices);
        mMesh->addPrimitiveGroup(dc.NumIndices, dc.mPrimType, last);
    }

    mDrawCmdArray.resize(0);
    setClean();
}

void CanvasRenderer::postRender(RenderBackendService *rbSrv) {
    osre_assert(rbSrv != nullptr);

    //rbSrv->
}

void CanvasRenderer::setResolution(i32 x, i32 y, i32 w, i32 h) {
    Rect2i newResolution(x, y, w, h);
    if (mResolution == newResolution) {
        return;
    }

    mResolution = newResolution;
    setDirty();
}

bool CanvasRenderer::selectLayer(i32 layer) {
    if (layer < 0 || layer >= mNumLayers) {
        return false;
    }

    mActiveLayer = layer;

    return true;
}

i32 CanvasRenderer::getActiveLayer() const {
    return mActiveLayer;
}

void CanvasRenderer::setcolor(const Color4 &color) {
    mPenColor.r = color.m_r;
    mPenColor.g = color.m_g;
    mPenColor.b = color.m_b;
}

void CanvasRenderer::drawline(i32 x1, i32 y1, i32 x2, i32 y2) {
    DrawCmd *dc = alloc();

    i32 x_clipped, y_clipped;
    clip(mResolution, x1, y1, x_clipped, y_clipped);
    dc->NumVertices = 2;
    dc->Vertices = new ColorVert[dc->NumVertices];
    dc->Vertices[0].color0 = mPenColor;
    dc->Vertices[0].position.x = (f32)x_clipped;
    dc->Vertices[0].position.y = (f32)y_clipped;
    dc->Vertices[0].position.z = static_cast<f32>(mActiveLayer);

    clip(mResolution, x2, y2, x_clipped, y_clipped);
    dc->Vertices[1].color0 = mPenColor;
    dc->Vertices[1].position.x = (f32)x_clipped;
    dc->Vertices[1].position.y = (f32)y_clipped;
    dc->Vertices[1].position.z = static_cast<f32>(mActiveLayer);

    dc->NumIndices = 2;
    dc->Indices = new ui32[dc->NumIndices];
    dc->Indices[0] = 0;
    dc->Indices[0] = 1;

    mDrawCmdArray.add(dc);

    setDirty();
}

void CanvasRenderer::drawTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, bool filled) {
    DrawCmd *dc = alloc();
    dc->NumVertices = 3;
    dc->Vertices = new ColorVert[dc->NumVertices];

    i32 x_clipped, y_clipped;
    clip(mResolution, x1, y1, x_clipped, y_clipped);
    dc->Vertices[0].color0 = mPenColor;
    dc->Vertices[0].position.x = (f32)x_clipped;
    dc->Vertices[0].position.y = (f32)y_clipped;
    dc->Vertices[0].position.z = static_cast<f32>(mActiveLayer);

    clip(mResolution, x2, y2, x_clipped, y_clipped);
    dc->Vertices[1].color0 = mPenColor;
    dc->Vertices[1].position.x = (f32)x_clipped;
    dc->Vertices[1].position.y = (f32)y_clipped;
    dc->Vertices[1].position.z = static_cast<f32>(mActiveLayer);

    clip(mResolution, x3, y3, x_clipped, y_clipped);
    dc->Vertices[2].color0 = mPenColor;
    dc->Vertices[2].position.x = (f32)x_clipped;
    dc->Vertices[2].position.y = (f32)y_clipped;
    dc->Vertices[2].position.z = static_cast<f32>(mActiveLayer);

    if (filled) {
    } else {
    }

    dc->NumIndices = 3;
    dc->Indices = new ui32[dc->NumIndices];
    dc->Indices[0] = 0;
    dc->Indices[1] = 1;
    dc->Indices[2] = 2;

    mDrawCmdArray.add(dc);

    setDirty();
}

void CanvasRenderer::drawRect(i32 x, i32 y, i32 w, i32 h, bool filled) {
    DrawCmd *dc = alloc();

    if (filled) {
        dc->mPrimType = PrimitiveType::TriangleList;
        dc->NumVertices = 6;
        dc->Vertices = new ColorVert[dc->NumVertices];

        i32 x_clipped, y_clipped;
        clip(mResolution, x, y, x_clipped, y_clipped);
        dc->Vertices[0].color0 = mPenColor;
        dc->Vertices[0].position.x = (f32)x_clipped;
        dc->Vertices[0].position.y = (f32)y_clipped;
        dc->Vertices[0].position.z = static_cast<f32>(mActiveLayer);

        clip(mResolution, x+w, y, x_clipped, y_clipped);
        dc->Vertices[1].color0 = mPenColor;
        dc->Vertices[1].position.x = (f32)x_clipped;
        dc->Vertices[1].position.y = (f32)y_clipped;
        dc->Vertices[1].position.z = static_cast<f32>(mActiveLayer);

        clip(mResolution, x+w, y+h, x_clipped, y_clipped);
        dc->Vertices[2].color0 = mPenColor;
        dc->Vertices[2].position.x = (f32)x_clipped;
        dc->Vertices[2].position.y = (f32)y_clipped;
        dc->Vertices[2].position.z = static_cast<f32>(mActiveLayer);

        clip(mResolution, x+w, y+h, x_clipped, y_clipped);
        dc->Vertices[3].color0 = mPenColor;
        dc->Vertices[3].position.x = (f32)x_clipped;
        dc->Vertices[3].position.y = (f32)y_clipped;
        dc->Vertices[3].position.z = static_cast<f32>(mActiveLayer);

        clip(mResolution, x, y+h, x_clipped, y_clipped);
        dc->Vertices[4].color0 = mPenColor;
        dc->Vertices[4].position.x = (f32)x_clipped;
        dc->Vertices[4].position.y = (f32)y_clipped;
        dc->Vertices[4].position.z = static_cast<f32>(mActiveLayer);

        clip(mResolution, x, y, x_clipped, y_clipped);
        dc->Vertices[5].color0 = mPenColor;
        dc->Vertices[5].position.x = (f32)x_clipped;
        dc->Vertices[5].position.y = (f32)y_clipped;
        dc->Vertices[5].position.z = static_cast<f32>(mActiveLayer);

        dc->NumIndices = 6;
        dc->Indices = new ui32[dc->NumIndices];
        dc->Indices[0] = 0;
        dc->Indices[1] = 1;
        dc->Indices[2] = 2;

        dc->Indices[3] = 2;
        dc->Indices[4] = 3;
        dc->Indices[5] = 0;
    } else {
        dc->NumVertices = 4;
        dc->mPrimType = PrimitiveType::LineList;
        dc->Vertices = new ColorVert[dc->NumVertices];

        i32 x_clipped, y_clipped;
        clip(mResolution, x, y, x_clipped, y_clipped);
        dc->Vertices[0].color0 = mPenColor;
        dc->Vertices[0].position.x = (f32)x_clipped;
        dc->Vertices[0].position.y = (f32)y_clipped;
        dc->Vertices[0].position.z = static_cast<f32>(mActiveLayer);

        clip(mResolution, x, y, x_clipped, y_clipped);
        dc->Vertices[1].color0 = mPenColor;
        dc->Vertices[1].position.x = (f32)x_clipped + w;
        dc->Vertices[1].position.y = (f32)y_clipped;
        dc->Vertices[1].position.z = static_cast<f32>(mActiveLayer);

        clip(mResolution, x, y, x_clipped, y_clipped);
        dc->Vertices[2].color0 = mPenColor;
        dc->Vertices[2].position.x = (f32)x_clipped + w;
        dc->Vertices[2].position.y = (f32)y_clipped + h;
        dc->Vertices[2].position.z = static_cast<f32>(mActiveLayer);

        clip(mResolution, x, y, x_clipped, y_clipped);
        dc->Vertices[3].color0 = mPenColor;
        dc->Vertices[3].position.x = (f32)x_clipped;
        dc->Vertices[3].position.y = (f32)y_clipped + h;
        dc->Vertices[3].position.z = static_cast<f32>(mActiveLayer);

        dc->NumIndices = 8;
        dc->Indices = new ui32[dc->NumIndices];
        dc->Indices[0] = 0;
        dc->Indices[1] = 1;

        dc->Indices[1] = 1;
        dc->Indices[2] = 2;

        dc->Indices[2] = 2;
        dc->Indices[3] = 3;

        dc->Indices[3] = 3;
        dc->Indices[0] = 0;
    }

    mDrawCmdArray.add(dc);

    setDirty();
}

} // namespace RenderBackend
} // namespace OSRE
