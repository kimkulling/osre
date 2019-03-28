/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Platform/SystemInfo.h>
#include <osre/Platform/CPUInfo.h>

#include <cassert>

#ifdef OSRE_WINDOWS
#  include <osre/Platform/Windows/MinWindows.h>
#else
#   include <sys/time.h>
#   include <sys/resource.h>
#   include "SDL_thread.h"
#endif

#include <sstream>

namespace OSRE {
namespace Platform {

//	The size in bytes of a KB.
enum {
    KBSize = 1024
};

bool                      SystemInfo::m_IsInited  = false;
CPUInfo                  *SystemInfo::m_pCPUInfo  = nullptr;
SystemInfo::ThreadNameMap SystemInfo::s_threadNames;

SystemInfo::SystemInfo() {
    // empty
}

SystemInfo::~SystemInfo() {
    // empty
}

SystemInfo::Platform SystemInfo::getPlatform() {
#if defined( OSRE_WINDOWS )
    return Platform::Win32;
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    return Platform::GNU_Linux;
#endif
}

void SystemInfo::getMemoryStatus( ui32 &totalPhysicMem, ui32 &memInUse ) {
    totalPhysicMem = 0;
    memInUse = 0;

#ifdef OSRE_WINDOWS
    // windows specific memory statistics
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof( statex );
    if ( ::GlobalMemoryStatusEx( &statex ) ) {
        totalPhysicMem = static_cast<ui32>( statex.ullTotalPhys / KBSize );
        memInUse = statex.dwMemoryLoad;
    }

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__) )
    struct rusage rusage;
    getrusage( RUSAGE_SELF, &rusage );
    totalPhysicMem = rusage.ru_maxrss;
    memInUse       = 0;
#endif
}

CPUInfo *SystemInfo::getCPUInfo() {
    return m_pCPUInfo;
}

bool SystemInfo::registerThreadName( const ThreadId &id, const String &name ) {
    ThreadNameMap::const_iterator it( s_threadNames.find( id.Id) );
    bool success( true );
    if ( s_threadNames.end() == it ) {
        s_threadNames[ id.Id ] = name;
    } else {
        success = false;
    }

    return success;
}

bool SystemInfo::unregisterThreadName( const ThreadId &id ) {
    ThreadNameMap::iterator it( s_threadNames.find( id.Id) );
    bool success( false );
    if ( s_threadNames.end() != it ) {
        s_threadNames.erase( it );
        success = true;
    } 

    return success;
}

String SystemInfo::getThreadName( const ThreadId &id ) {
    ThreadNameMap::const_iterator it( s_threadNames.find( id.Id) );
    if ( s_threadNames.end() != it ) {
        return it->second;
    } 
    std::stringstream stream;
    stream << "t" << id.Id;
    const String name( stream.str() );
    s_threadNames[ id.Id ] = name;
    return name;
}

String SystemInfo::getCurrentThreadName() {
    ThreadId threadId;
#ifdef OSRE_WINDOWS
    threadId.Id = ::GetCurrentThreadId();
#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__) )
    threadId.Id = (unsigned long)SDL_ThreadID();
#endif // OSRE_WINDOWS
    return getThreadName( threadId );
}

bool SystemInfo::init() {
    if ( m_IsInited ) {
        return false;
    }

    // Get the system info
#ifdef OSRE_WINDOWS
    SYSTEM_INFO systeminfo;
    ::GetSystemInfo( &systeminfo );
#else
    //TODO!
#endif

    bool success = true;
    if ( !m_pCPUInfo ) {
        if ( !CPUInfo::isInited() ) {
            success = CPUInfo::init();
        }

        m_pCPUInfo = new CPUInfo;
    }

    // Set the init state
    m_IsInited = success;

    return success;
}

} // Namespace System
} // Namespace OSRE
