/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
///	@brief Base abstract interface of functor implementation
//-------------------------------------------------------------------------------------------------
template <class RET, class P1, class P2>
class FunctorImpl {
public:
    /// @brief The virtual destructor.
    virtual ~FunctorImpl();

    /// @brief Call for overwrite.
    ///	@param	p1	UniformVar 1.
    ///	@param	p2	UniformVar 2.
    ///	@return	The return value.
    virtual RET call(P1 p1, P2 p2) const = 0;
};

template <class RET, class P1, class P2>
inline FunctorImpl<RET, P1, P2>::~FunctorImpl() {
    // empty
}

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
/// @brief	This template class implements a binding to a function.
//-------------------------------------------------------------------------------------------------
template <class RET, class P1, class P2>
class FunctorFunction : public FunctorImpl<RET, P1, P2> {
public:
    ///	The function type.
    typedef RET (*FunctorFunctionType)(P1, P2);

    /// @brief The constructor with the function type.
    FunctorFunction(FunctorFunctionType func) : m_Func(func) {}

    /// @brief Virtual destructor.
    virtual ~FunctorFunction() {
        // empty
    }

    /// @brief Performs the binded function call
    ///	@param	p1	UniformVar 1.
    ///	@param	p2	UniformVar 1.
    RET call(P1 p1, P2 p2) const override {
        return m_Func(p1, p2);
    }

private:
    FunctorFunctionType m_Func;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
/// @brief	Binding to a member with the class instance.
//-------------------------------------------------------------------------------------------------
template <class T, class RET, class P1, class P2>
class FunctorMember : public FunctorImpl<RET, P1, P2> {
public:
    /// @brief The class constructor with instance and members.
    ///	@param	obj				[in] A pointer showing to the object to get called.
    ///	@param	( T::*func )	[in] The method pointer to call.
    ///	@param	P1				[in] UniformVar one.
    ///	@param	P2				[in] UniformVar two.
    FunctorMember(T *obj, RET (T::*func)(P1, P2)) :
            m_Func(func),
            m_Obj(obj) {
        // empty
    }

    /// @brief The class destructor.
    virtual ~FunctorMember() = default;

    /// @brief Calls the member
    ///	@param	P1		[in] UniformVar one.
    ///	@param	P2		[in] UniformVar two.
    ///	@return	The return value.
    RET call(P1 p1, P2 p2) const override {
        return (m_Obj->*m_Func)(p1, p2);
    }

private:
    RET(T::*m_Func)
    (P1, P2);
    T *m_Obj;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
/// @brief Functor implementation.
//-------------------------------------------------------------------------------------------------
template <class RET, class P1, class P2>
class Functor {
public:
    /// @brief The class default constructor
    Functor() noexcept :
            m_data(nullptr),
            m_refCounter(nullptr) {
        // empty
    }

    /// @brief The class copy constructor
    Functor(const Functor &f) :
            m_data(f.m_data), m_refCounter(f.m_refCounter) {
        ++(*m_refCounter);
    }

    /// @brief The class destructor.
    virtual ~Functor() {
        if (nullptr != m_refCounter) {
            if (--(*m_refCounter) <= 0) {
                delete m_refCounter;
                delete m_data;
            }
        }
    }

    /// @brief () operator implementation, performs function call
    /// @param p1	UniformVar 1
    /// @param p1	UniformVar 2
    /// @return return type
    RET operator()(P1 p1, P2 p2) const {
        // Check for a valid this pointer
        if (nullptr != m_data) {
            return (m_data->call(p1, p2));
        }

        // Just return the default
        return RET();
    }

    void incRef() {
        ++(*m_refCounter);
    }

    /// @brief == operator implementation
    bool operator==(const Functor &other) const {
        return (m_data == other.m_data);
    }

    /// @brief Binding to function by a function pointer
    static Functor make(RET (*func)(P1, P2)) {
        return Functor(new FunctorFunction<RET, P1, P2>(func));
    }

    /// @brief Binds a member and an instance
    template <class T>
    static Functor Make(T *obj, RET (T::*func)(P1, P2)) {
        return (Functor(new FunctorMember<T, RET, P1, P2>(obj, func)));
    }

private:
    /// @brief The constructor with the functor.
    ///	@param	d	The functor.
    Functor(FunctorImpl<RET, P1, P2> *d) :
            m_data(d), m_refCounter(new ui32(1)) {
        // empty
    }

private:
    FunctorImpl<RET, P1, P2> *m_data;
    ui32 *m_refCounter;
};

} // Namespace Common
} // Namespace OSRE
