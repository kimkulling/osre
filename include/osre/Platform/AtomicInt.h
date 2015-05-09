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
