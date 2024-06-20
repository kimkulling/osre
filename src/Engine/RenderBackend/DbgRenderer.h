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
#pragma once

#include <cppcore/Container/TArray.h>
#include <cppcore/Container/THashMap.h>
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/TransformMatrixBlock.h"
#include "Common/TAABB.h"

namespace OSRE {
namespace RenderBackend {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT DbgRenderer {
    struct DebugText;

public:
    void render();
    void renderDbgText(ui32 x, ui32 y, guid id, const String &text);
    void renderAABB(const glm::mat4 &transform, const Common::AABB &aabb);
    void clear();
    void addLine(const RenderBackend::ColorVert &v0, const RenderBackend::ColorVert &v1);
    static bool create(RenderBackend::RenderBackendService *rbSrv);
    static bool destroy();
    static DbgRenderer *getInstance();
    static const c8 *getDebugRenderBatchName();

private:
    DbgRenderer(RenderBackend::RenderBackendService *rbSrv);
    ~DbgRenderer();
    DebugText *getDebugText(guid id) const;

private:
    static DbgRenderer *sInstance;

    RenderBackend::RenderBackendService *mRbSrv;
    RenderBackend::TransformMatrixBlock mTransformMatrix;
    RenderBackend::Mesh *mDebugMesh;
    
    struct DebugText {
        RenderBackend::Mesh *mesh;
        String text;
    };

    cppcore::TArray<DebugText*> mDebugTextMeshes;
    ui16 mLastIndex;
};

} // Namespace RenderBackend
} // namespace OSRE
