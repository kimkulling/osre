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
#pragma once

#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace RenderBackend {

class RenderBackendService;

class CanvasRenderer : IRenderer {
public:
    CanvasRenderer(RenderBackendService *rbSrv, i32 numLayers);
    ~CanvasRenderer() override;
    void setResolution(i32 x, i32 y, i32 w, i32 h);
    bool selectLayer(i32 layer);
    i32 getActiveLayer() const;
    void setcolor(const Color4 &color);
    void drawline(i32 x1, i32 y1, i32 x2, i32 y2);
    void drawTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, bool filled);
    void drawRect(i32 x, i32 y, i32 w, i32 h, bool filled);
    void render() override;
    void setDirty();
    void setClean();
    bool isDirty() const;

private:    
    RenderBackendService *mRbSrv;
    bool mDirty;
    
    struct DrawCmd {
        PrimitiveType mPrimType;
        size_t NumVertices;
        ColorVert *Vertices;
    };
    using DrawCmdArray = cppcore::TArray<DrawCmd>;
    DrawCmdArray mDrawCmdArray;
    glm::vec3 mPenColor;
    Rect2i mResolution;
    i32 mActiveLayer;
    i32 mNumLayers;
};

inline void CanvasRenderer::setDirty() {
    mDirty = true;
}

inline void CanvasRenderer::setClean() {
    mDirty = false;
}

inline bool CanvasRenderer::isDirty() const {
    return mDirty;
}
 
} // namespace RenderBackend
} // namespace OSRE
