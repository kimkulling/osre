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
    ~AABB();
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
    glm::vec3 m_min;
    glm::vec3 m_max;
    f32 m_diameter;
};

inline AABB::AABB() :
        m_min(Invalid, Invalid, Invalid), m_max(-Invalid, -Invalid, -Invalid), m_diameter(0) {
    // empty
}

inline AABB::AABB(const glm::vec3 &min, const glm::vec3 &max) :
        m_min(min), m_max(max), m_diameter(0) {
    // empty
}

inline AABB::~AABB() {
    // empty
}

inline void AABB::reset() {
    m_min.x = m_min.y = m_min.z = Invalid;
    m_max.x = m_max.y = m_max.z = -Invalid;
}

inline void AABB::set(const glm::vec3 &min, const glm::vec3 &max) {
    m_min = min;
    m_max = max;
}

inline const glm::vec3 &AABB::getMin() const {
    return m_min;
}

inline const glm::vec3 &AABB::getMax() const {
    return m_max;
}

inline void AABB::merge(const glm::vec3 &vec) {
    merge(vec[0], vec[1], vec[2]);
}

inline void AABB::merge(f32 x, f32 y, f32 z) {
    // set min values
    if (x < m_min.x) {
        m_min.x = x;
    }
    if (y < m_min.y) {
        m_min.y = y;
    }
    if (z < m_min.z) {
        m_min.z = z;
    }

    // set max values
    if (x > m_max.x) {
        m_max.x = x;
    }
    if (y > m_max.y) {
        m_max.y = y;
    }
    if (z > m_max.z) {
        m_max.z =z;
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
    if (0 != m_diameter) {
        return m_diameter;
    }
    const glm::vec3 diff = (m_max - m_min);
    const f32 len = glm::length(diff);

    return len;
}

inline glm::vec3 AABB::getCenter() const {
    const glm::vec3 center = (m_min + m_max) * 0.5f;

    return center;
}

inline bool AABB::isIn(const glm::vec3 &pt) const {
    if (pt.x < m_min.x || pt.y < m_min.y || pt.z < m_min.z) {
        return false;
    }

    if (pt.x > m_max.x || pt.y > m_max.y || pt.z > m_max.z) {
        return false;
    }

    return true;
}

inline bool AABB::operator == (const AABB &rhs) const {
    return (m_max == rhs.m_max && m_min == rhs.m_min);
}

inline bool AABB::operator != (const AABB &rhs) const {
    return !(*this == rhs);
}

} // namespace Common
} // Namespace OSRE
