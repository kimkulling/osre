/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/Types.h>

namespace OSRE {
namespace Common {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Common::FunctorImpl
///	@ingroup	Infrastructure
///	
///	@brief Base abstract interface of functor implementation
//-------------------------------------------------------------------------------------------------
template <class RET, class P1, class P2 >
class FunctorImpl {
public:
    /// @brief The virtual destructor.
    virtual ~FunctorImpl();

    /// @brief Call for overwrite.
    ///	@param	p1	Parameter 1.
    ///	@param	p2	Parameter 2.
    ///	@return	The return value.
    virtual RET Call( P1 p1, P2 p2 ) const = 0;
};

//-------------------------------------------------------------------------------------------------
template < class RET, class P1, class P2 >
inline 
FunctorImpl<RET, P1, P2>::~FunctorImpl() {
    // empty
}

//-------------------------------------------------------------------------------------------------
/// @class		::ZFXCE2::Common::FunctorFunction
///	@ingroup	Infrastructure
///
/// @brief	This template class implements a binding to a function.
//-------------------------------------------------------------------------------------------------
template < class RET, class P1, class P2 >
class FunctorFunction : public FunctorImpl<RET, P1, P2> {
public:
    ///	The function type.
    typedef RET (*FunctorFunctionType)( P1, P2 );

    /// @brief The constructor with the function type.
    FunctorFunction( FunctorFunctionType func ) : m_Func( func ) { /* empty */ }

    /// @brief Virtual destructor.
    virtual ~FunctorFunction() { /* empty */ }
        
    /// @brief Performs the binded function call
    ///	@param	p1	Parameter 1.
    ///	@param	p2	Parameter 1.
    virtual RET Call( P1 p1, P2 p2 ) const {
        return m_Func(p1,p2);
    }

private:
    FunctorFunctionType m_Func;
};

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Common::FunctorMember
///	@ingroup	Infrastructure
///
/// @brief	Binding to a member with the class instance.
//-------------------------------------------------------------------------------------------------
template < class T, class RET, class P1, class P2 >
class FunctorMember : public FunctorImpl<RET, P1, P2> {
private:
    RET (T::*m_Func) (P1,P2);
    T* m_Obj;

public:
    /// @brief The class constructor with instance and members.
    ///	@param	obj				[in] A pointer showing to the object to get called.
    ///	@param	( T::*func )	[in] The method pointer to call.
    ///	@param	P1				[in] Parameter one.
    ///	@param	P2				[in] Parameter two.
    FunctorMember( T* obj, RET ( T::*func ) ( P1,P2 ) ) :
            m_Func( func ),
            m_Obj( obj ) {
        // empty
    }

    /// @brief The class destructor.
    virtual ~FunctorMember() {
        // empty
    }

    /// @brief Calls the member
    ///	@param	P1		[in] Parameter one.
    ///	@param	P2		[in] Parameter two.
    ///	@return	The return value.
    virtual RET Call(P1 p1, P2 p2) const {
        return ( m_Obj->*m_Func )( p1, p2 );
    }
};

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Common::Functor
///	@ingroup	Infrastructure
///
/// @brief Functor implementation.
//-------------------------------------------------------------------------------------------------
template < class RET, class P1, class P2 >
class Functor {
public:
    /// @brief The class default constructor
    Functor() :	
            m_Data( NULL ), 
            m_RefCounter( NULL ) {
        // empty 
    }
    
    /// @brief The class copy constructor
    Functor( const Functor& f ) :
            m_Data( f.m_Data ), 
            m_RefCounter( f.m_RefCounter ) {
        ++(*m_RefCounter);
    }

    /// @brief The class destructor.
    virtual ~Functor() {
        if ( 0L != m_RefCounter ) {
            if (--(*m_RefCounter) <= 0 ) {
                delete m_RefCounter;
                delete m_Data;
            }
        }
    }

    /// @brief () operator implementation, performs function call
    /// @param p1	Parameter 1
    /// @param p1	Parameter 2
    /// @return return type
    RET operator () (P1 p1, P2 p2) const {
        // Check for a valid this pointer
        if (NULL != m_Data) {
            return ( m_Data->Call( p1, p2 ) );
        }

        // Just return the default
        return RET();
    }

    /// @brief == operator implementation
    bool operator == (const Functor& other) const {
        return (m_Data == other.m_Data);
    }

    /// @brief Binding to function by a function pointer
    static Functor Make(RET (*func) (P1,P2)) {
        return Functor(new FunctorFunction<RET, P1, P2>(func));
    }

    /// @brief Binds a member and an instance
    template<class T>
    static Functor Make(T* obj, RET (T::*func) (P1,P2)) {
        return ( Functor( new FunctorMember<T, RET, P1, P2>( obj, func ) ) );
    }

private:
    /// @brief The constructor with the functor.
    ///	@param	d	The functor.
    Functor( FunctorImpl<RET, P1, P2>* d ) :
            m_Data( d ),
            m_RefCounter( new ui32( 1 ) ) {
        // empty
    }

private:
    FunctorImpl<RET, P1, P2> *m_Data;
    ui32* m_RefCounter;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Common
} // Namespace OSRE
