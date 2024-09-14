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
#include "RenderBackend/TransformMatrixBlock.h"

namespace OSRE {
namespace RenderBackend {

TransformMatrixBlock::TransformMatrixBlock() {
    init();
}

void TransformMatrixBlock::init() {
    mProjection = glm::mat4(1.0f);
    mModel = glm::mat4(1.0f);
    mView = glm::mat4(1.0f);
    mNormal = glm::mat4(1.0f);
    mMvp = glm::mat4(1.0f);
}

void TransformMatrixBlock::update() {
    const glm::mat4 modelView = mView * mModel;
    mNormal = transpose(inverse(modelView));
}

const glm::mat4 &TransformMatrixBlock::getModel() const {
    return mModel;
}

const glm::mat4 &TransformMatrixBlock::getView() const {
    return mView;
}

const glm::mat4 &TransformMatrixBlock::getProjection() const {
    return mProjection;
}

const float *TransformMatrixBlock::getModelPtr() const {
    return glm::value_ptr(mModel);
}

const float *TransformMatrixBlock::getViewPtr() const {
    return glm::value_ptr(mView);
}

const float *TransformMatrixBlock::getProjectionPtr() const {
    return glm::value_ptr(mProjection);
}

const float *TransformMatrixBlock::getMVP() {
    const glm::mat4 modelView = mView * mModel;
    mMvp = mProjection * modelView;
    return glm::value_ptr(mMvp);
}

} // namespace RenderBackend
} // namespace OSRE
