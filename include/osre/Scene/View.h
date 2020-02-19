/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Scene/Node.h>
#include <osre/Scene/SceneCommon.h>
#include <osre/Scene/TAABB.h>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace OSRE {
namespace Scene {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT View : public Node {
public:
    View(const String &name, Common::Ids &ids, Node *parent = nullptr);
    ~View() override;
    void setProjectionParameters(f32 fov, f32 w, f32 h, f32 near, f32 far);
    void update(Time dt);
    void draw(RenderBackend::RenderBackendService *renderBackendSrv);
    void observeBoundingBox(const TAABB<f32> &box);
    void setLookAt(const glm::vec3 &pos, const glm::vec3 &view, const glm::vec3 &up);
    void setProjectionMode(f32 fov, f32 aspectRatio, f32 near, f32 far);
    void setOrthoMode(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);
    const glm::mat4 &getView() const;
    const glm::mat4 &getProjection() const;
    f32 getFov() const;
    f32 getAspectRatio() const;
    f32 getNear() const;
    f32 getFar() const;

protected:
    virtual void onUpdate(Time dt);
    virtual void onRender(RenderBackend::RenderBackendService *renderBackendSrv);

private:
    f32 m_fov;
    f32 m_w, m_h;
    f32 m_near, m_far;
    f32 m_aspectRatio;
    Node *m_observedNode;
    glm::vec3 m_eye, m_center, m_up;
    glm::mat4 m_view, m_projection;
};

inline f32 View::getFov() const {
    return m_fov;
}

inline f32 View::getAspectRatio() const {
    return m_aspectRatio;
}

inline f32 View::getNear() const {
    return m_near;
}

inline f32 View::getFar() const {
    return m_far;
}

} // Namespace Scene
} // Namespace OSRE
