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
#include <osre/Common/glm_common.h>

namespace OSRE {
namespace Common {

class Frustum {
public:
    enum {
        Top = 0,
        Bottom, 
        Left,
        Right,
        NearP, 
        FarP
    };

    Frustum();
    ~Frustum() = default;
    bool isIn(glm::vec3 &point);
    void extractFrom(const glm::mat4 &vp);
    void clear();

private:
    glm::vec4 mPlanes[6];
};

inline Frustum::Frustum() {
    clear();
}

inline bool Frustum::isIn( glm::vec3 &point ) {
    bool in = true;
    for (auto & plane : mPlanes) {
        const f32 d = plane.x * point.x + plane.y * point.y + plane.z * point.z + plane.w;
        if (d < 0.0f) {
            in = false;
            break;
        }
    }
    
    return in;
}

inline void Frustum::extractFrom(const glm::mat4 &vp) {
    glm::vec4 rowX = glm::row(vp, 0);
    glm::vec4 rowY = glm::row(vp, 1);
    glm::vec4 rowZ = glm::row(vp, 2);
    glm::vec4 rowW = glm::row(vp, 3);

    mPlanes[0] = normalize(rowW + rowX);
    mPlanes[1] = normalize(rowW - rowX);
    mPlanes[2] = normalize(rowW + rowY);
    mPlanes[3] = normalize(rowW - rowY);
    mPlanes[4] = normalize(rowW + rowZ);
    mPlanes[5] = normalize(rowW - rowZ);
}

inline void Frustum::clear() {
    for (auto &plane : mPlanes) {
        plane.x = plane.y = plane.z = plane.w = 0.0f;
    }
}

} // namespace Common
} // namespace OSRE
