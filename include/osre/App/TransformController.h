#pragma once

#include <osre/Animation/AnimatorBase.h>
#include <osre/Platform/PlatformCommon.h>
#include <osre/Platform/KeyTypes.h>

namespace OSRE {

namespace RenderBackend {
    struct TransformMatrixBlock;
} 

namespace App {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the default keyboard controlling.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT TransformController : public Animation::AnimationControllerBase {
public:
    TransformController(RenderBackend::TransformMatrixBlock &tmb);
    ~TransformController() override;
    static Animation::TransformCommandType getKeyBinding(Platform::Key key);
    static Animation::TransformCommandType getMouseBinding();
    void update(Animation::TransformCommandType cmdType) override;

private:
    RenderBackend::TransformMatrixBlock &mTransform;
};

}
}
