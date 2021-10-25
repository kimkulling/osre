/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/Object.h>
#include <osre/Common/glm_common.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/SceneCommon.h>
#include <osre/Scene/AABB.h>

namespace OSRE {
namespace Scene {

enum class CameraModel {
    Perspective,
    Orthogonal
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class declares a base camera node within a scene-graph.
/// There are two camera options supported:
/// 1. Perspective camera model
/// 2. Orthogonal camera model
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Camera : public Node {
public:
    /// @brief  The class constructor.
    /// @param  name        [in] The name for the camera node instance.
    /// @param  ids         [in] The id container, for unique ids.
    /// parent              [in] The parent node, nullptr for a root node.
    Camera(const String &name, Common::Ids &ids, Node *parent = nullptr);

    /// @brief The class destructor.
    ~Camera() override;

    /// @brief  Will set the projection parameter.
    /// @param  fov         [in] The file of view, describes the angle of the view frustum.
    /// @param  w           [in] The width of the viewport.
    /// @param  h           [in] The height of the viewport.
    /// @param  nearPlane   [in] The distance to the near plane of the view frustum.
    /// @param  farPlane    [in] The distance to the far plane of the view frustum.
    void setProjectionParameters(f32 fov, f32 w, f32 h, f32 nearPlane, f32 farPlane);

    ///	@brief
    /// @param cm 
    void setCameraModel(CameraModel cm);

    /// @brief  Will return the camera model.
    /// @return The current camera model.
    CameraModel getCameraModel() const;

    /// @brief  Will update the camera node once per frame.
    /// @param  dt  [in] The time tick diff since the last tick.
    void update(Time dt) override;

    /// @brief The draw callback.
    /// @param renderBackendSrv     [in] The RenderBackend-Server.
    void draw(RenderBackend::RenderBackendService *renderBackendSrv);

    /// @brief
    void observeBoundingBox(const TAABB<f32> &box);

    /// @brief
    void setLookAt(const glm::vec3 &eyePosition, const glm::vec3 &center, const glm::vec3 &up);

    void setEyePos(const glm::vec3 &eyePosistion);

    /// @brief
    void setProjectionMode(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane);

    /// @brief
    void setOrthoMode(f32 left, f32 right, f32 bottom, f32 top, f32 nearPlane, f32 farPlane);

    /// @brief
    const glm::mat4 &getView() const;

    /// @brief
    const glm::mat4 &getProjection() const;

    /// @brief
    f32 getFov() const;

    /// @brief
    f32 getAspectRatio() const;

    /// @brief
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
    void onUpdate(Time dt) override;
    void onRender(RenderBackend::RenderBackendService *renderBackendSrv) override;

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

inline void Camera::setCameraModel(CameraModel cm) {
    mCameraModel = cm;
}

inline CameraModel Camera::getCameraModel() const {
    return mCameraModel;
}

inline void Camera::setEyePos(const glm::vec3 &eyePosistion) {
    m_eye = eyePosistion;
}

inline f32 Camera::getFov() const {
    return m_fov;
}

inline f32 Camera::getAspectRatio() const {
    return m_aspectRatio;
}

inline f32 Camera::getNear() const {
    return m_near;
}

inline f32 Camera::getFar() const {
    return m_far;
}

inline const glm::vec3 &Camera::getEye() const {
    return m_eye;
}

inline const glm::vec3 &Camera::getCenter() const {
    return m_center;
}

inline const glm::vec3 &Camera::getUp() const {
    return m_up;
}

} // Namespace Scene
} // Namespace OSRE
