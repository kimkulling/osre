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
#include "SDL2ThreadFactory.h"
#include "SDL2CriticalSection.h"
#include "SDL2ThreadEvent.h"
#include "SDL2Thread.h"
#include "SDL2Atomic.h"

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
SDL2ThreadFactory::SDL2ThreadFactory( )
: AbstractThreadFactory( "platform/sdl2threadfactory" ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
SDL2ThreadFactory::~SDL2ThreadFactory( ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
AbstractThread *SDL2ThreadFactory::createThread( const String &name, ui32 stacksize ) {
    return new SDL2Thread( name, stacksize );
}

//-------------------------------------------------------------------------------------------------
AbstractCriticalSection *SDL2ThreadFactory::createCriticalSection( ) {
    return new SDL2CriticalSection;
}

//-------------------------------------------------------------------------------------------------
AbstractThreadEvent *SDL2ThreadFactory::createThreadEvent( ) {
    return new SDL2ThreadEvent;
}

//-------------------------------------------------------------------------------------------------
AbstractAtomic *SDL2ThreadFactory::createAtomic( i32 val ) {
    return new SDL2Atomic( val );
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace OSRE
