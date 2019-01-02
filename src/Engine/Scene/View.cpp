/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Scene/View.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Common/Logger.h>

#include <glm/gtc/matrix_transform.hpp>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::RenderBackend;

static const c8 *Tag = "View";

View::View(const String &name, Common::Ids &ids, Node *parent )
: Node( name, ids, RenderCompRequest::NoRenderComp, TransformCompRequest::TransformCompRequested, parent )
, m_fov( 60.0f )
, m_w( 0.0f )
, m_h( 0.0f )
, m_far( 1000.0f )
, m_observedNode( nullptr )
, m_eye( 1, 1, 1 )
, m_center( 0, 0, 0 )
, m_up( 0, 0, 1 )
, m_view()
, m_projection() {
    // empty
}

View::~View() {
    // empty
}

void View::setProjectionParameters(f32 fov, f32 w, f32 h, f32 zNear, f32 zFar) {
    m_fov = fov;
    m_w = w;
    m_h = h;

    f32 aspect = 1.0f;
    if (0.0f != h) {
        aspect = w / h;
    }
    m_projection = glm::perspective(glm::radians(m_fov), aspect, zNear, zFar);
}

void View::update( Time dt ) {
    onUpdate( dt );
}

void View::draw( RenderBackendService *rbSrv ) {
    if ( nullptr == rbSrv ) {
        return;
    }
    
    onDraw( rbSrv );
}

void View::observeBoundingBox( const Collision::TAABB<f32> &aabb) {
    const f32 diam = aabb.getDiameter();
    const Vec3f center = aabb.getCenter();

    glm::vec3 eye(diam, 0, 0), up(0, 0, 1);
    glm::vec3 c(center.getX(), center.getY(), center.getZ());
    setLookAt(eye, c, up);
}

void View::setLookAt( const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up ) {
    m_eye    = eye;
    m_center = center;
    m_up     = up;

    m_view = glm::lookAt( eye, center, up );
}

void View::setProjectionMode( f32 fov, f32 aspectRatio, f32 near, f32 far ) {
    m_projection = glm::perspective( fov, aspectRatio, near, far );
}

void View::setOrthoMode( f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far ) {
    m_projection = glm::ortho( left, right, bottom, top, near, far );
}

const glm::mat4 &View::getView() const {
    return m_view;
}

const glm::mat4 &View::getProjection() const {
    return m_projection;
}

void View::onUpdate( Time dt ) {
    // empty
}

void View::onDraw( RenderBackendService *rbSrv ) {
    rbSrv->setMatrix(MatrixType::View, m_view );
    rbSrv->setMatrix(MatrixType::Projection, m_projection);
}

} // Namespace Scene
} // Namespace OSRE

