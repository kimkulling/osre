#pragma once

/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-2015, ZFXCE2 Development Team
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
#pragma once

#include <osre/Common/Types.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Platform::AbstractAtomic
///	@ingroup	Infrastructure
///
///	@brief  This is the abstract base class for all atomic implementations.
//-------------------------------------------------------------------------------------------------
class AbstractAtomic {
public:
    /// @brief  The class destructor, virtual.
    virtual ~AbstractAtomic();

    /// @brief  Increments the int.
    /// @param  value   [in] Increment value.
    virtual void incValue( i32 value ) = 0;

    /// @brief  Decrements the int.
    /// @param  value   [in] Decrement value.
    virtual void decValue( i32 value ) = 0;
    
    /// @brief  Will return the atomic in value.
    /// @return The value.
    virtual i32 getValue( ) = 0;
    
    /// @brief  Increment by one.
    /// @return The new value
    virtual i32 inc() = 0;

    /// @brief  Decrement by one.
    /// @return The new value
    virtual i32 dec() = 0;
};

//-------------------------------------------------------------------------------------------------
inline
AbstractAtomic::~AbstractAtomic() {
    // empty
}

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::AtomicInt
///	@ingroup	Infrastructure
///
///	@brief  Implements the accessor for an atomic operation.
//-------------------------------------------------------------------------------------------------
class AtomicInt {
public:
    /// @brief  The class constructor.
    /// @param  val     [in] The initial value.
    AtomicInt( i32 val );

    /// @brief  The class destructor.
    ~AtomicInt();

    /// @brief  increments the int.
    /// @param  value   [in] Increment value.
    void incValue( i32 value );

    /// @brief  Decrements the int.
    /// @param  value   [in] Decrement value.
    void decValue( i32 value );
    
    /// @brief  Returns the value.
    /// @return The value.
    i32 getValue() const;
    
    /// @brief  ++ op impl.
    i32 operator ++ ();

    /// @brief  ++ op impl.
    i32 operator -- ( );

private:
    AbstractAtomic *m_pImpl;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
