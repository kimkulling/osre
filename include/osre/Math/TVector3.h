#pragma once

#include <osre/Common/osre_common.h>

#include <cmath>

namespace OSRE {
namespace Math {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Math::TVector3
///	@ingroup	Engine
///
///	@brief This template implements a vector with the components x,y and z. 
//-------------------------------------------------------------------------------------------------
template<class T>
class TVector3 {
public:
	///	@brief	The class default constructor.
	TVector3();

	///	@brief	The class constructor with the vector components.
	///	@param	x	[in] Vector component for x.
	///	@param	y	[in] Vector component for y.
	///	@param	z	[in] Vector component for z.
	TVector3( T x, T y, T z );

	///	@brief	The class copy constructor.
	TVector3( const TVector3<T> &other );

	///	@brief	The class destructor.
	~TVector3();

	///	@brief	Sets the components.
	///	@param	x	[in] Vector component for x.
	///	@param	y	[in] Vector component for y.
	///	@param	z	[in] Vector component for z.
	void set( T x, T y, T z );
	
	///	@brief	Returns the x component.
	///	@return	The x component.
	T getX() const;

	///	@brief	Returns the y component.
	///	@return	The y component.
	T getY() const;

	///	@brief	Returns the z component.
	///	@return	The z component.
	T getZ() const;

	///	@brief	Returns the length of the vector.
	///	@return	The length.
	T getLength() const;

	///	@brief	Returns the squared length of the vector.
	///	@return	The squared length.
	T getSqaredLength() const;

	///	@brief	The vector will be normalized, the length will be 1 after that.
	///	@return	The normalized vector itself.
	TVector3<T> &normalize();

	///	@brief	Computes the dot-product with another vector instance.
	///	@param	other	The other vector instance.
	///	@return	The dot-product.
	T dotProduct( const TVector3<T> &other ) const;

	///	@brief	Computes the cross-product with another vector instance, the result vector will be
	///			returned.
	///	@param	other	The other vector.
	///	@return	A vector, which contains the resulting cross product.
	TVector3<T> crossProduct( const TVector3<T> &other ) const;

	void crossProduct(const TVector3<T> &v1, const TVector3<T> &v2);

	bool isZero() const;

	///	@brief	+ operator implementation.
	///	@param	other	Vector to add.
	///	@return	Result vector.
	TVector3<T> operator + ( const TVector3<T> &other ) const;

	///	@brief	=+ operator implementation.
	///	@param	other	Vector to subtract.
	///	@return	Result vector.
	TVector3<T> &operator += ( const TVector3<T> &other );

	///	@brief	- operator implementation.
	///	@param	other	Vector to subtract.
	///	@return	Result vector.
	TVector3<T> operator - ( const TVector3<T> &other ) const;

	///	@brief	-= operator implementation.
	///	@param	other	Vector to subtract.
	///	@return	Result vector.
	TVector3<T> &operator -= ( const TVector3<T> &other );

	///	@brief	- operator implementation.
	///	@param	other	Vector to subtract.
	///	@return	Result vector.
	TVector3<T> operator * ( const TVector3<T> &other ) const;

	///	@brief	- operator implementation.
	///	@param	other	Vector to subtract.
	///	@return	Result vector.
	TVector3<T> &operator *= ( const TVector3<T> &other );

	///	@brief	- operator implementation.
	///	@param	other	Vector to subtract.
	///	@return	Result vector.
	TVector3<T> operator + ( const T value ) const;

	///	@brief	- operator implementation.
	///	@param	other	Vector to subtract.
	///	@return	Result vector.
	TVector3<T> &operator += ( const T value );

	///	@brief	- operator implementation.
	///	@param	other	Vector to subtract.
	///	@return	Result vector.
	TVector3<T> operator - ( const T value ) const;

	///	@brief	- operator implementation.
	///	@param	other	Vector to subtract.
	///	@return	Result vector.
	TVector3<T> &operator -= ( const T value );

	///	@brief	- operator implementation.
	///	@param	other	Vector to multiply.
	///	@return	Result vector.
	TVector3<T> operator * ( const T value ) const;

	///	@brief	- operator implementation.
	///	@param	other	Vector to multiply.
	///	@return	Result vector.
	TVector3<T> &operator *= ( const T value );

	///	@brief	Compare operator implementation.
	///	@param	other	Vector to compare.
	///	@return	true, if both vectors are equal.
	bool operator == ( const TVector3<T> &other ) const;

	///	@brief	Not equal operator implementation.
	///	@param	other	Vector to compare.
	///	@return	true, if both vectors are equal.
	bool operator != ( const TVector3<T> &other ) const;

	///	@brief	Assignment operator implementation.
	///	@param	other	Vector to assign.
	///	@return	Result vector.
	TVector3<T> &operator = ( const TVector3<T> &other );

	///	The components x, y, z.
	T m_X, m_Y, m_Z;
};

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T>::TVector3() :
		m_X( 0.0 ),
		m_Y( 0.0 ), 
		m_Z( 0.0 ) {
	// empty
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T>::TVector3( T x, T y, T z ) :
		m_X( x ), 
		m_Y( y ), 
		m_Z( z ) {
	// empty
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T>::TVector3( const TVector3<T> &other ) :
		m_X( other.m_X ), 
		m_Y( other.m_Y ),
		m_Z( other.m_Z ) {
	// empty
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T>::~TVector3() {
	// empty
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
void TVector3<T>::set( T x, T y, T z ) {
	m_X = x;
	m_Y = y;
	m_Z = z;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector3<T>::getX() const {
	return m_X;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector3<T>::getY() const {
	return m_Y;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector3<T>::getZ() const {
	return m_Z;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector3<T>::getLength() const {
	return ::sqrt( m_X*m_X + m_Y*m_Y + m_Z*m_Z );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector3<T>::getSqaredLength() const {
	return ( m_X*m_X + m_Y*m_Y + m_Z*m_Z );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
 TVector3<T> &TVector3<T>::normalize() {
	const T len = this->getLength();
	if ( 0.0 != len ) {
		const T invLen = static_cast<T>( 1.0 / len );
		m_X *= invLen;
		m_Y *= invLen;
		m_Z *= invLen;
	}

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T  TVector3<T>::dotProduct( const TVector3<T> &other ) const {
	return m_X * other.m_X + m_Y * other.m_Y + m_Z * other.m_Z;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T>  TVector3<T>::crossProduct( const TVector3<T> &other ) const {
	return TVector3<T>( m_Y * other.m_Z 
		- m_Z * other.m_Y, m_Z * other.m_X 
		- m_X * other.m_Z, m_X * other.m_Y 
		- m_Y * other.m_X );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
void TVector3<T>::crossProduct( const TVector3<T> &v1, const TVector3<T> &v2 )  {
	m_X = v1.m_Y * v2.m_Z - v1.m_Z * v2.m_Y;
	m_Y = v1.m_Z * v2.m_X - v1.m_X * v2.m_Z;
	m_Z = v1.m_X * v2.m_Y - v1.m_Y * v2.m_X;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
bool TVector3<T>::isZero() const {
	if ( m_X <=0 && m_Y<=0 && m_Z <=0 ){
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T> TVector3<T>::operator + ( const TVector3<T> &other ) const {
	TVector3<T> newVector( m_X + other.m_X, m_Y + other.m_Y, m_Z + other.m_Z );

	return newVector;

}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T> &TVector3<T>::operator += ( const TVector3<T> &other ) {
	m_X += other.m_X;
	m_Y += other.m_Y;
	m_Z += other.m_Z;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T> TVector3<T>::operator - ( const TVector3<T> &other ) const {
	TVector3<T> newVector( m_X - other.m_X, m_Y - other.m_Y, m_Z - other.m_Z );

	return newVector;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T> &TVector3<T>::operator -= ( const TVector3<T> &other ) {
	m_X -= other.m_X;
	m_Y -= other.m_Y;
	m_Z -= other.m_Z;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T> TVector3<T>::operator * ( const TVector3<T> &other ) const {
	TVector3<T> newVector( m_X * other.m_X, m_Y * other.m_Y, m_Z * other.m_Z );

	return newVector;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T> &TVector3<T>::operator *= ( const TVector3<T> &other ) {
	m_X *= other.m_X;
	m_Y *= other.m_Y;
	m_Z *= other.m_Z;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T> TVector3<T>::operator + ( const T value ) const {
	TVector3<T> newValue;
	newValue.m_X = m_X + value;
	newValue.m_Y = m_Y + value;
	newValue.m_Z = m_Z + value;

	return newValue;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T> &TVector3<T>::operator += ( const T value ) {
	m_X += value;
	m_Y += value;
	m_Z += value;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T> TVector3<T>::operator - ( const T value ) const {
	TVector3<T> newValue;
	newValue.m_X = m_X - value;
	newValue.m_Y = m_Y - value;
	newValue.m_Z = m_Z - value;

	return newValue;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T> &TVector3<T>::operator -= ( const T value ) {
	m_X -= value;
	m_Y -= value;
	m_Z -= value;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T> TVector3<T>::operator * ( const T value ) const {
	TVector3<T> newValue;
	newValue.m_X = m_X * value;
	newValue.m_Y = m_Y * value;
	newValue.m_Z = m_Z * value;

	return newValue;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T> &TVector3<T>::operator *= ( const T value ) {
	m_X *= value;
	m_Y *= value;
	m_Z *= value;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
bool TVector3<T>::operator == ( const TVector3<T> &other ) const {
	return ( this->m_X == other.m_X && this->m_Y == other.m_Y && this->m_Z == other.m_Z );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
bool TVector3<T>::operator != ( const TVector3<T> &other ) const {
	return ( !( other == *this ) );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector3<T> &TVector3<T>::operator = ( const TVector3<T> &other ) {
	if ( other == *this ) {
		return *this;
	}

	m_X = other.m_X;
	m_Y = other.m_Y;
	m_Z = other.m_Z;

	return *this;
}

//-------------------------------------------------------------------------------------------------
///	The single precision vector.
typedef TVector3<f32> Vector3f;
///	The double precision vector.
typedef TVector3<d32> Vector3d;

//-------------------------------------------------------------------------------------------------

template<class S, class T>
inline 
TVector3<T> operator * ( const S scalar, const TVector3<T>& vector ) {
	return vector * scalar; 
}

//-------------------------------------------------------------------------------------------------

} // Namespace Math
} // Namespace ZFXCE2
