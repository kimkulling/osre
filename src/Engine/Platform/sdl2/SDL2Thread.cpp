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
#include "SDL2Thread.h"
#include "SDL2ThreadEvent.h"
#include "SDL2CriticalSection.h"
#include "SDL2ThreadLocalStorage.h"

#include <osre/Platform/SystemInfo.h>
#include <osre/Common/Logger.h>

#include "SDL_thread.h"

#include <iostream>
#include <cassert>

namespace OSRE {
namespace Platform {

static const String Tag = "SDL2Thread";

SDL2Thread::SDL2Thread( const String &name, ui32 stacksize )
:  m_thread( nullptr )
, m_threadSignal( nullptr )
, m_tls( nullptr )
, m_Prio( Priority::Normal )
, m_threadName( name )
, m_id() {
    // empty
}

SDL2Thread::~SDL2Thread( ) {
    if ( ThreadState::Running == AbstractThread::getCurrentState() ) {
        osre_debug( Tag, "Thread " + getName() + " is still running." );
        SDL2Thread::stop( );
    }
}

bool SDL2Thread::start( void *data ) {
    if ( ThreadState::Running == AbstractThread::getCurrentState() ) {
        osre_debug( Tag, "Thread " + getName() + " is already running." );
        return false;
    }
    if ( nullptr == data ) {
        data = this;
    }

    bool result( true );
    m_thread = SDL_CreateThread( SDL2Thread::sdl2threadfunc, getName( ).c_str( ), data );
    if( m_thread ) {
        setState( ThreadState::Running );
    } else {
        // cannot start thread, so it's terminated
        setState( ThreadState::Terminated );
        result = false;
    }

    return result;
}

bool SDL2Thread::stop( ) {
    if ( ThreadState::Running != AbstractThread::getCurrentState()) {
        osre_debug( Tag, "Thread " + getName() + " is not running." );
        return false;
    }

    SDL_DetachThread( m_thread );
    m_thread = nullptr;

    // update state and release signal
    setState( ThreadState::Terminated );
    delete m_threadSignal;
    m_threadSignal = nullptr;

    SystemInfo::unregisterThreadName( getThreadId() );

    return true;
}

bool SDL2Thread::suspend( ) {
    // check for a valid thread state
    if ( ThreadState::Running == AbstractThread::getCurrentState() ) {
        osre_debug( Tag, "Thread " + getName() + " is not running." );
        return false;
    }

    // suspend the thread
    setState( ThreadState::Suspended );

    return true;
}

bool SDL2Thread::resume( ) {
    // check for a valid thread state
    if ( ThreadState::Waiting != AbstractThread::getCurrentState()) {
        osre_debug( Tag, "Thread " + getName() + " is not suspended." );
        return false;
    }

    // resume the thread
    setState( ThreadState::Running );

    return true;
}

void SDL2Thread::setName( const String &name ) {
    assert( !name.empty( ) );

    m_threadName = name;
}

const String &SDL2Thread::getName( ) const {
    return m_threadName;
}

void SDL2Thread::waitForTimeout( ui32 ms ) {
    if ( !m_threadSignal ) {
        osre_debug( Tag, "Invalid pointer to thread signal." );
        return;
    } else {
        m_threadSignal->waitForTimeout( ms );
    }
}

void SDL2Thread::wait( ) {
    if ( !m_threadSignal ) {
        osre_debug( Tag, "Invalid pointer to thread signal." );
        return;
    } else {
        m_threadSignal->waitForOne();
    }
}

AbstractThreadEvent *SDL2Thread::getThreadEvent( ) const {
    return m_threadSignal;
}

void SDL2Thread::setPriority( Priority prio ) {
    m_Prio = prio;
}

SDL2Thread::Priority SDL2Thread::getPriority( ) const {
    return m_Prio;
}

const String &SDL2Thread::getThreadName() const {
    return m_threadName;
}

AbstractThreadLocalStorage *SDL2Thread::getThreadLocalStorage() {
    return m_tls;
}

void SDL2Thread::setThreadLocalStorage( AbstractThreadLocalStorage *tls ) {
    m_tls = ( SDL2ThreadLocalStorage*) tls;
}

void SDL2Thread::setThreadId(const ThreadId &id) {
    m_id = id;
}

ThreadId SDL2Thread::getThreadId() {
    return m_id;
}

i32 SDL2Thread::sdl2threadfunc( void *data ) {
    i32 retCode( 0 );
    if( data ) {
        SDL2Thread *instance = ( SDL2Thread* ) data;
        switch( instance->getPriority() ) {
            case Priority::Low:
                retCode = SDL_SetThreadPriority( SDL_THREAD_PRIORITY_LOW );
                break;
            case Priority::Normal:
                retCode = SDL_SetThreadPriority( SDL_THREAD_PRIORITY_NORMAL );
                break;
            case Priority::High:
                retCode = SDL_SetThreadPriority( SDL_THREAD_PRIORITY_HIGH );
                break;
            default:
                retCode = 1;
                break;
        }
        if( 0 == retCode ) {
            ThreadId id;
            id.Id = ( unsigned long ) SDL_ThreadID();
            instance->setThreadId( id );
            SystemInfo::registerThreadName( id, instance->getName() );
            retCode = instance->run();
        }
    } else {
        osre_error( Tag, "Invalid thread data." );
    }

    return retCode;
}

i32 SDL2Thread::run( ) {
    // Override me!

    return 0;
}

} // Namespace Platform
} // Namespace OSRE
