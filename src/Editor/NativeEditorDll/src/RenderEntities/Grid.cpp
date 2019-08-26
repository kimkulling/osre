/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Grid.h"

#include <osre/Scene/LineBuilder.h>
#include <osre/Debugging/osre_debugging.h>

namespace OSRE {
namespace NativeEditor {

using namespace ::OSRE::Scene;
using namespace ::OSRE::RenderBackend;

Grid::Grid( Common::Ids &ids )
: Node( "grid", ids )
, m_mesh( nullptr ) {
    // empty
}

Grid::~Grid() {
    // empty
}

void Grid::onUpdate(Time ) {

}

void Grid::onRender(RenderBackendService *rbSrv ) {
    OSRE_ASSERT(nullptr != rbSrv);

    if (nullptr == m_mesh) {
        createMesh(rbSrv);
    }
}

void Grid::createMesh(RenderBackendService *rbSrv) {
    OSRE_ASSERT(nullptr != rbSrv);

    LineBuilder lineBuilder;
    const ui32 NumLines = 101;

    f32 x0(0.f), y0(0.f), z(0.f);
    f32 x1(1.f), y1(0.f);
    f32 w = 1.f, h=1.f;
    f32 diffCol = w / ( NumLines - 1 );
    f32 diffRow = h / ( NumLines - 1 );
    for (ui32 col = 0; col < NumLines; ++col) {
        y0 += diffCol;
        Vec3f pos0(x0, y0, z);
        Vec3f pos1(x1, y1, z);
        lineBuilder.addLine(pos0, pos1);
    }

    y0 = 0;
    for (ui32 row = 0; row < NumLines; ++row) {
        x0 += diffRow;
        Vec3f pos0(x0, y0, z);
        Vec3f pos1(x1, y1, z);
        lineBuilder.addLine(pos0, pos1);
    }

    m_mesh = lineBuilder.getMesh();
}

}
}
