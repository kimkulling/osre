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
#include "Common/Ids.h"
#include "Common/Object.h"
#include "Common/glm_common.h"
#include "App/TransformComponent.h"
#include "App/SceneCommon.h"
#include "Common/TAABB.h"

namespace OSRE {
namespace App {

/// @brief This enum describes the used camera model.
enum class CameraModel {
    InvalidModel = -1,
    Perspective = 0,
    Orthogonal= 1,
    NumModels
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class declares a base camera node within a scene-graph.
/// There are two camera options supported:
/// 1. Perspective camera model
/// 2. Orthogonal camera model
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT CameraComponent : public Component {
public:
    /// @brief  The class constructor.
    /// @param[in] name        [in] The name for the camera node instance.
    /// @param[in] ids         [in] The id container, for unique ids.
    /// @param[in] parent              [in] The parent node, nullptr for a root node.
    CameraComponent(Entity *owner);

    /// @brief The default class destructor, default.
    ~CameraComponent() override = default;

    /// @brief  Will set the projection parameter.
    /// @param[in] fov         The file of view, describes the angle of the view frustum.
    /// @param[in] w           The width of the viewport.
    /// @param[in] h           The height of the viewport.
    /// @param[in] nearPlane   The distance to the near plane of the view frustum.
    /// @param[in] farPlane    The distance to the far plane of the view frustum.
    void setProjectionParameters(f32 fov, f32 w, f32 h, f32 nearPlane, f32 farPlane);

    ///	@brief Will set the camera model.
    /// @param cm 
    void setCameraModel(CameraModel cm);

    /// @brief  Will return the camera model.
    /// @return The current camera model.
    CameraModel getCameraModel() const;

    /// @brief  Will update the camera node once per frame.
    /// @param  dt  [in] The time tick diff since the last tick.
    void update(Time dt) override;

    /// @brief The render callback.
    /// @param renderBackendSrv     [in] The RenderBackend-Server.
    void render(RenderBackend::RenderBackendService *renderBackendSrv);

    /// @brief  Will rearrange the camera view parameters, so that the given bounding box will be observed.
    /// @param[in]  box     The bounding box to observe.
    void observeBoundingBox(const Common::AABB &box);

    /// @brief  Will set the new lookAt parameters.
    /// @param[in] eyePosition  The new eye position.
    /// @param[in] center       The center to look at.
    /// @param[in] up           The up vector of the camera.
    void setLookAt(const glm::vec3 &eyePosition, const glm::vec3 &center, const glm::vec3 &up);

    /// @brief Will set the new eye position.
    /// @param[in] eyePosition  The new eye position.
    void setEyePos(const glm::vec3 &eyePosistion);

    /// @brief  Will set the new projection mode parameter.
    /// @param[in] fov         The fiel-of-view.
    /// @param[in] aspectRatio The aspect ratio.
    /// @param[in] nearPlane   The distance to the near plance.
    /// @param[in] farPlane    The distance to the far plane.
    void setProjectionMode(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane);

    /// @brief Will set the camera parameters for an ortho camera model.
    /// @param[in] left   The left corner.
    /// @param[in] right  The right corner.
    /// @param[in] bottom The bottom corner.
    /// @param[in] top    The top corner.
    void setOrthoMode(f32 left, f32 right, f32 bottom, f32 top, f32 nearPlane, f32 farPlane);

    /// @brief  Will return the view matrix.
    /// @return The view matrix.
    const glm::mat4 &getView() const;

    /// @brief  Will return the projection matrix.
    /// @return The projection matrix.
    const glm::mat4 &getProjection() const;

    /// @brief  Will return the field of view.
    /// @return The field of view.
    f32 getFov() const;

    /// @brief Will reutnr a aspects ratio
    /// @return The aspect ratio.
    f32 getAspectRatio() const;

    /// @brief  Will return the near distance of the view frustum.
    /// @return The near distance of the view frustum.
    f32 getNear() const;

    /// @brief  Will return the far distance of the view frustum.
    /// @return The far distance of the view frustum.
    f32 getFar() const;

    /// @brief  Will return the eye vector.
    /// @return The exe vector.
    const glm::vec3 &getEye() const;

    /// @brief  Will return the position / center vector.
    /// @return The position / center vector.
    const glm::vec3 &getCenter() const;

    /// @brief Will reutrn the up vector.
    /// @return the up-vector.
    const glm::vec3 &getUp() const;

protected:
    bool onUpdate(Time dt) override;
    bool onRender(RenderBackend::RenderBackendService *renderBackendSrv) override;

private:
    bool mRecalculateRequested;
    CameraModel mCameraModel;
    f32 mFOV;
    TResolution<f32> mResolution;
    f32 mNear, mFar;
    f32 mAspectRatio;
    f32 mLeft, mRight, mTop, mBottom;
    glm::vec3 mEye, mCenter, mUp;
    glm::mat4 mView, mProjection;
};

inline CameraModel CameraComponent::getCameraModel() const {
    return mCameraModel;
}

inline void CameraComponent::setEyePos(const glm::vec3 &eyePosistion) {
    mEye = eyePosistion;
}

inline f32 CameraComponent::getFov() const {
    return mFOV;
}

inline f32 CameraComponent::getAspectRatio() const {
    return mAspectRatio;
}

inline f32 CameraComponent::getNear() const {
    return mNear;
}

inline f32 CameraComponent::getFar() const {
    return mFar;
}

inline const glm::vec3 &CameraComponent::getEye() const {
    return mEye;
}

inline const glm::vec3 &CameraComponent::getCenter() const {
    return mCenter;
}

inline const glm::vec3 &CameraComponent::getUp() const {
    return mUp;
}

} // Namespace App
} // Namespace OSRE
