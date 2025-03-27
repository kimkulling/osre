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

#include "RenderBackend/TransformMatrixBlock.h"
#include "Common/BaseMath.h"

namespace OSRE::App {

// Forward declarations ---------------------------------------------------------------------------
class MouseEventListener;

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief This class implements a simle orbital mouse control for a camera view.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT OrbitalMouseControl {
public:
    /// @brief The class constructor.
    /// @param[in] tmb    The transfor matrix block as the interface to the camera.
    explicit OrbitalMouseControl(RenderBackend::TransformMatrixBlock *tmb);

    /// @brief The class destructor.
    OrbitalMouseControl() = default;

    /// @brief  Will zoom the camera.
    void zoom();

    /// @brief  Will update the camera.
    /// @param[in] mouseListener    The mouse input listener.
    /// @param[in] right            The right orientation of the camera.
    /// @param[in] up               Thee up orientation of the camera.
    void update(MouseEventListener *mouseListener, const glm::vec3 &right, const glm::vec3 &up);

private:
    glm::i32vec2 mMousePos;
    glm::i32vec2 mLastMousePos;
    RenderBackend::TransformMatrixBlock *mTransformMatrix = nullptr;
};

}
