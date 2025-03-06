/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "App/Entity.h"

namespace OSRE {

namespace RenderBackend {
    class Mesh;
}

namespace App {
    class RenderComponent;
}

namespace Animation {
    struct Skeleton;
}

namespace Editor {

struct Style {
    Color4 fg;
    Color4 bg;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup    Editor
///
/// @brief
//-------------------------------------------------------------------------------------------------
class MainRenderView {
public:
    MainRenderView();
    ~MainRenderView();
    static RenderBackend::Mesh *createCoordAxis(ui32 size);
    static RenderBackend::Mesh *createGrid(ui32 numLines);
    static void createRect2D(const Rect2ui &r, RenderBackend::Mesh *mesh2D, Style &style);
    static void createBonesFromSkeleton(Animation::Skeleton *skeleton);
    void createEditorElements(App::RenderComponent *rc);
    void render(RenderBackend::RenderBackendService *rbSrb, glm::mat4 model);

private:
    RenderBackend::MeshArray mEditorElements;
};

} // namespace Editor
} // namespace OSRE
