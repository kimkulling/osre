#pragma once

#include <osre/Common/Types.h>

namespace OSRE {
namespace Math {

 struct OSRE_EXPORT Quatf {
    float m_x, m_y, m_z, m_w;

    Quatf() 
    : m_x( 0.0f ), m_y( 0.0f ), m_z( 0.0f ), m_w( 1.0f ) {

    }

    void set( float x, float y, float z, float w ) {
        m_x = x;
        m_y = y;
        m_z = z;
        m_w = w;
    }
};

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Math::BaseMath
///	@ingroup	Infrastructure
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
///	@fn			::ZFXCE2::Math::ce_min
///	@ingroup	Infrastructure
///
///	@brief	Returns the minimal value.
///	@param	val1	[in] The value 1.
///	@param	val2	[in] The value 2.
///	@return	The minimal value.
//-------------------------------------------------------------------------------------------------
template<class T>
T ce_min( T val1, T val2 ) {
    return val1 < val2 ? val1 : val2;
}

//-------------------------------------------------------------------------------------------------
///	@fn			::ZFXCE2::Math::ce_max
///	@ingroup	Infrastructure
///
///	@brief	Returns the maximal value.
///	@param	val1	[in] The value 1.
///	@param	val2	[in] The value 2.
///	@return	The maximal value.
//-------------------------------------------------------------------------------------------------
template<class T>
T ce_max( T val1, T val2 ) {
    return val1 > val2 ? val1 : val2;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Math
} // Namespace ZFXCE2
