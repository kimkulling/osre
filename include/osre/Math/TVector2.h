#pragma once

#include <osre/Common/osre_common.h>

#include <cmath>

namespace OSRE {
namespace Math {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Math::TVector2
///	@ingroup	Infrastructure
///
///	@brief This template implements a vector with the components x,y and z. 
//-------------------------------------------------------------------------------------------------
template<class T>
class TVector2 {
public:
	///	@brief	The class default constructor.
	TVector2();

	///	@brief	The class constructor with values.
	///	@param	x, y	Vector components.
	TVector2( T x, T y );

	///	@brief	The class Copy constructor.
	TVector2( const TVector2<T> &other );

	///	@brief	The class destructor.
	~TVector2();

	///	@brief	Sets the components.
	///	@param	x,y		[in] Vector components.
	void set( T x, T y );

	///	@brief	Returns the x component.
	///	@return	The x component.
	T getX() const;

	///	@brief	Returns the y component.
	///	@return	The y component.
	T getY() const;

	///	@brief	Returns the length of the vector.
	///	@return	The length.
	T getLength() const;
	
	///	@brief	Returns the squared length of the vector.
	///	@return	The squared length.
	T getSqaredLength() const;

	///	@brief	The vector will be normalized, the length will be 1 after that.
	///	@return	The normalized vector itself.
	TVector2<T> &normalize();

	///	@brief	Computes the dot-product with another vector instance.
	///	@param	other	The other vector instance.
	///	@return	The dot-product.
	T dotProduct( const TVector2<T> &other ) const;

	///	@brief	Computes the cross-product with another vector instance, the result vector will be
	///			returned.
	///	@param	other	The other vector.
	///	@return	A vector, which contains the resulting cross product.
	TVector2<T> crossProduct( const TVector2<T> &other ) const;

	///	@brief	+ operator implementation.
	///	@param	other	Vector to add.
	///	@return	Result vector.
	TVector2<T> operator + ( const TVector2<T> &other );

	///	@brief	=+ operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector2<T> &operator += ( const TVector2<T> &other );

	///	@brief	- operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector2<T> operator - ( const TVector2<T> &other );

	///	@brief	-= operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector2<T> &operator -= ( const TVector2<T> &other );

	///	@brief	- operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector2<T> operator * ( const TVector2<T> &other );

	///	@brief	- operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector2<T> &operator *= ( const TVector2<T> &other );

	///	@brief	- operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector2<T> operator + ( const T value );

	///	@brief	- operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector2<T> &operator += ( const T value );

	///	@brief	- operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector2<T> operator - ( const T value );

	///	@brief	- operator implementation.
	///	@param	other	Vector to substract.
	///	@return	Result vector.
	TVector2<T> &operator -= ( const T value );

	///	@brief	- operator implementation.
	///	@param	other	Vector to multiply.
	///	@return	Result vector.
	TVector2<T> operator * ( const T value );

	///	@brief	- operator implementation.
	///	@param	other	Vector to multiply.
	///	@return	Result vector.
	TVector2<T> &operator *= ( const T value );

	///	@brief	Compare operator implementation.
	///	@param	other	Vector to compare.
	///	@return	true, if both vectors are equal.
	bool operator == ( const TVector2<T> &other ) const;

	///	@brief	Assignment operator implementation.
	///	@param	other	Vector to assign.
	///	@return	Result vector.
	TVector2<T> &operator = ( const TVector2<T> &other );

	///	The components x and y.
	T m_X, m_Y;
};

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T>::TVector2() :
	m_X( 0.0 ), 
	m_Y( 0.0 ) 
{
	// empty
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T>::TVector2( T x, T y ) :
	m_X( x ),
	m_Y( y ) 
{
	// empty
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T>::TVector2( const TVector2<T> &other ) :
	m_X( other.m_X ), 
	m_Y( other.m_Y ) 
{
	// empty
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T>::~TVector2() {
	// empty
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
void TVector2<T>::set( T x, T y ) {
	m_X = x;
	m_Y = y;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector2<T>::getX() const {
	return m_X;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector2<T>::getY() const {
	return m_Y;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector2<T>::getLength() const {
	return ::sqrt( this->m_X * this->m_X + this->m_Y * this->m_Y );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector2<T>::getSqaredLength() const {
	return ( this->m_X * this->m_X + this->m_Y * this->m_Y );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> &TVector2<T>::normalize() {
	T len = getLength();
	if ( 0.0 != len ) {
		T invLen = (T) 1.0 / len;
		m_X *= invLen;
		m_Y *= invLen;
	}

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
T TVector2<T>::dotProduct( const TVector2<T> &other ) const {
	return ( this->m_X * other.m_X + this->m_Y * other.m_Y );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> TVector2<T>::crossProduct( const TVector2<T> &other ) const {
	return TVector2<T>( this->m_X * other.m_Y, this->m_Y * other.m_X );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> TVector2<T>::operator + ( const TVector2<T> &other ) {
	return TVector2<T>( this->m_X + other.m_X, this->m_Y + other.m_Y );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> &TVector2<T>::operator += ( const TVector2<T> &other ) {
	m_X += other.m_X;
	m_Y += other.m_Y;

	return *this;

}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> TVector2<T>::operator - ( const TVector2<T> &other ) {
	return TVector2<T>( m_X - other.m_X, m_Y - other.m_Y );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> &TVector2<T>::operator -= ( const TVector2<T> &other ) {
	m_X -= other.m_X;
	m_Y -= other.m_Y;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> TVector2<T>::operator * ( const TVector2<T> &other ) {
	return TVector2<T>( m_X * other.m_X, m_Y * other.m_Y );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> &TVector2<T>::operator *= ( const TVector2<T> &other ) {
	m_X *= other.m_X;
	m_Y *= other.m_Y;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> TVector2<T>::operator + ( const T value ) {
	return TVector2<T>( m_X + value, m_Y + value );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> &TVector2<T>::operator += ( const T value ) {
	m_X += value;
	m_Y += value;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> TVector2<T>::operator - ( const T value ) {
	return TVector2<T>( m_X - value, m_Y - value );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> &TVector2<T>::operator -= ( const T value ) {
	m_X -= value;
	m_Y -= value;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> TVector2<T>::operator * ( const T value ) {
	return TVector2<T>( m_X * value, m_Y * value );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> &TVector2<T>::operator *= ( const T value ) {
	m_X *= value;
	m_Y *= value;

	return *this;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
bool TVector2<T>::operator == ( const TVector2<T> &other ) const {
	return ( m_X == other.m_X && m_Y == other.m_Y  );
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
TVector2<T> &TVector2<T>::operator = ( const TVector2<T> &other ) {
	if ( other == *this ) {
		return *this;
	}

	m_X = other.m_X;
	m_Y = other.m_Y;

	return *this;
}

//-------------------------------------------------------------------------------------------------
typedef TVector2<f32> Vector2f;

typedef TVector2<d32> Vector2d;

//-------------------------------------------------------------------------------------------------
template<class S, class T>
inline 
TVector2<T> operator * ( const S scalar, const TVector2<T>& vector ) {
	return vector * scalar; 
}

//-------------------------------------------------------------------------------------------------

} // Namespace Math
} // Namespace ZFXCE2
