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

namespace OSRE {
namespace Common {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class is used to define a bounding volume, which is aligned at the axis of the
/// global coordinate system.
//-------------------------------------------------------------------------------------------------
class AABB {
public:
    static constexpr f32 Invalid = 999999;

    AABB();
    AABB(const glm::vec3 &min, const glm::vec3 &max);
    ~AABB() = default;
    void reset();
    void set(const glm::vec3 &min, const glm::vec3 &max);
    const glm::vec3 &getMin() const;
    const glm::vec3 &getMax() const;
    void merge(const glm::vec3 &vec);
    void merge(f32 x, f32 y, f32 z);
    void updateFromVector3Array(glm::vec3 *vecArray, ui32 numVectors);
    f32 getDiameter() const;
    glm::vec3 getCenter() const;
    bool isIn(const glm::vec3 &pt) const;
    bool operator==(const AABB &rhs) const;
    bool operator!=(const AABB &rhs) const;

private:
    glm::vec3 mMin;
    glm::vec3 mMax;
    f32 mDiameter = 0.0f;
};

inline AABB::AABB() :
        mMin(Invalid, Invalid, Invalid), mMax(-Invalid, -Invalid, -Invalid) {
    // empty
}

inline AABB::AABB(const glm::vec3 &min, const glm::vec3 &max) :
        mMin(min), mMax(max) {
    // empty
}

inline void AABB::reset() {
    mMin.x = mMin.y = mMin.z = Invalid;
    mMax.x = mMax.y = mMax.z = -Invalid;
}

inline void AABB::set(const glm::vec3 &min, const glm::vec3 &max) {
    mMin = min;
    mMax = max;
}

inline const glm::vec3 &AABB::getMin() const {
    return mMin;
}

inline const glm::vec3 &AABB::getMax() const {
    return mMax;
}

inline void AABB::merge(const glm::vec3 &vec) {
    merge(vec[0], vec[1], vec[2]);
}

inline void AABB::merge(f32 x, f32 y, f32 z) {
    // set min values
    if (x < mMin.x) {
        mMin.x = x;
    }
    if (y < mMin.y) {
        mMin.y = y;
    }
    if (z < mMin.z) {
        mMin.z = z;
    }

    // set max values
    if (x > mMax.x) {
        mMax.x = x;
    }
    if (y > mMax.y) {
        mMax.y = y;
    }
    if (z > mMax.z) {
        mMax.z = z;
    }
}

inline void AABB::updateFromVector3Array(glm::vec3 *vecArray, ui32 numVectors) {
    if (nullptr == vecArray || 0 == numVectors) {
        return;
    }

    for (ui32 i = 0; i < numVectors; ++i) {
        glm::vec3 &v(vecArray[i]);
        merge(v);
    }
}

inline f32 AABB::getDiameter() const {
    if (0 != mDiameter) {
        return mDiameter;
    }
    const glm::vec3 diff = (mMax - mMin);
    const f32 len = glm::length(diff);

    return len;
}

inline glm::vec3 AABB::getCenter() const {
    const glm::vec3 center = (mMin + mMax) * 0.5f;

    return center;
}

inline bool AABB::isIn(const glm::vec3 &pt) const {
    if (pt.x < mMin.x || pt.y < mMin.y || pt.z < mMin.z) {
        return false;
    }

    if (pt.x > mMax.x || pt.y > mMax.y || pt.z > mMax.z) {
        return false;
    }

    return true;
}

inline bool AABB::operator == (const AABB &rhs) const {
    return (mMax == rhs.mMax && mMin == rhs.mMin);
}

inline bool AABB::operator != (const AABB &rhs) const {
    return !(*this == rhs);
}

} // namespace Common
} // Namespace OSRE
