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

#include <osre/Common/osre_common.h>
#include <osre/Common/glm_common.h>

namespace OSRE {
namespace RenderBackend {

///	@brief
struct OSRE_EXPORT TransformMatrixBlock {
    glm::mat4 m_projection;
    glm::mat4 m_model;
    glm::mat4 m_view;
    glm::mat4 m_normal;
    glm::mat4 m_mvp;

    TransformMatrixBlock();
    ~TransformMatrixBlock();
    void init();
    void update();
    const glm::mat4 &getModel() const;
    const glm::mat4 &getView() const;
    const glm::mat4 &getProjection() const;
    const float *getModelPtr() const;
    const float *getViewPtr() const;
    const float *getProjectionPtr() const;
    const float *TransformMatrixBlock::getMVP();

    OSRE_NON_COPYABLE(TransformMatrixBlock)
};

} // namespace RenderBackend
} // namespace OSRE
