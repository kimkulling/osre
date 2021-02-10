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
#include <osre/Common/Logger.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Scene/Camera.h>

#include <glm/gtc/matrix_transform.hpp>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::RenderBackend;

static const c8 *Tag = "Camera";

Camera::Camera(const String &name, Common::Ids &ids, Node *parent) :
        Node(name, ids, parent),
        m_fov(60.0f),
        m_w(1.0f),
        m_h(1.0f),
        m_near(0.001f),
        m_far(1000.0f),
        m_aspectRatio(1.0),
        m_observedNode(nullptr),
        m_eye(1, 1, 1),
        m_center(0, 0, 0),
        m_up(0, 0, 1),
        m_view(1),
        m_projection(1) {
    // empty
}

Camera::~Camera() {
    // empty
}

void Camera::setProjectionParameters(f32 fov, f32 w, f32 h, f32 zNear, f32 zFar) {
    m_fov = fov;
    m_w = w;
    m_h = h;
    m_near = zNear;
    m_far = zFar;
    m_aspectRatio = 1.0f;
    if (0.0f != h) {
        m_aspectRatio = w / h;
    }
    m_projection = glm::perspective(glm::radians(m_fov), m_aspectRatio, zNear, zFar);
}

void Camera::update(Time dt) {
    onUpdate(dt);
}

void Camera::draw(RenderBackendService *rbSrv) {
    if (nullptr == rbSrv) {
        return;
    }

    onRender(rbSrv);
}

void Camera::observeBoundingBox(const TAABB<f32> &aabb) {
    const f32 diam = aabb.getDiameter();
    const Vec3f center = aabb.getCenter();

    glm::vec3 eye(-diam / 2, -diam / 2, diam / 2), up(0, 0, 1);
    glm::vec3 c(center.getX(), center.getY(), center.getZ());
    setLookAt(eye, c, up);
}

void Camera::setLookAt(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up) {
    m_eye = eye;
    m_center = center;
    m_up = up;

    m_view = glm::lookAt(eye, center, up);
}

void Camera::setProjectionMode(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane) {
    m_fov = fov;
    m_aspectRatio = aspectRatio;
    m_near = nearPlane;
    m_far = farPlane;
    m_projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
}

void Camera::setOrthoMode(f32 left, f32 right, f32 bottom, f32 top, f32 nearPlane, f32 farPlane) {
    m_w = right - left;
    m_h = bottom - top;

    m_near = nearPlane;
    m_far = farPlane;
    m_projection = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
}

const glm::mat4 &Camera::getView() const {
    return m_view;
}

const glm::mat4 &Camera::getProjection() const {
    return m_projection;
}

void Camera::onUpdate(Time dt) {
    (void)dt;
}

void Camera::onRender(RenderBackendService *rbSrv) {
    if (nullptr == rbSrv) {
        osre_debug(Tag, "Pointer to renderbackend service is nullptr.");
        return;
    }

    rbSrv->setMatrix(MatrixType::View, m_view);
    rbSrv->setMatrix(MatrixType::Projection, m_projection);
}

} // Namespace Scene
} // Namespace OSRE
