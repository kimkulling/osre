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

#include <osre/Common/osre_common.h>
#include <osre/Common/glm_common.h>

namespace OSRE {
namespace Common {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements a ray with a origin and a distance.
//-------------------------------------------------------------------------------------------------
class Ray {
public:
    /// @brief  The default class constructor.
    Ray();
    
    /// @brief  The constructor with the ray parameters.
    /// @param[in] origin     The ray origin
    /// @param[in] direction  The director for the ray
    Ray(const glm::vec3 &origin, const glm::vec3 &direction);

    /// @brief  The default class destructor.
    ~Ray();
    
    /// @brief  Will validate the length for a given vector for a given time(scaling).
    /// @param[in] time  The scaling.
    /// @return The validated vector.
    glm::vec3 validate(f32 time);
    
    /// @brief  Returns the origin of the ray.
    /// @return The origin.
    const glm::vec3 &getOrigin() const;
    
    /// @brief  Returns the direction of the ray.
    /// @return The direction.
    const glm::vec3 &getDirection() const;
    
    /// @brief The compare operator.
    bool operator == ( const Ray &rhs) const;
    
    /// @brief The not-equal operator.
    bool operator != (const Ray &rhs) const;

private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
};

inline Ray::Ray() :
        m_origin(), 
        m_direction() {
    // empty
}

inline Ray::Ray(const glm::vec3 &origin, const glm::vec3 &direction) :
        m_origin(origin), 
        m_direction(direction) {
    // empty
}

inline Ray::~Ray() {
    // empty
}

inline glm::vec3 Ray::validate(f32 time) {
    const glm::vec3 res = m_origin + time * m_direction;
    return res;
}

inline const glm::vec3 &Ray::getOrigin() const {
    return m_origin;
}

inline const glm::vec3  &Ray::getDirection() const {
    return m_direction;
}

inline bool Ray::operator == (const Ray &rhs) const {
    return ( m_origin == rhs.m_origin && m_direction == rhs.m_direction );
}

inline bool Ray::operator != (const Ray &rhs) const {
    return !( *this == rhs );
}

} // Namespace Collision
} // Namespace OSRE
