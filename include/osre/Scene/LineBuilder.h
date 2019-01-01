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

#include <osre/Scene/SceneCommon.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace Scene {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT LineBuilder {
public:
    LineBuilder();
    ~LineBuilder();
    LineBuilder &addLine(const Vec3f &pos0, const Vec3f &pos1);
    LineBuilder &addLines(Vec3f *pos0, Vec3f *pos1, ui32 numLines);
    RenderBackend::Mesh *getMesh();

    /// No copying.
    LineBuilder(const LineBuilder &) = delete;
    LineBuilder& operator = (const LineBuilder &) = delete;

protected:
    void preparePrimGroups();

private:
    CPPCore::TArray<glm::vec3> m_posCache;
    CPPCore::TArray<glm::vec3> m_diffuseColCache;
    CPPCore::TArray<glm::vec3> m_normalCache;
    CPPCore::TArray<glm::vec2> m_tex0Cache;
    RenderBackend::PrimitiveGroup *m_activePrimGroup;
    CPPCore::TArray<ui32> m_indexCache;

    CPPCore::TArray<RenderBackend::PrimitiveGroup*> m_primGroupCache;
    bool m_isDirty;
    RenderBackend::Mesh *m_ActiveGeo;
};

} // Namespace Scene
} // Namespace OSRE
