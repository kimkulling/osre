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

#include <osre/Common/osre_common.h>
#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace Common {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Common::IObject
///	@ingroup    Engine
///
///	@brief	This base-class implements a simple reference counting. To get an ownership call get, 
///	to release it call release. Objects with a reference count of 0 will be destroyed.
///	You can assign an object name to the instance. 
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Object {
public:
    ///	@brief	The class destructor, virtual.
    virtual ~Object();

    ///	@brief	INcrease reference counter by one.
    void get();

    ///	@brief	Will release a shared ownership, the reference count will be decreased by one.
    void release();

    ///	@brief	Returns the number of shared references.
    ///	@return	The number of shared references.
    ui32 getNumRefs() const;

    ///	@brief	A new object name will be assigned.
    ///	@param	objName     [in] The new object name for the instance.
    void setName( const String &objName );

    ///	@brief	The name of the object will be returned.
    ///	@return	The name of the object.
    const String &getName() const;

protected:
    ///	@brief	The constructor with the name of the object.
    ///	@param	objectName  [in] The object name.
    Object( const String &objectName );

private: 
    Object();
    Object( const Object & );
    Object &operator = ( const Object & );

private:
    String m_ObjectName;
    ui32 m_Refcount;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Common
} // Namespace OSRE
