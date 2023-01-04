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
    ///	@brief  The default class constructor.
    LineBuilder();

    ///	@brief  The class destructor, default generated.
    ~LineBuilder() = default;

    ///	@brief  Will add a new line element to the active mesh.
    /// 
    /// The form p0 --- p1 will be used.
    /// 
    /// @param[in] pos0     Edge 0
    /// @param[in] pos1     Edge 1
    /// @return The line builder reference.
    LineBuilder &addLine(const glm::vec3 &pos0, const glm::vec3 &pos1);

    /// @brief Will add a bundle of line-segments.
    ///
    /// The form p0 --- p1 will be used.
    ///
    /// @param pos0     Array with pos0 edges
    /// @param pos1     Array with pos1 edges.
    /// @param numLines The number of lines.
    /// @return The line builder reference.
    LineBuilder &addLines(glm::vec3 *pos0, glm::vec3 *pos1, ui32 numLines);

    /// @brief  Will return the active mesh or nullptr when no mesh is active
    /// @return The pointer showing to the active mesh. The ownership will be moved to the 
    ///         customer of the active mesh.
    RenderBackend::Mesh *getMesh();

    /// No copying.
    LineBuilder(const LineBuilder &) = delete;
    LineBuilder& operator = (const LineBuilder &) = delete;

protected:
    void preparePrimGroups();

private:
    cppcore::TArray<glm::vec3> m_posCache;
    cppcore::TArray<glm::vec3> m_diffuseColCache;
    cppcore::TArray<glm::vec3> m_normalCache;
    cppcore::TArray<glm::vec2> m_tex0Cache;
    RenderBackend::PrimitiveGroup *m_activePrimGroup;
    cppcore::TArray<ui32> m_indexCache;

    cppcore::TArray<RenderBackend::PrimitiveGroup*> m_primGroupCache;
    bool m_isDirty;
    RenderBackend::Mesh *m_activeMesh;
};

} // Namespace Scene
} // Namespace OSRE
