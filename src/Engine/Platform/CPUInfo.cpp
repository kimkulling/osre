/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Platform/CPUInfo.h>
#ifdef OSRE_WINDOWS
#  include <osre/Platform/Windows/MinWindows.h>
#else
#   include <unistd.h>
#endif

namespace OSRE {
namespace Platform {

bool CPUInfo::m_IsInited  = false;
i32  CPUInfo::m_CPUFlags  = CPUID_None;
ui32 CPUInfo::m_NumCPUs   = 0;

//-------------------------------------------------------------------------------------------------
//	Looks for the CPU ids.
//-------------------------------------------------------------------------------------------------
#ifdef OSRE_WINDOWS
/* static bool hasCPUId() {
    __asm {
        pushfd						// save eflags
            pop		eax
            test	eax, 0x00200000		// check ID bit
            jz		set21				// bit 21 is not set, so jump to set_21
            and		eax, 0xffdfffff		// clear bit 21
            push	eax					// save new value in register
            popfd						// store new value in flags
            pushfd
            pop		eax
            test	eax, 0x00200000		// check ID bit
            jz		good
            jmp		err					// cpuid not supported
set21:
        or		eax, 0x00200000		// set ID bit
            push	eax					// store new value
            popfd						// store new value in EFLAGS
            pushfd
            pop		eax
            test	eax, 0x00200000		// if bit 21 is on
            jnz		good
            jmp		err
    }

err:
    return false;
good:
    return true;
}

#define _REG_EAX		0
#define _REG_EBX		1
#define _REG_ECX		2
#define _REG_EDX		3

static void CPUID( int func, unsigned regs[ 4 ] ) {
    unsigned regEAX, regEBX, regECX, regEDX;

    __asm pusha
        __asm mov eax, func
        __asm __emit 00fh
        __asm __emit 0a2h
        __asm mov regEAX, eax
        __asm mov regEBX, ebx
        __asm mov regECX, ecx
        __asm mov regEDX, edx
        __asm popa

    regs[ _REG_EAX ] = regEAX;
    regs[ _REG_EBX ] = regEBX;
    regs[ _REG_ECX ] = regECX;
    regs[ _REG_EDX ] = regEDX;
}

static bool isAMD() {
    c8 pstring[ 16 ];
    c8 processorString[ 13 ];

    // get name of processor
    CPUID( 0, ( unsigned int * ) pstring );
    processorString[ 0 ] = pstring[4];
    processorString[ 1 ] = pstring[5];
    processorString[ 2 ] = pstring[6];
    processorString[ 3 ] = pstring[7];
    processorString[ 4 ] = pstring[12];
    processorString[ 5 ] = pstring[13];
    processorString[ 6 ] = pstring[14];
    processorString[ 7 ] = pstring[15];
    processorString[ 8 ] = pstring[8];
    processorString[ 9 ] = pstring[9];
    processorString[ 10 ] = pstring[10];
    processorString[ 11 ] = pstring[11];
    processorString[ 12 ] = 0;

    if ( strcmp( processorString, "AuthenticAMD" ) == 0 ) {
        return true;
    }
    return false;
}

static bool has3DNow() {
    unsigned regs[4];

    // check AMD-specific functions
    CPUID( 0x80000000, regs );
    if ( regs[ _REG_EAX ] < 0x80000000 ) {
        return false;
    }

    // bit 31 of EDX denotes 3DNow! support
    CPUID( 0x80000001, regs );
    if ( regs[_REG_EDX] & ( 1 << 31 ) ) {
        return true;
    }

    return false;
}

static bool hasMMX() {
    unsigned regs[ 4 ];

    // get CPU feature bits
    CPUID( 1, regs );

    // Bit 23 of EDX denotes MMX existence
    if ( regs[_REG_EDX] & ( 1 << 23 ) ) {
        return true;
    }
    return false;
}

static bool hasSSE()  {
    unsigned regs[ 4 ];

    // get CPU feature bits
    CPUID( 1, regs );

    // bit 25 of EDX denotes SSE existence
    if ( regs[_REG_EDX] & ( 1 << 25 ) ) {
        return true;
    }
    return false;
}

static bool hasSSE2() {
    unsigned regs[ 4 ];

    // Get CPU feature bits
    CPUID( 1, regs );

    // bit 26 of EDX denotes SSE2 existence
    if ( regs[ _REG_EDX ] & ( 1 << 26 ) ) {
        return true;
    }
    return false;
}

static bool hasSSE3() {
    unsigned regs[4];

    // Get CPU feature bits
    CPUID( 1, regs );

    // bit 0 of ECX denotes SSE3 existence
    if ( regs[_REG_ECX] & ( 1 << 0 ) ) {
        return true;
    }
    return false;
}*/
#endif

CPUInfo::CPUInfo() {
    // empty
}

//-------------------------------------------------------------------------------------------------
CPUInfo::~CPUInfo() {
    // empty
}

ui32 CPUInfo::getNumCPUs() const {
    return m_NumCPUs;
}

i32 CPUInfo::getCPUProperties() const {
    return m_CPUFlags;
}

bool CPUInfo::isInited() {
    return m_IsInited;
}

bool CPUInfo::init() {
    if( m_IsInited ) {
        return true;
    }

    if ( !initCPUProperties() ) {
        return false;
    }

    m_IsInited = true;

    return true;
}

bool CPUInfo::initCPUProperties() {
    m_CPUFlags = CPUID_None;
#ifdef OSRE_WINDOWS
/*    if( !hasCPUId() ) {
        m_CPUFlags |= CPUID_Unsupported;
        return false;
    }

    if( isAMD() ) {
        m_CPUFlags |= CPUID_AMD;
    } else {
        m_CPUFlags |= CPUID_Intel;
    }

    SYSTEM_INFO sysInfo;
    ::GetSystemInfo( &sysInfo );
    m_NumCPUs = sysInfo.dwNumberOfProcessors;
        if( has3DNow() ) {
        m_CPUFlags |= CPUID_3DNOW;
    }
    if( hasMMX() ) {
        m_CPUFlags |= CPUID_MMX;
    }

    if( hasSSE() ) {
        m_CPUFlags |= CPUID_SSE;
    }

    if( hasSSE2() ) {
        m_CPUFlags |= CPUID_SSE2;
    }

    if ( hasSSE3() ) {
        m_CPUFlags |= CPUID_SSE3;
    }
    */
#else
    m_NumCPUs = sysconf( _SC_NPROCESSORS_ONLN );
#endif

    return true;
}

} // Namespace System
} // Namespace OSRE
