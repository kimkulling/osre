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

#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OSRE {
namespace Math {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Math::BaseMath
///	@ingroup	Engine
///
///	@brief	This class offer base informations about the mathematical subsystem like precisions.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT BaseMath {
public:
    ///	@brief	The default class constructor.
    BaseMath();

    ///	@brief	The class destructor.
    ~BaseMath();

    ///	@brief	Returns the single precision boundary value.
    /// @return epsilon for float.
    static f32 getSPEPS();

    ///	@brief	Returns PI.
    /// @return Pi value.
    static f32 getPI();
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	Returns the minimal value.
///	@param	val1	[in] The value 1.
///	@param	val2	[in] The value 2.
///	@return	The minimal value.
//-------------------------------------------------------------------------------------------------
template <class T>
inline T osre_min(T val1, T val2) {
    return val1 < val2 ? val1 : val2;
}

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	Returns the maximal value.
///	@param	val1	[in] The value 1.
///	@param	val2	[in] The value 2.
///	@return	The maximal value.
//-------------------------------------------------------------------------------------------------
template <class T>
inline T osre_max(T val1, T val2) {
    return val1 > val2 ? val1 : val2;
}

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	Performs a linear interpolation.
///	@param	v0	[in] The value 1.
///	@param	v1	[in] The value 2.
/// @param  t   [in] The time ( 0 ... 1 ).
///	@return	The interpolated value.
//-------------------------------------------------------------------------------------------------
template <typename T>
inline T osre_lerp(T v0, T v1, T t) {
    return (1 - t) * v0 + t * v1;
}

} // Namespace Math
} // Namespace OSRE
