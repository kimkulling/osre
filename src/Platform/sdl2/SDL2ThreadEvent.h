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

#include <osre/Platform/AbstractThreadEvent.h>

struct SDL_cond;
struct SDL_mutex;

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Platform::SDL2ThreadEvent
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
class SDL2ThreadEvent : public AbstractThreadEvent {
public:
    ///	The class constructor.
    SDL2ThreadEvent( );
    ///	The class destructor.
    ~SDL2ThreadEvent( );
    ///	The event will be signaled.
    void signal();
    /// Wait for one when signaled.
    void waitForOne();
    /// Wait for all when signaled.
    void waitForAll();
    ///	Wait until the event is signaled until a given timeout.
    void waitForTimeout( ui32 ms );

private:
    i32 m_bool;
    SDL_mutex *m_lock;
    SDL_cond *m_event;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace OSRE
