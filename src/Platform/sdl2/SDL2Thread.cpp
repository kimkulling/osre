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
#include "SDL2Thread.h"
#include "SDL2ThreadEvent.h"
#include "SDL2CriticalSection.h"

#include <osre2/Infrastructure/Debugging/ce_assert.h>
#include <osre2/Infrastructure/System/SystemInfo.h>
#include <osre2/Infrastructure/Common/Logger.h>

#include "SDL_thread.h"

#include <iostream>

namespace ZFXCE2 {
namespace Platform {

//-------------------------------------------------------------------------------------------------
SDL2Thread::SDL2Thread( const ce_string &name, ui32 stacksize ) 
:  m_thread( nullptr )
, m_pThreadSignal( nullptr )
, m_Prio( Normal )
, m_ThreadState( New )
, m_ThreadName( name )
, m_StackSize( stacksize )
, m_threadId( 0 ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
SDL2Thread::~SDL2Thread( ) {
    if ( Running == m_ThreadState ) {
        ce_debug( "Thread " + getName() + " is still running." );
        SDL2Thread::stop( );
    }
}

//-------------------------------------------------------------------------------------------------
bool SDL2Thread::start( void *pData ) {
    if ( Running == m_ThreadState ) {
        ce_debug( "Thread " + getName() + " is already running." );
        return false;
    }
    if ( !pData ) {
        pData = this;
    }

    bool result( true );
    m_thread = SDL_CreateThread( SDL2Thread::sdl2threadfunc, getName( ).c_str( ), pData );
    if( m_thread ) {
        setState( Running );
    } else {
        // cannot start thread, so it's terminated
        setState( AbstractThread::Terminated );
        result = false;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------
bool SDL2Thread::stop( ) {
    if ( !Running == m_ThreadState ) {
        ce_debug( "Thread " + getName() + " is not running." );
        return false;
    }

    SDL_DetachThread( m_thread );
    m_thread = nullptr;

    // update state and release signal
    setState( Terminated );
    delete m_pThreadSignal;
    m_pThreadSignal = nullptr;

    System::SystemInfo::ThreadId id;
    System::SystemInfo::unregisterThreadName( id );

    return true;
}

//-------------------------------------------------------------------------------------------------
SDL2Thread::ThreadState SDL2Thread::getCurrentState( ) const {
    return m_ThreadState;
}

//-------------------------------------------------------------------------------------------------
bool SDL2Thread::suspend( ) {
    // check for a valid thread state
    if ( Running == m_ThreadState ) {
        ce_debug( "Thread " + getName() + " is not running." );
        return false;
    }

    // suspend the thread

    setState( Suspended );

    return true;
}

//-------------------------------------------------------------------------------------------------
bool SDL2Thread::resume( ) {
    // check for a valid thread state
    if ( Waiting != m_ThreadState ) {
        ce_debug( "Thread " + getName() + " is not suspended." );
        return false;
    }

    // resume the thread
    setState( Running );

    return true;
}

//-------------------------------------------------------------------------------------------------
void SDL2Thread::setName( const ce_string &name ) {
    ce_assert( !name.empty( ) );

    m_ThreadName = name;
}

//-------------------------------------------------------------------------------------------------
const ce_string &SDL2Thread::getName( ) const {
    return m_ThreadName;
}

//-------------------------------------------------------------------------------------------------
void SDL2Thread::setStackSize( ui32 stacksize ) {
    m_StackSize = stacksize;
}

//-------------------------------------------------------------------------------------------------
ui32 SDL2Thread::getStackSize( ) const {
    return m_StackSize;
}

//-------------------------------------------------------------------------------------------------
void SDL2Thread::waitForTimeout( ui32 ms ) {
    if ( !m_pThreadSignal ) {
        ce_debug( "Invalid pointer to thread signal." );
        return;
    } else {
        m_pThreadSignal->waitForTimeout( ms );
    }
}

//-------------------------------------------------------------------------------------------------
void SDL2Thread::wait( ) {
    if ( !m_pThreadSignal ) {
        ce_debug( "Invalid pointer to thread signal." );
        return;
    } else {
        m_pThreadSignal->wait();
    }
}

//-------------------------------------------------------------------------------------------------
AbstractThreadEvent *SDL2Thread::getThreadEvent( ) const {
    return m_pThreadSignal;
}

//-------------------------------------------------------------------------------------------------
void SDL2Thread::setPriority( Priority prio ) {
    m_Prio = prio;
}

//-------------------------------------------------------------------------------------------------
SDL2Thread::Priority SDL2Thread::getPriority( ) const {
    return m_Prio;
}

//-------------------------------------------------------------------------------------------------
const ce_string &SDL2Thread::getThreadName() const {
    return m_ThreadName;
}

//-------------------------------------------------------------------------------------------------
i32 SDL2Thread::sdl2threadfunc( void *data ) {
    i32 retCode( 0 );
    if( data ) {
        SDL2Thread *instance = ( SDL2Thread* ) data;
        switch( instance->getPriority() ) {
            case Low:
                retCode = SDL_SetThreadPriority( SDL_THREAD_PRIORITY_LOW );
                break;
            case Normal:
                retCode = SDL_SetThreadPriority( SDL_THREAD_PRIORITY_NORMAL );
                break;
            case High:
                retCode = SDL_SetThreadPriority( SDL_THREAD_PRIORITY_HIGH );
                break;
            default:
                retCode = 1;
                break;
        }
        if( 0 == retCode ) {
            System::SystemInfo::ThreadId id;
            id.Id = ( unsigned long ) SDL_ThreadID();
            System::SystemInfo::registerThreadName( id, instance->getName() );
            instance->m_threadId = id.Id;
            retCode = instance->run();
        }
    } else {
        ce_error( "Invalid thread data." );
    }

    return retCode;
}

//-------------------------------------------------------------------------------------------------
i32 SDL2Thread::run( ) {
    // Override me!

    return 0;
}
//-------------------------------------------------------------------------------------------------
void SDL2Thread::setState( ThreadState newState ) {
    m_ThreadState = newState;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
