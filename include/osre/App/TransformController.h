#pragma once

#include <osre/Animation/AnimatorBase.h>
#include <osre/Platform/PlatformCommon.h>
#include <osre/Platform/KeyTypes.h>

namespace OSRE {

namespace RenderBackend {
struct TransformMatrixBlock;
}

namespace App {

struct MouseInputState;

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
    void getMouseUpdate(const MouseInputState &mis) override;
    void update(Animation::TransformCommandType cmdType) override;
    void scaleAll();

private:
    RenderBackend::TransformMatrixBlock &mTransform;
};

} // namespace App
} // namespace OSRE
