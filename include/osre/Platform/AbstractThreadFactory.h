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

namespace OSRE {
namespace Platform {

class AbstractCriticalSection;
class AbstractThreadEvent;
class AbstractThread;
class AbstractAtomic;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::IThreadFactory
///	@ingroup	Infrastructure
///
///	@brief	This abstract interface declares the factory methods for any thread implementation.
//-------------------------------------------------------------------------------------------------
class AbstractThreadFactory : public Common::Object {
public:
    ///	@brief	The destructor, virtual.
    virtual ~AbstractThreadFactory();

    virtual AbstractThread *createThread( const String &name, ui32 stacksize ) = 0;

    ///	@brief	Creates a new critical section.
    ///	@return	A pointer showing to the new critical section.
    virtual AbstractCriticalSection *createCriticalSection( ) = 0;

    ///	@brief	Creates a new thread event.
    ///	@return	A pointer showing to the new thread event.
    virtual AbstractThreadEvent *createThreadEvent() = 0;
    
    virtual AbstractAtomic *createAtomic( i32 val ) = 0;

    ///	@brief	Installs a new thread factory instance.
    ///	@param	pInstance	[in] A pointer showing to the new thread factory instance.
    static void setInstance( AbstractThreadFactory *pInstance );

    ///	@brief	Returns the installed thread factory.
    ///	@return	A pointer showing to the installed thread factory, equal NULL if no one is installed.
    static AbstractThreadFactory *getInstance();

protected:
    ///	@brief	The class constructor.
    ///	@param	name	[in] The name.
    AbstractThreadFactory( const String &name );

private:
    static AbstractThreadFactory *s_pThreadFactory;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace OSRE
