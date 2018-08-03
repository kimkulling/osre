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
#pragma once

#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Collision/TAABB.h>
#include <cppcore/Container/THashMap.h>
#include <cppcore/Container/TArray.h>

namespace OSRE {
    
// Forward declarations
namespace RenderBackend {
    class RenderBackendService;

    struct Geometry;
}

namespace Scene {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT DbgRenderer {
public:
    struct DbgTextEntry {
        String m_text;
        RenderBackend::Geometry *m_geo;
    };
    using TextBoxHashMap = CPPCore::THashMap<ui32, DbgTextEntry*>;
    using TextBoxArray = CPPCore::TArray<DbgTextEntry*>;

    void renderDbgText( ui32 x, ui32 y, ui32 id, const String &text );
    void renderAABB( const glm::mat4 &transform, const Collision::TAABB<f32> &aabb );
    void clearDbgTextCache();
    void clearDbgRenderPass();
    ui32 numDbgTexts() const;
    void addLine( const RenderBackend::ColorVert &v0, const RenderBackend::ColorVert &v1 );
    void addTriangle( const RenderBackend::ColorVert &v0, const RenderBackend::ColorVert &v1, const RenderBackend::ColorVert &v2 );
    
    static bool create( RenderBackend::RenderBackendService *rbSrv );
    static bool destroy();
    static DbgRenderer *getInstance();

private:
    DbgRenderer( RenderBackend::RenderBackendService *rbSrv );
    ~DbgRenderer();

private:
    static DbgRenderer *s_instance;

    RenderBackend::RenderBackendService *m_rbSrv;
    RenderBackend::TransformMatrixBlock m_transformMatrix;
    TextBoxHashMap m_textBoxes;
    TextBoxArray m_tbArray;
};

} // Namespace Scene
} // namespace OSRE
