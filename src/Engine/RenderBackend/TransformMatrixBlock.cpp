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
#include <osre/RenderBackend/TransformMatrixBlock.h>

namespace OSRE {
namespace RenderBackend {

TransformMatrixBlock::TransformMatrixBlock() :
        m_projection(1.0f),
        m_model(1.0f),
        m_view(1.0f),
        m_normal(1.0f),
        m_mvp(1.0f) {
    init();
}

TransformMatrixBlock::~TransformMatrixBlock() {
    // empty
}

void TransformMatrixBlock::init() {
    m_projection = glm::mat4(1.0f);
    m_model = glm::mat4(1.0f);
    m_view = glm::mat4(1.0f);
    m_normal = glm::mat4(1.0f);
    m_mvp = glm::mat4(1.0f);
}

void TransformMatrixBlock::update() {
    const glm::mat4 modelView = m_view * m_model;
    m_normal = transpose(inverse(modelView));
}

const glm::mat4 &TransformMatrixBlock::getModel() const {
    return m_model;
}

const glm::mat4 &TransformMatrixBlock::getView() const {
    return m_view;
}

const glm::mat4 &TransformMatrixBlock::getProjection() const {
    return m_projection;
}

const float *TransformMatrixBlock::getModelPtr() const {
    return glm::value_ptr(m_model);
}

const float *TransformMatrixBlock::getViewPtr() const {
    return glm::value_ptr(m_view);
}

const float *TransformMatrixBlock::getProjectionPtr() const {
    return glm::value_ptr(m_projection);
}

const float *TransformMatrixBlock::getMVP() {
    const glm::mat4 modelView = m_view * m_model;
    m_mvp = m_projection * modelView;
    return glm::value_ptr(m_mvp);
}


} // namespace RenderBackend
} // namespace OSRE
