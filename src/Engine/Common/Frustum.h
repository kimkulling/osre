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
#include "Common/glm_common.h"
#include <cppcore/Container/TStaticArray.h>

namespace OSRE {
namespace Common {

struct Plane {
    glm::vec4 param;

    bool operator==(const Plane &rhs) const {
        return (param == rhs.param);
    }

    bool operator!=(const Plane &rhs) const {
        return (param != rhs.param);
    }
};


//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
/// @brief	This classs implements a view frustum.
//-------------------------------------------------------------------------------------------------
class Frustum {
public:
    enum {
        Invalid = -1,
        Top = 0,
        Bottom, 
        Left,
        Right,
        NearP, 
        FarP,
        Count
    };

    explicit Frustum(const Plane *planes);
    Frustum();
    ~Frustum() = default;
    bool isIn(const glm::vec3 &point);
    void extractFrom(const glm::mat4 &vp);
    void clear();

private:
    cppcore::TStaticArray<Plane, 6> mPlanes;
};

inline Frustum::Frustum(const Plane *planes) {
    if (planes != nullptr) {
        for (ui32 i = 0; i < Count; ++i) {
            mPlanes[i] = planes[i];
        }
    }
}

inline Frustum::Frustum() {
    clear();
}

inline bool Frustum::isIn(const glm::vec3 &point ) {
    bool in = true;
    for (size_t i = 0; i < mPlanes.size(); ++i) {
        const Plane &plane = mPlanes[i];
        const f32 d = plane.param.x * point.x + plane.param.y * point.y + plane.param.z * point.z + plane.param.w;
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

    mPlanes[0].param = normalize(rowW + rowX);
    mPlanes[1].param = normalize(rowW - rowX);
    mPlanes[2].param = normalize(rowW + rowY);
    mPlanes[3].param = normalize(rowW - rowY);
    mPlanes[4].param = normalize(rowW + rowZ);
    mPlanes[5].param = normalize(rowW - rowZ);
}

inline void Frustum::clear() {
    for (size_t i = 0; i < mPlanes.size(); ++i) {
        Plane &plane = mPlanes[i];
        plane.param.x = plane.param.y = plane.param.z = plane.param.w = 0.0f;
    }
}

} // namespace Common
} // namespace OSRE
