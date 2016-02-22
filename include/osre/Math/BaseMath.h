/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
///	@fn			::OSRE::Math::ce_min
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
///	@fn			::OSRE::Math::ce_max
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
        T invLen = ( T ) 1.0 / len;
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
    return ( m_X == other.m_X && m_Y == other.m_Y );
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
///	@class		::ZFXCE2::Math::TVector3
///	@ingroup	Infrastructure
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

    void crossProduct( const TVector3<T> &v1, const TVector3<T> &v2 );

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
        const T invLen = static_cast< T >( 1.0 / len );
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
void TVector3<T>::crossProduct( const TVector3<T> &v1, const TVector3<T> &v2 ) {
    m_X = v1.m_Y * v2.m_Z - v1.m_Z * v2.m_Y;
    m_Y = v1.m_Z * v2.m_X - v1.m_X * v2.m_Z;
    m_Z = v1.m_X * v2.m_Y - v1.m_Y * v2.m_X;
}

//-------------------------------------------------------------------------------------------------
template<class T>
inline
bool TVector3<T>::isZero() const {
    if ( m_X <= 0 && m_Y <= 0 && m_Z <= 0 ) {
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

} // Namespace Math
} // Namespace OSRE
