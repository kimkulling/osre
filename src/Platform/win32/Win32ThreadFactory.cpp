/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Win32ThreadFactory.h"
#include "Win32CriticalSection.h"
#include "Win32ThreadEvent.h"
#include "Win32Thread.h"
#include "Win32Atomic.h"

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
Win32ThreadFactory::Win32ThreadFactory() 
: AbstractThreadFactory( "platform/win32threadfactory" ) {
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
} // Namespace OSRE
