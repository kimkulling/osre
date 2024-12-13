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
#include "Common/osre_common.h"
#include "App/TransformController.h"
#include "App/AppCommon.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/TransformMatrixBlock.h"
#include "App/MouseEventListener.h"

namespace OSRE {
namespace App {

using namespace ::OSRE::Animation;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Platform;

static constexpr c8 Tag[] = "TransformController";

TransformController::TransformController(TransformMatrixBlock &tmb) :
        mTransform(tmb) {
    mInputMap.setDefault();
}

TransformCommandType TransformController::getKeyBinding(Key key) const {
    return mInputMap.get(key);
}

void mapToSphere(const glm::vec2 &newPt, glm::vec3 *newVector, f32 w, f32 h, f32 r) {
    // copy parameter into temp point
    glm::vec2 tempPt(newPt);

    // adjust point coordinates and scale down to range of [-1 ... 1]
    f32 x = (tempPt.x * w) - 1.0f;
    f32 y = tempPt.y * h;
    tempPt.x = x;
    tempPt.y = y;

    // compute the square of the length of the vector to the point from the center
    f32 length = (tempPt.x * tempPt.x) + (tempPt.y * tempPt.y);

    *newVector = glm::vec3(tempPt.x, tempPt.y, sqrt(length));

    if (length > r) {
        // compute a normalizing factor (radius / sqrt(length))
        f32 norm = r / sqrt(length);

        // return the "normalized" vector, a point on the sphere
        *newVector = glm::vec3(tempPt.x * norm, tempPt.y * norm, 0.0f);
    } else { // else it's on the inside
        // return a vector to a point mapped inside the sphere sqrt(radius squared - length)
        *newVector = glm::vec3(tempPt.x, tempPt.y, sqrt(r - length));
    }
}

void TransformController::getMouseUpdate(const MouseInputState &mis) {
    f32 dirX = 0.0f, dirY = 0.0f;
    if (mis.RelX != 0 || mis.RelY != 0) {
        dirX = (f32)mis.RelX;
        dirY = (f32)mis.RelY;
        f32 len = sqrt(dirX * dirX + dirY * dirY);
        if (mis.MouseButtonState.getBit(MouseEventListener::LeftButton)) {
            glm::vec3 res;
            mapToSphere(glm::vec2(dirY, dirX), &res, 1000, 768, 1);
            mTransform.mModel = glm::rotate(mTransform.mModel, 0.005f * len, res);
        }
        if (mis.MouseButtonState.getBit(MouseEventListener::MiddleButton)) {
            glm::vec3 res;
            mapToSphere(glm::vec2(dirY, dirX), &res, 1000, 768, 1);
            mTransform.mModel = glm::translate(mTransform.mModel, res);
        }
        if (mis.MouseButtonState.getBit(MouseEventListener::RightButton)) {
            f32 factor = 0.005f * len;
            if (dirY > 0) {
                mTransform.mModel = glm::scale(mTransform.mModel, glm::vec3(1.01 + factor, 1.01 + factor, 1.01 + factor));
            } else {
                mTransform.mModel = glm::scale(mTransform.mModel, glm::vec3(0.99 - factor, 0.99 - factor, 0.99 - factor));
            }
        }
    }
}

void TransformController::update(TransformCommandType cmdType) {
    glm::mat4 rot(1.0);
    switch (cmdType) {
        case TransformCommandType::RotateXCommandPositive:
            mTransform.mModel *= glm::rotate(rot, mTransformConfig.mRotateFactor, glm::vec3(1, 0, 0));
            break;
        case TransformCommandType::RotateXCommandNegative:
            mTransform.mModel *= glm::rotate(rot, -mTransformConfig.mRotateFactor, glm::vec3(1, 0, 0));
            break;
        case TransformCommandType::RotateYCommandPositive:
            mTransform.mModel *= glm::rotate(rot, mTransformConfig.mRotateFactor, glm::vec3(0, 1, 0));
            break;
        case TransformCommandType::RotateYCommandNegative:
            mTransform.mModel *= glm::rotate(rot, -mTransformConfig.mRotateFactor, glm::vec3(0, 1, 0));
            break;
        case TransformCommandType::RotateZCommandNegative:
            mTransform.mModel *= glm::rotate(rot, -mTransformConfig.mRotateFactor, glm::vec3(0, 0, 1));
            break;
        case TransformCommandType::RotateZCommandPositive:
            mTransform.mModel *= glm::rotate(rot, mTransformConfig.mRotateFactor, glm::vec3(0, 0, 1));
            break;
        case TransformCommandType::ScaleInCommand:
            mTransform.mModel *= glm::scale(rot, glm::vec3(mTransformConfig.mScaleFactorPlus, mTransformConfig.mScaleFactorPlus, mTransformConfig.mScaleFactorPlus));
            break;
        case TransformCommandType::ScaleOutCommand:
            mTransform.mModel *= glm::scale(rot, glm::vec3(mTransformConfig.mScaleFactorMinus, mTransformConfig.mScaleFactorMinus, mTransformConfig.mScaleFactorMinus));
            break;
        case TransformCommandType::TransformCommandXPositive:
            mTransform.mModel = glm::translate(mTransform.mModel, glm::vec3(mTransformConfig.mTranslateFactor, 0, 0));
            break;
        case TransformCommandType::TransformCommandXNegative:
            mTransform.mModel = glm::translate(mTransform.mModel, glm::vec3(-mTransformConfig.mTranslateFactor, 0, 0));
            break;
        case TransformCommandType::TransformCommandYPositive:
            mTransform.mModel = glm::translate(mTransform.mModel, glm::vec3(0, mTransformConfig.mTranslateFactor, 0));
            break;
        case TransformCommandType::TransformCommandYNegative:
            mTransform.mModel = glm::translate(mTransform.mModel, glm::vec3(0, -mTransformConfig.mTranslateFactor, 0));
            break;

        case TransformCommandType::TransformCommandZPositive:
        case TransformCommandType::TransformCommandZNegative:
            // ignored
            break;  
        case TransformCommandType::Invalid:
            break;

        default:
            osre_warn(Tag, "Invalid tag");
            printf("Tag: %d\n", static_cast<int>(cmdType));
            break;
    }
}

} // namespace App
} // namespace OSRE
