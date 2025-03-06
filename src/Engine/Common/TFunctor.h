/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "Common/osre_common.h"

namespace OSRE::Common {

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief Base abstract interface of functor implementation
//-------------------------------------------------------------------------------------------------
template <class RET, class P1, class P2>
class FunctorImpl {
public:
    /// @brief The virtual destructor.
    virtual ~FunctorImpl() = default;

    /// @brief Call for overwrite.
    ///	@param	p1	UniformVar 1.
    ///	@param	p2	UniformVar 2.
    ///	@return	The return value.
    virtual RET call(P1 p1, P2 p2) const = 0;
};

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
    FunctorFunction(FunctorFunctionType func) :
            mFunc(func) {
        // empty
    }

    /// @brief Virtual destructor.
    virtual ~FunctorFunction() = default;

    /// @brief Performs the binded function call
    ///	@param	p1	UniformVar 1.
    ///	@param	p2	UniformVar 1.
    RET call(P1 p1, P2 p2) const override {
        return mFunc(p1, p2);
    }

private:
    FunctorFunctionType mFunc;
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
            mFunc(func),
            mObj(obj) {
        // empty
    }

    /// @brief The class destructor.
    virtual ~FunctorMember() = default;

    /// @brief Calls the member
    ///	@param	P1		[in] UniformVar one.
    ///	@param	P2		[in] UniformVar two.
    ///	@return	The return value.
    RET call(P1 p1, P2 p2) const override {
        return (mObj->*mFunc)(p1, p2);
    }

private:
    RET(T::*mFunc)
    (P1, P2);
    T *mObj;
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
            mData(nullptr),
            mRefCounter(nullptr) {
        // empty
    }

    /// @brief The class copy constructor
    Functor(const Functor &f) :
            mData(f.mData), mRefCounter(f.mRefCounter) {
        ++(*mRefCounter);
    }

    /// @brief The class destructor.
    virtual ~Functor() {
        if (nullptr != mRefCounter) {
            if (--(*mRefCounter) <= 0) {
                delete mRefCounter;
                delete mData;
            }
        }
    }

    /// @brief () operator implementation, performs function call
    /// @param p1	UniformVar 1
    /// @param p1	UniformVar 2
    /// @return return type
    RET operator()(P1 p1, P2 p2) const {
        // Check for a valid this pointer
        if (nullptr != mData) {
            return (mData->call(p1, p2));
        }

        // Just return the default
        return RET();
    }

    void incRef() {
        ++(*mRefCounter);
    }

    /// @brief == operator implementation
    bool operator==(const Functor &other) const {
        return (mData == other.mData);
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
            mData(d), mRefCounter(new ui32(1)) {
        // empty
    }

private:
    FunctorImpl<RET, P1, P2> *mData;
    ui32 *mRefCounter;
};

} // Namespace OSRE::Common
