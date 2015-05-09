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
#include "Win32ThreadFactory.h"
#include "Win32CriticalSection.h"
#include "Win32ThreadEvent.h"
#include "Win32Thread.h"
#include "Win32Atomic.h"

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
Win32ThreadFactory::Win32ThreadFactory() 
: AbstractThreadFactory( "threading/win32threadfactory" ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
Win32ThreadFactory::~Win32ThreadFactory() {
    // empty
}

//-------------------------------------------------------------------------------------------------
AbstractThread *Win32ThreadFactory::createThread( const String &name, ui32 stacksize ) {
    return new Win32Thread( name, stacksize );
}

//-------------------------------------------------------------------------------------------------
AbstractCriticalSection *Win32ThreadFactory::createCriticalSection( ) {
    return new Win32CriticalSection;
}

//-------------------------------------------------------------------------------------------------
AbstractThreadEvent *Win32ThreadFactory::createThreadEvent() {
    return new Win32ThreadEvent;
}

//-------------------------------------------------------------------------------------------------
AbstractAtomic *Win32ThreadFactory::createAtomic( i32 val ) {
    return new Win32Atomic( val );
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
