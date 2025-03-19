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
#include "App/OrbitalMouseControl.h"
#include "App/MouseEventListener.h"

namespace OSRE::App {

using namespace ::OSRE::RenderBackend;

OrbitalMouseControl::OrbitalMouseControl(TransformMatrixBlock *tmb) :
        mMousePos(), mLastMousePos(), mTransformMatrix(tmb) {
    // empty
}

void OrbitalMouseControl::zoom() {
    glm::mat4 localModel = glm::mat4(1.0);
    const int diff = (mMousePos.y - mLastMousePos.y);
    if (diff != 0) {
        const f32 scaleFactor = 1.0 + (diff * 0.01);

        glm::vec3 s(scaleFactor, scaleFactor, scaleFactor);
        localModel = scale(localModel, s);
        mTransformMatrix->mModel *= localModel;
    }
}

void OrbitalMouseControl::update(MouseEventListener *mouseListener, const glm::vec3 &right, const glm::vec3 &up) {
    if (mouseListener == nullptr) {
        return;
    }
    if (mTransformMatrix == nullptr) {
        return;
    }
    mMousePos.x = mouseListener->getAbsoluteX();
    mMousePos.y = mouseListener->getAbsoluteY();

    glm::mat4 localModel = glm::mat4(1.0);
    if (mouseListener->leftButtonPressed()) {
        const int diffX = (mMousePos.x - mLastMousePos.x);
        const int diffY = (mMousePos.y - mLastMousePos.y);
        if (diffX != 0) {
            const f32 angle = glm::radians(((f32)diffX)/6);
            localModel = rotate(localModel, angle, right);
        }

        if (diffY != 0) {
            const f32 angle = glm::radians(((f32)diffY)/6);
            localModel = rotate(localModel, angle, up);
        }
        mTransformMatrix->mModel *= localModel;
    }

    if (mouseListener->middleButtonPressed()) {
        zoom();
    }

    mLastMousePos.x = mMousePos.x;
    mLastMousePos.y = mMousePos.y;
}

} // namespace OSRE::App
