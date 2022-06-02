#include <osre/Common/osre_common.h>
#include <osre/App/TransformController.h>
#include <osre/App/AppCommon.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <OSRE/RenderBackend/TransformMatrixBlock.h>

#include "src//Engine/App/MouseEventListener.h"

namespace OSRE {
namespace App {

using namespace ::OSRE::Animation;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Platform;

TransformController::TransformController(TransformMatrixBlock &tmb) :
        mTransform(tmb) {
    // empty
}

TransformController::~TransformController() {
    // empty
}

TransformCommandType TransformController::getKeyBinding(Key key) {
    switch (key) {
        case Platform::KEY_A:
            return TransformCommandType::RotateXCommandPositive;
        case Platform::KEY_D:
            return TransformCommandType::RotateXCommandNegative;
        case Platform::KEY_W:
            return TransformCommandType::RotateYCommandPositive;
        case Platform::KEY_S:
            return TransformCommandType::RotateYCommandNegative;
        case Platform::KEY_Q:
            return TransformCommandType::RotateZCommandPositive;
        case Platform::KEY_E:
            return TransformCommandType::RotateZCommandNegative;
        case Platform::KEY_PLUS:
            return TransformCommandType::ScaleInCommand;
        case Platform::KEY_MINUS:
            return TransformCommandType::ScaleOutCommand;
        default:
            break;
    }

    return TransformCommandType::InvalidCommand;
}

void TransformController::getMouseUpdate(const MouseInputState &mis) {
    f32 dirX = 0.0f, dirY = 0.0f;
    if (mis.mRelX != 0 || mis.mRelY != 0) {
        dirX = (f32)mis.mRelX;
        dirY = (f32)mis.mRelY;
        f32 len = sqrt(dirX * dirX + dirY * dirY);
        if (mis.mMouseButtonState.getBit(MouseEventListener::LeftButton)) {
            mTransform.m_model = glm::rotate(mTransform.m_model, 0.005f * len, glm::vec3(dirX, dirY, 1.0f));
        }
    }
}

void TransformController::update(TransformCommandType cmdType) {
    glm::mat4 rot(1.0);
    if (cmdType == TransformCommandType::RotateXCommandPositive) {
        mTransform.m_model *= glm::rotate(rot, 0.01f, glm::vec3(1, 0, 0));
    }

    if (cmdType == TransformCommandType::RotateXCommandNegative) {
        mTransform.m_model *= glm::rotate(rot, -0.01f, glm::vec3(1, 0, 0));
    }

    if (cmdType == TransformCommandType::RotateYCommandPositive) {
        mTransform.m_model *= glm::rotate(rot, 0.01f, glm::vec3(0, 1, 0));
    }

    if (cmdType == TransformCommandType::RotateYCommandNegative) {
        mTransform.m_model *= glm::rotate(rot, -0.01f, glm::vec3(0, 1, 0));
    }

    if (cmdType == TransformCommandType::RotateZCommandNegative) {
        mTransform.m_model *= glm::rotate(rot, -0.01f, glm::vec3(0, 0, 1));
    }

    if (cmdType == TransformCommandType::RotateZCommandPositive) {
        mTransform.m_model *= glm::rotate(rot, 0.01f, glm::vec3(0, 0, 1));
    }

    glm::mat4 scale(1.0);
    if (cmdType == TransformCommandType::ScaleInCommand) {
        mTransform.m_model *= glm::scale(scale, glm::vec3(1.01, 1.01, 1.01));
    }

    if (cmdType == TransformCommandType::ScaleOutCommand) {
        mTransform.m_model *= glm::scale(scale, glm::vec3(0.99, 0.99, 0.99));
    }
}

}
}
