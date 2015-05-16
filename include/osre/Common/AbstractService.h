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

#include <osre/Common/Object.h>
#include <cassert>

namespace OSRE {
namespace Common {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Common::AbstractService
///	@ingroup	Infrastructure
///
///	@brief	This class is the base interface for all server classes.
/// A server provides services like a render interface for instance. To access a service you have 
/// to open it at first and close it and the end. 
//-------------------------------------------------------------------------------------------------
class AbstractService : public Object {
public:
    ///	@brief	The class destructor, virtual.
    virtual ~AbstractService();

    ///	@brief	Will open the access to the server interface, must be in closed state.
    ///	@return	true, if access was opened, false if not.
    virtual bool open();
    
    ///	@brief	Will close the access to the server interface, must be opened before.
    ///	@return	true, if access was closed, false if not.
    virtual bool close();

    ///	@brief	Will return the access state.
    ///	@return	true for open, false for close.
    virtual bool isOpen();

    ///	@brief	Will update the server.
    ///	@param	timeDiff	[in] The time difference since the last update callback.
    ///	@return	true, if update callback was successful
    virtual bool update( d32 timeDiff );

protected:
    ///	@brief	The default class constructor.
    ///	@param	serverName	[in] The server name.
    AbstractService( const String &serverName );

    /// @brief  Will be called, when server access is opened.
    /// @return true for success, false for error.
    virtual bool onOpen() = 0;

    /// @brief  Will be called, when server access will be closed.
    /// @return true for success, false for error.
    virtual bool onClose() = 0;

    /// @brief  Override this method for the update callback.
    ///	@param	timeDiff	[in] The time difference since the last update callback.
    ///	@return	true, if update callback was successful
    virtual bool onUpdate( d32 timediff ) = 0;

private:
    bool m_IsOpen;
};

//-------------------------------------------------------------------------------------------------
inline
AbstractService::AbstractService( const String &serverName )
: Object( serverName )
, m_IsOpen( false ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline 
AbstractService::~AbstractService() {
    assert( !m_IsOpen );
}

//-------------------------------------------------------------------------------------------------
inline
bool AbstractService::open() {
    if ( !m_IsOpen ) {
        m_IsOpen = true;
        return onOpen();
    }

    return false;
}

//-------------------------------------------------------------------------------------------------
inline
bool AbstractService::close() {
    if ( m_IsOpen )	{
        m_IsOpen = false;
        return onClose();
    }

    return false;
}

//-------------------------------------------------------------------------------------------------
inline
bool AbstractService::isOpen() {
    return m_IsOpen;
}

//-------------------------------------------------------------------------------------------------
inline
bool AbstractService::update(  d32 timeDiff ) {
    if ( !isOpen() ) {
        return false;
    }

    return onUpdate( timeDiff );
}

//-------------------------------------------------------------------------------------------------
/// @def        ::ZFXCE2::Common::DECLARE_SINGLETON
/// @ingroup    Infrastructure
///
/// @brief  Helper macro to define a singleton and declare all needed methods.
//-------------------------------------------------------------------------------------------------
#define DECLARE_SINGLETON( type )                                                 \
public:                                                                           \
    static type * s_pTypeInstance;                                                \
    static type * getInstance() { return s_pTypeInstance; }                       \
    static void   setInstance( type *pInstance ) { s_pTypeInstance = pInstance; } \
private:

//-------------------------------------------------------------------------------------------------
/// @def        ::ZFXCE2::Common::IMPLEMENT_SINGLETON
/// @ingroup    Infrastructure
///
/// @brief  Helper macro to define a singleton and implement all needed methods.
//-------------------------------------------------------------------------------------------------
#define IMPLEMENT_SINGLETON( type )                 \
    type * type::s_pTypeInstance = nullptr;

//-------------------------------------------------------------------------------------------------
/// @def        ::ZFXCE2::Common::CREATE_SINGLETON
/// @ingroup    Infrastructure
///
/// @brief  Helper macro to create a macro from a given class.
//-------------------------------------------------------------------------------------------------
#define CREATE_SINGLETON( type )                    \
    assert( nullptr == s_pTypeInstance );        \
    s_pTypeInstance = this;         

//-------------------------------------------------------------------------------------------------
/// @def        ::ZFXCE2::Common::DESTROY_SINGLETON
/// @ingroup    Infrastructure
///
/// @brief  Helper macro to destroy a macro from a given class.
//-------------------------------------------------------------------------------------------------
#define DESTROY_SINGLETON( type )                   \
    assert( nullptr != s_pTypeInstance );        \
    s_pTypeInstance = nullptr;         

//-------------------------------------------------------------------------------------------------

} // Namespace Common
} // Namespace OSRE
