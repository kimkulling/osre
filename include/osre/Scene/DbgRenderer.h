/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/TransformMatrixBlock.h>
#include <osre/Scene/SceneCommon.h>
#include <osre/Scene/TAABB.h>

namespace OSRE {

namespace UI {

class FontRenderer;
}

namespace Scene {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT DbgRenderer {
public:
    void renderDbgText(ui32 x, ui32 y, ui32 id, const String &text);
    void renderAABB(const glm::mat4 &transform, const TAABB<f32> &aabb);
    void clear();
    void addLine(const RenderBackend::ColorVert &v0, const RenderBackend::ColorVert &v1);

    static bool create(RenderBackend::RenderBackendService *rbSrv);
    static bool destroy();
    static DbgRenderer *getInstance();

private:
    DbgRenderer(RenderBackend::RenderBackendService *rbSrv);
    ~DbgRenderer();

private:
    static DbgRenderer *s_instance;

    RenderBackend::RenderBackendService *mRbSrv;
    RenderBackend::TransformMatrixBlock mTransformMatrix;
    UI::FontRenderer *mFontRenderer;
    RenderBackend::Mesh *mDebugGeometry;
    ui16 mLastIndex;
};

} // Namespace Scene
} // namespace OSRE
