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
class DLL_EXPORT AbstractThreadFactory : public Common::Object {
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
} // Namespace ZFXCE2
