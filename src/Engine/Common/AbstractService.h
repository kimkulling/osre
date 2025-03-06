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

#include "Common/Object.h"
#include "Debugging/osre_debugging.h"

namespace OSRE::Common {

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief	This class is the base interface for all server classes.
///
/// A server provides services like a render interface for instance. To access a service you have 
/// to open it at first and close it and the end. 
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractService : public Object {
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
    ///	@return	true, if update callback was successful
    virtual bool update();

protected:
    ///	@brief The default class constructor.
    ///	@param[in] serviceName    The server name.
    AbstractService( const String &serviceName );

    /// @brief  Will be called, when server access is opened.
    /// @return true for success, false for error.
    virtual bool onOpen() = 0;

    /// @brief  Will be called, when server access will be closed.
    /// @return true for success, false for error.
    virtual bool onClose() = 0;

    /// @brief  Override this method for the update callback.
    ///	@param[in] timeDiff   The time difference since the last update callback.
    ///	@return	true, if update callback was successful
    virtual bool onUpdate() = 0;

private:
    bool mIsOpen;
};

inline AbstractService::AbstractService( const String &serverName ) : Object( serverName ), mIsOpen( false ) {
    // empty
}

inline AbstractService::~AbstractService() {
    osre_assert( !mIsOpen );
}

inline bool AbstractService::open() {
    if ( mIsOpen ) {
        osre_debug( getName(), "Cannot open, service already opened." );
        return false;
    }
    mIsOpen = onOpen();

    return mIsOpen;
}

inline bool AbstractService::close() {
    if ( mIsOpen )	{
        osre_debug( getName(), "Cannot close, service not open." );
        mIsOpen = false;
        return onClose();
    }

    return false;
}

inline bool AbstractService::isOpen() {
    return mIsOpen;
}

inline bool AbstractService::update() {
    if ( !isOpen() ) {
        osre_debug( getName(), "Cannot update, service not open." );
        return false;
    }

    return onUpdate();
}

//-------------------------------------------------------------------------------------------------
/// @ingroup    Engine
///
/// @brief  Helper macro to define a singleton and declare all needed methods.
//-------------------------------------------------------------------------------------------------
#define DECLARE_SINGLETON( type )                                            \
public:                                                                      \
    static type * s_instance;                                                \
    static type * getInstance() {                                            \
        osre_assert(nullptr != s_instance);                                  \
        return s_instance;                                                   \
    }                                                                        \
    static void setInstance( type *instance ) { s_instance = instance; }     \
private:

//-------------------------------------------------------------------------------------------------
/// @ingroup    Engine
///
/// @brief  Helper macro to define a singleton and implement all needed methods.
//-------------------------------------------------------------------------------------------------
#define IMPLEMENT_SINGLETON( type )                 \
    type * type::s_instance( nullptr );

//-------------------------------------------------------------------------------------------------
/// @ingroup    Engine
///
/// @brief  Helper macro to create a macro from a given class.
//-------------------------------------------------------------------------------------------------
#define CREATE_SINGLETON( type )               \
    osre_assert(nullptr == s_instance);        \
    s_instance = this;         

//-------------------------------------------------------------------------------------------------
/// @ingroup    Engine
///
/// @brief  Helper macro to destroy a macro from a given class.
//-------------------------------------------------------------------------------------------------
#define DESTROY_SINGLETON( type )              \
    osre_assert(nullptr != s_instance);        \
    s_instance = nullptr;         

} // Namespace OSRE::Common
