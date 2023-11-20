/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>
#include <osre/Common/Ids.h>
#include <osre/Common/Object.h>
#include <osre/Common/glm_common.h>
#include <osre/App/TransformComponent.h>
#include <osre/Scene/SceneCommon.h>
#include <osre/Common/TAABB.h>

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
    /// @param[in] fov
    /// @param[in] aspectRatio
    /// @param[in] nearPlane
    /// @param[in] farPlane
    void setProjectionMode(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane);

    /// @brief
    /// @param[in]
    /// @param[in]
    /// @param[in]
    /// @param[in]
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

    /// @brief
    /// @return 
    f32 getAspectRatio() const;

    /// @brief
    /// @return 
    f32 getNear() const;

    /// @brief
    f32 getFar() const;

    /// @brief
    const glm::vec3 &getEye() const;

    /// @brief
    const glm::vec3 &getCenter() const;

    /// @brief
    const glm::vec3 &getUp() const;

protected:
    bool onUpdate(Time dt) override;
    bool onRender(RenderBackend::RenderBackendService *renderBackendSrv) override;

private:
    bool mRecalculateRequested;
    CameraModel mCameraModel;
    f32 m_fov;
    TResolution<f32> mResolution;
    f32 m_near, m_far;
    f32 m_aspectRatio;
    f32 m_left, m_right, m_top, m_bottom;
    glm::vec3 m_eye, m_center, m_up;
    glm::mat4 m_view, m_projection;
};

inline CameraModel CameraComponent::getCameraModel() const {
    return mCameraModel;
}

inline void CameraComponent::setEyePos(const glm::vec3 &eyePosistion) {
    m_eye = eyePosistion;
}

inline f32 CameraComponent::getFov() const {
    return m_fov;
}

inline f32 CameraComponent::getAspectRatio() const {
    return m_aspectRatio;
}

inline f32 CameraComponent::getNear() const {
    return m_near;
}

inline f32 CameraComponent::getFar() const {
    return m_far;
}

inline const glm::vec3 &CameraComponent::getEye() const {
    return m_eye;
}

inline const glm::vec3 &CameraComponent::getCenter() const {
    return m_center;
}

inline const glm::vec3 &CameraComponent::getUp() const {
    return m_up;
}

} // Namespace App
} // Namespace OSRE
