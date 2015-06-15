#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace Common {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Core::TArea
///	@ingroup    Engine
///
///	@brief	This class describes a simple 2D rectangle area.
//-------------------------------------------------------------------------------------------------
template<class Type>
class TArea {
public:
    ///	@brief	The default class constructor.
    TArea();

    ///	@brief	The class constructor with the width and height of the area.
    ///	@param	width	The width for the area.
    ///	@param	height	The height of the area.
    TArea( Type width, Type height );

    ///	@brief	The class copy constructor.
    ///	@param	rOther	Instance to copy from.
    TArea( const TArea<Type> &rOther );

    ///	@brief	The class destructor.
    ~TArea();

    ///	@brief	This method will set new width and height for the area.
    ///	@param	width	[in] The new width for the area.
    ///	@param	height	[in] The new height of the area.
    void set( Type width, Type height );
    
    ///	@brief	This method will return the width of the area.
    ///	@return	The width of the area will be returned.
    Type getWidth() const;

    ///	@brief	This method will return the height of the area.
    ///	@return	The height of the area will be returned.
    Type getHeight() const;

    ///	@brief	Assignment operator.
    TArea<Type> &operator = ( const TArea<Type> &rhs );
    
    ///	@brief	Compare operator.
    bool operator == ( const TArea<Type> &rhs ) const;

private:
    Type m_Width, m_Height;
};

//-------------------------------------------------------------------------------------------------
template<class Type>
inline
TArea<Type>::TArea() 
: m_Width( 0 )
, m_Height( 0 ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
template<class Type>
inline
TArea<Type>::TArea( Type width, Type height ) 
: m_Width( width )
, m_Height( height ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
template<class Type>
inline
TArea<Type>::TArea( const TArea<Type> &rhs ) 
: m_Width( rhs.m_Width )
, m_Height( rhs.m_Height ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
template<class Type>
inline
TArea<Type>::~TArea() {
    // empty
}

//-------------------------------------------------------------------------------------------------
template<class Type>
inline
void TArea<Type>::set( Type width, Type height ) {
    m_Width = width;
    m_Height = height;
}

//-------------------------------------------------------------------------------------------------
template<class Type>
inline
Type TArea<Type>::getWidth() const {
    return m_Width;
}

//-------------------------------------------------------------------------------------------------
template<class Type>
inline
Type TArea<Type>::getHeight() const {
    return m_Height;
}

//-------------------------------------------------------------------------------------------------
template<class Type>
inline
TArea<Type> &TArea<Type>::operator = ( const TArea<Type> &rhs ) {
    if( !( *this == rhs ) )	{
        m_Width = rhs.m_Width;
        m_Height = rhs.m_Height;
    }
    
    return *this;
}

//-------------------------------------------------------------------------------------------------
template<class Type>
inline
bool TArea<Type>::operator == ( const TArea<Type> &rhs ) const {
    return ( m_Width == rhs.m_Width && m_Height == rhs.m_Height );
}

//-------------------------------------------------------------------------------------------------

} // Namespace Core
} // Namespace ZFXCE2
