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

#include "Common/osre_common.h"
#include "Common/glm_common.h"

namespace OSRE {
namespace RenderBackend {

///	@brief  This class is used to describe the global transformation matrices.
/// - You can get and set each matrix (model, view, projection, normal or the computed MVP).
/// - You can use these matrices in your shader.
struct OSRE_EXPORT TransformMatrixBlock {
    glm::mat4 mProjection;
    glm::mat4 mModel;
    glm::mat4 mView;
    glm::mat4 mNormal;
    glm::mat4 mMvp;

    ///	@brief The class constructor.
    TransformMatrixBlock();
    
    ///	@brief The class destructor.
    ~TransformMatrixBlock() = default;
    
    ///	@brief Will init the data, all matrixes ar unit-matrices again.
    void init();
    
    /// @brief All matrices will be recomputed.
    void update();
    
    /// @brief The model matrix getter.
    /// @return const reference to the model matrix.
    const glm::mat4 &getModel() const;
    
    /// @brief The view matrix getter.
    /// @return const reference to the view matrix.
    const glm::mat4 &getView() const;

    /// @brief The projection matrix getter.
    /// @return const reference to the projection matrix.
    const glm::mat4 &getProjection() const;

    /// @brief The model matrix getter.
    /// @return Pointer to the model matrix.
    const float *getModelPtr() const;

    /// @brief The view matrix getter.
    /// @return Pointer to the view matrix.
    const float *getViewPtr() const;

    /// @brief The projection matrix getter.
    /// @return Pointer to the projection matrix.
    const float *getProjectionPtr() const;

    /// @brief The model-view-projection matrix getter.
    /// @return Pointer to the model-view-projection matrix.
    const float *getMVP();

    OSRE_NON_COPYABLE(TransformMatrixBlock)
};

} // namespace RenderBackend
} // namespace OSRE
