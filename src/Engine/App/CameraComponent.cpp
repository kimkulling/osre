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
#include "App/CameraComponent.h"
#include "Common/Logger.h"
#include "RenderBackend/RenderBackendService.h"
#include "Common/glm_common.h"

namespace OSRE::App {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::glm;

DECL_OSRE_LOG_MODULE(Camera)

static const String CameraModelName[3] = {
    "Perspective ",
    "Orthogonal",
    "Invalid"
};

static constexpr f32 DefaultNear = 0.001f;
static constexpr f32 DefaultFar = 1000.0f;
static constexpr f32 DefaultAspectRatio = 1.0f;

CameraComponent::CameraComponent(Entity *owner) :
        Component(owner, ComponentType::CameraComponentType),
        mRecalculateRequested(true),
        mCameraModel(CameraModel::Perspective),
        mFOV(60.0f),
        mResolution(1.0f, 1.0f),
        mNear(DefaultNear),
        mFar(DefaultFar),
        mAspectRatio(DefaultAspectRatio),
        mLeft(0.0f),
        mRight(1.0f),
        mTop(0.0f),
        mBottom(1.0f),
        mEye(1, 1, 1),
        mCenter(0, 0, 0),
        mRightVec(0, 1, 0),
        mUp(0, 0, 1),
        mView(1),
        mProjection(1) {
    // empty
}

void CameraComponent::setProjectionParameters(f32 fov, f32 w, f32 h, f32 zNear, f32 zFar) {
    mFOV = glm::radians(fov);
    mResolution.width = w;
    mResolution.height = h;
    mNear = zNear;
    mFar = zFar;
    mAspectRatio = DefaultAspectRatio;
    if (0.0f != h) {
        mAspectRatio = w / h;
    }

    setCameraModel(CameraModel::Perspective);
}

void CameraComponent::update(Time dt) {
    onUpdate(dt);
}

void CameraComponent::render(RenderBackendService *rbSrv) {
    if (nullptr == rbSrv) {
        return;
    }

    onRender(rbSrv);
}

void CameraComponent::observeBoundingBox(const AABB &aabb) {
    f32 diam = aabb.getDiameter();
    const f32 maxDist = mFar - mNear;
    if (diam > maxDist) {
        diam = maxDist - 100.0f;
    }

    const glm::vec3 center = aabb.getCenter();

    glm::vec3 eye(-diam * 0.5f, -diam * 0.5f, diam * 0.5f), up(0, 0, 1);

    setLookAt(eye, center, up);
}

void CameraComponent::setLookAt(const glm::vec3 &eyePosition, const glm::vec3 &center, const glm::vec3 &up) {
    mEye = eyePosition;
    mCenter = center;
    mUp = up;
}

void CameraComponent::setProjectionMode(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane) {
    mFOV = fov;
    mAspectRatio = aspectRatio;
    mNear = nearPlane;
    mFar = farPlane;

    setCameraModel(CameraModel::Perspective);
}

void CameraComponent::setCameraModel(CameraModel cm) {
    osre_trace(Tag, "Set camera model to " + CameraModelName[static_cast<size_t>(cm)]);

    mCameraModel = cm;
}

void CameraComponent::setOrthoMode(f32 left, f32 right, f32 bottom, f32 top, f32 nearPlane, f32 farPlane) {
    mResolution.width = right - left;
    mResolution.height = bottom - top;
    mLeft = left;
    mRight = right;
    mBottom = bottom;
    mTop = top;
    mNear = nearPlane;
    mFar = farPlane;

    setCameraModel(CameraModel::Orthogonal);
}

const glm::mat4 &CameraComponent::getView() const {
    return mView;
}

const glm::mat4 &CameraComponent::getProjection() const {
    return mProjection;
}

bool CameraComponent::onUpdate(Time) {
    const CameraModel cm = getCameraModel();
    if (cm == CameraModel::Perspective) {
        mProjection = glm::perspective(mFOV, mAspectRatio, mNear, mFar);
    } else if (cm == CameraModel::Orthogonal) {
        mProjection = glm::ortho(mLeft, mRight, mBottom, mTop, mNear, mFar);
    }
    mView = glm::lookAt(mEye, mCenter, mUp);
    mViewProjection = mProjection * mView;
    mFrustum.extractFrom(mViewProjection);

    return true;
}

bool CameraComponent::onRender(RenderBackendService *rbSrv) {
    if (nullptr == rbSrv) {
        return false;
    }

    rbSrv->setMatrix(MatrixType::View, mView);
    rbSrv->setMatrix(MatrixType::Projection, mProjection);

    return true;
}

} // namespace OSRE::App