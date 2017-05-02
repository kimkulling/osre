/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
namespace Common {

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief	This helper class stores any derived class from the ZFXCE specific base-class IObject.
///
///	It will manage the whole management of its ownership and release it after getting destroyed. 
///	You should create instances from it on the stack to get an automatically released instance.
///	The -> and . operators are offering the access to the pointer of the managed class as well. 
///	Any copy operations are managed shallow, the ownership by calling its references will be 
///	managed by this class.
//-------------------------------------------------------------------------------------------------
template<class T>
class TObjPtr {
public:
	///	@brief	The default class constructor.
	TObjPtr();

	///	@brief	Constructor with a pointer showing to the managed instance.
	///	@param	pPtr	[in] A valid pointer showing to the managed instance, must be != NULL.
	TObjPtr( T *pPtr );

	///	@brief	The copy constructor.
	///	@param	other	[in] The instance to copy from.
	TObjPtr( const TObjPtr<T> &other );

	///	@brief	The destructor.
	~TObjPtr();

	///	@brief	Initiates the pointer without incrementing the reference counter.
	///	@param	pPtr	[in] The init pointer.
	void init( T *pPtr );

	///	@brief	This method will assign a new instance, if an older instance is managed it will released.
	///	@param	pPtr	[in] A valid pointer showing to the managed instance, must be != NULL.
	void set( T *pPtr );

	///	@brief	This method will return the raw pointer directly.
	///	@return	The raw pointer will be returned.
	T *getPtr() const;
	
	///	@brief	The managed pointer will be dropped, the reference will be released as well.
	void clear();

	///	@brief	The managed pointer will be validated. If its not NULL, the pointer is valid.
	///	@return	true, if the pointer is valid, false if not.
	bool isValid() const;

	///	@brief	-> operator implementation, allows access to the managed pointer.
	T *operator ->() const;

	///	@brief	() operator implementation.
	T &operator *() const;
	
	///	@brief	The copy operator.
	TObjPtr<T> &operator = ( const TObjPtr<T> &rOther );
	
	///	@brief	The assignment operator.
	TObjPtr<T> &operator = ( T *pPtr );

	///	@brief	The compare operator.
	bool operator == ( const TObjPtr<T> &rOther ) const;
	
	///	@brief	Not-equal operator.
	bool operator != ( const TObjPtr<T> &rOther ) const;

private:
	T *m_ptr;
};

template<class T>
inline
TObjPtr<T>::TObjPtr() 
: m_ptr( nullptr ) {
	// empty
}

template<class T>
inline
TObjPtr<T>::TObjPtr( T *pPtr ) 
: m_ptr( pPtr ) {
	if ( nullptr != m_ptr ) {
		m_ptr->get();
	}
}

template<class T>
inline
TObjPtr<T>::TObjPtr( const TObjPtr<T> &other ) 
: m_ptr( other.m_ptr ) {
	if ( nullptr != m_ptr ) {
		m_ptr->get();
	}
}

template<class T>
inline
TObjPtr<T>::~TObjPtr() {
	clear();
}

template<class T>
inline
void TObjPtr<T>::init( T *pPtr ) {
	if ( m_ptr ) {
		m_ptr->release();
	}

	if ( nullptr != pPtr )	{
		m_ptr = pPtr;
	}
}

template<class T>
inline
void TObjPtr<T>::set( T *pPtr ) {
	clear();
	if ( nullptr != pPtr )	{
		m_ptr = pPtr;
		m_ptr->get();
	}
}

template<class T>
inline
T *TObjPtr<T>::getPtr() const {
	return m_ptr;
}

template<class T>
inline
void TObjPtr<T>::clear() {
	if ( nullptr != m_ptr ) {
		m_ptr->release();
		m_ptr = nullptr;
	}
}
template<class T>
inline
bool TObjPtr<T>::isValid() const {
	return ( nullptr != m_ptr );
}

template<class T>
inline
T *TObjPtr<T>::operator -> () const {
	return m_ptr;
}

template<class T>
inline
T &TObjPtr<T>::operator *() const {
	return *m_ptr;
}

template<class T>
inline
TObjPtr<T> &TObjPtr<T>::operator = ( const TObjPtr<T> &other ) {
	if ( m_ptr == other.m_ptr ) {
		return *this;
	}

	clear();
	m_ptr = other.m_ptr;
	m_ptr->get();

	return *this;
}

template<class T>
inline
TObjPtr<T> &TObjPtr<T>::operator = ( T *pPtr ) {
	clear();
	if ( nullptr != pPtr ) {
		m_ptr = pPtr;
		m_ptr->get();
	}

	return *this;
}

template<class T>
inline
bool TObjPtr<T>::operator == ( const TObjPtr<T> &rhs ) const {
	return ( m_ptr == rhs.m_ptr );
}

template<class T>
inline
bool TObjPtr<T>::operator != ( const TObjPtr<T> &rhs ) const {
	return !( m_ptr == rhs.m_ptr );
}

} // Namespace Common
} // Namespace OSRE
