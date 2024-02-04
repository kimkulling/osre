/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/Shader.h>

namespace OSRE {
namespace RenderBackend {

struct DrawCmd {
    PrimitiveType mPrimType;
    size_t NumVertices;
    ColorVert *Vertices;
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

DrawCmd *alloc() {
    DrawCmd *dc = new DrawCmd;
    return dc;
}

CanvasRenderer::CanvasRenderer(RenderBackendService * rbSrv, i32 numLayers) : 
        mRbSrv(rbSrv), mDirty(true), mPenColor(1, 1, 1), mResolution(), mActiveLayer(0), mNumLayers(numLayers) {
    osre_assert(mRbSrv != nullptr);
}

CanvasRenderer::~CanvasRenderer() {
    // empty
}

void CanvasRenderer::render() {
    osre_assert(mRbSrv != nullptr);

    if (!isDirty()) {
        return;
    }

    for (size_t i=0; i<mDrawCmdArray.size(); ++i) {
        const auto *dc = mDrawCmdArray[i];
    }

    setClean();
}

void CanvasRenderer::setResolution(i32 x, i32 y, i32 w, i32 h) {
    if (mRbSrv == nullptr) {
        return;
    }

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
    }

    mDrawCmdArray.add(dc);

    setDirty();
}

} // namespace RenderBackend
} // namespace OSRE
