/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-2014, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions
and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
and the following disclaimer in the documentation and/or other materials provided with the
distribution.

* Neither the name of the ZFXCE2 team, nor the names of its contributors may be used to endorse or
promote products derived from this software without specific prior written permission of the
ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
#ifndef CE_INFRASTRUCTURE_MATH_TVERCTOR4_H_INC
#define CE_INFRASTRUCTURE_MATH_TVERCTOR4_H_INC
#pragma once

#include <zfxce2/Types.h>
#include <zfxce2/Infrastructure/Math/TVector3.h>
#include <cmath>

namespace OSRE {
namespace Math {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Math::TVector4
///	@ingroup	Engine
///
///	@brief This template implements a vector with the components x, y, z and w. 
//-------------------------------------------------------------------------------------------------
template<class T>
class TVector4 {
public:
	///	@brief	The class default constructor.
	TVector4();

	///	@brief	Constructor with components for the vector.
	///	@param	x	[in] Vector component for x.
	///	@param	y	[in] Vector component for y.
	///	@param	z	[in] Vector component for z.
	///	@param	z	[in] Vector component for w.
	TVector4( T x, T y, T z, T w );

	///	@brief	The class copy constructor.
	TVector4( const TVector4<T> &other );

	///	@brief	The class destructor.
	~TVector4();

	///	@brief	Sets the components.
	///	@param	x	[in] Vector component for x.
	///	@param	y	[in] Vector component for y.
	///	@param	z	[in] Vector component for z.
	///	@param	z	[in] Vector component for w.
	void set( T x, T y, T z, T w );

	///	@brief	Returns the x component.
	///	@return	The x component.
	T getX() const;

	///	@brief	Returns the y component.
	///	@return	The y component.
	T getY() const;

	///	@brief	Returns the z component.
	///	@return	The z component.
	T getZ() const;

	///	@brief	Returns the w component.
	///	@return	The w component
	T getW() const;

	///	@brief	Returns the length of the vector.
	///	@return	The length.
	T getLength() const;

	///	@brief	Returns the squared length of the vector.
	///	@return	The squared length.
	T getSqaredLength() const;

	///	@brief	+ operator implementation.
	///	@param	other	Vector to add.
	///	@return	Result vector.
	TVector4<T> operator + ( const TVector4<T> &other );

	///	@brief	=+ operator implementation.
	///	@param	other	Vector to add.
	///	@return	Result vector.
	TVector4<T> &operator += ( const TVector4<T> &other );

	///	@brief	- operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector4<T> operator - ( const TVector4<T> &other );

	///	@brief	-= operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector4<T> &operator -= ( const TVector4<T> &other );

	///	@brief	- operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector4<T> operator * ( const TVector4<T> &other );

	///	@brief	- operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector4<T> &operator *= ( const TVector4<T> &other );

	///	@brief	- operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector4<T> operator + ( const T value );

	///	@brief	- operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector4<T> &operator += ( const T value );

	///	@brief	- operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector4<T> operator - ( const T value );

	///	@brief	- operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector4<T> &operator -= ( const T value );

	///	@brief	- operator implementation.
	///	@param	other	Vector to multiply.
	///	@return	Result vector.
	TVector4<T> operator * ( const T value );

	///	@brief	- operator implementation.
	///	@param	other	Vector to multiply.
	///	@return	Result vector.
	TVector4<T> &operator *= ( const T value );

	///	@brief	Compare operator implementation.
	///	@param	other	Vector to compare.
	///	@return	true, if both vectors are equal.
	bool operator == ( const TVector4<T> &other ) const;

	///	@brief	Assignment operator implementation.
	///	@param	other	Vector to assign.
	///	@return	Result vector.
	TVector4<T> &operator = ( const TVector4<T> &other );

	///	The components x, y, z and w.
	T m_X, m_Y, m_Z, m_W;
};

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T>::TVector4() 
: m_X( 0.0 )
, m_Y( 0.0 )
, m_Z( 0.0 )
, m_W( 1.0 ) {
	// empty
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T>::TVector4( T x, T y, T z, T w ) 
: m_X( x )
, m_Y( y )
, m_Z( z )
, m_W( w ) {
	// empty
}

//-------------------------------------------------------------------------------------------------
//	The copy constructor.
template<class T>
inline
TVector4<T>::TVector4( const TVector4<T> &other ) :
		m_X( other.m_X ),
		m_Y( other.m_Y ),
		m_Z( other.m_Z ),
		m_W( other.m_W ) {
	// empty
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T>::~TVector4() {
	// empty
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
void TVector4<T>::set( T x, T y, T z, T w ) {
	m_X = x;
	m_Y = y;
	m_Z = z;
	m_W = w;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector4<T>::getX() const {
	return m_X;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector4<T>::getY() const {
	return m_Y;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector4<T>::getZ() const {
	return m_Z;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector4<T>::getW() const {
	return m_W;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector4<T>::getLength() const {
	return ::sqrt( m_X*m_X + m_Y*m_Y + m_Z*m_Z + m_W*m_W );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector4<T>::getSqaredLength() const {
	return ( m_X*m_X + m_Y*m_Y + m_Z*m_Z + m_W*m_W );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T> TVector4<T>::operator + ( const TVector4<T> &other ) {
	TVector4<T> newVector( m_X + other.m_X, m_Y + other.m_Y, m_Z + other.m_Z );

	return newVector;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T> &TVector4<T>::operator += ( const TVector4<T> &other ) {
	m_X += other.m_X;
	m_Y += other.m_Y;
	m_Z += other.m_Z;

	return *this;

}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T> TVector4<T>::operator - ( const TVector4<T> &other ) {
	TVector4<T> newVector( m_X - other.m_X, m_Y - other.m_Y, m_Z - other.m_Z );

	return newVector;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T> &TVector4<T>::operator -= ( const TVector4<T> &other ) {
	m_X -= other.m_X;
	m_Y -= other.m_Y;
	m_Z -= other.m_Z;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T> TVector4<T>::operator * ( const TVector4<T> &other ) {
	TVector4<T> newVector( m_X * other.m_X, m_Y * other.m_Y, m_Z * other.m_Z );

	return newVector;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T> &TVector4<T>::operator *= ( const TVector4<T> &other ) {
	m_X *= other.m_X;
	m_Y *= other.m_Y;
	m_Z *= other.m_Z;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T> TVector4<T>::operator + ( const T value ) {
	TVector3<T> newValue( m_X + value, m_Y + value,  m_Z + value );

	return newValue;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T> &TVector4<T>::operator += ( const T value ) {
	m_X += value;
	m_Y += value;
	m_Z += value;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T> TVector4<T>::operator - ( const T value ) {
	TVector3<T> newValue;
	newValue.m_X = m_X - value;
	newValue.m_Y = m_Y - value;
	newValue.m_Z = m_Z - value;

	return newValue;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T> &TVector4<T>::operator -= ( const T value ) {
	m_X -= value;
	m_Y -= value;
	m_Z -= value;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T> TVector4<T>::operator * ( const T value ) {
	TVector4<T> newValue;
	newValue.m_X = m_X * value;
	newValue.m_Y = m_Y * value;
	newValue.m_Z = m_Z * value;

	return newValue;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T> &TVector4<T>::operator *= ( const T value ) {
	m_X *= value;
	m_Y *= value;
	m_Z *= value;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
bool TVector4<T>::operator == ( const TVector4<T> &other ) const {
	return ( m_X == other.m_X && m_Y == other.m_Y && m_Z == other.m_Z && m_W == other.m_W );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector4<T> &TVector4<T>::operator = ( const TVector4<T> &other ) {
	if ( other == *this ) {
		return *this;
	}

	m_X = other.m_X;
	m_Y = other.m_Y;
	m_Z = other.m_Z;
	m_W = other.m_W;

	return *this;
}

//-------------------------------------------------------------------------------------------------
///	The float vector type, single precision.
typedef TVector4<f32> Vector4f;
///	The float vector type, double precision.
typedef TVector4<d32> Vector4d;

//-------------------------------------------------------------------------------------------------

template<class S, class T>
inline 
TVector3<T> operator * ( const S scalar, const TVector4<T>& vector ) {
	return vector * scalar; 
}

//-------------------------------------------------------------------------------------------------

} // Namespace Math
} // Namespace ZFXCE2

#endif // CE_INFRASTRUCTURE_MATH_TVERCTOR4_H_INC
