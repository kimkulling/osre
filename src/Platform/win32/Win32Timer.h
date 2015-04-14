/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011, ZFXCE2 Development Team
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
#ifndef ZFXCE2_INFRASTRUCTURE_SYSTEM_WIN32TIMER_H_INC
#define ZFXCE2_INFRASTRUCTURE_SYSTEM_WIN32TIMER_H_INC

#include <osre2/Infrastructure/Platform/AbstractTimer.h>
#include <windows.h>

namespace ZFXCE2 {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::Win32Timer
///	@ingroup	Infrastructure
///
///	@brief	This class implements a windows specific timer.
//-------------------------------------------------------------------------------------------------
class DLL_EXPORT Win32Timer : public AbstractTimer {
public:
	///	The class constructor.
	Win32Timer();
	///	The class destructor.
	~Win32Timer();
	///	Seconds getter.
	d32 getCurrentSeconds();
	///	Time difference getter.
	d32 getTimeDiff();

private:
	LARGE_INTEGER m_globeTime, m_globeFrequency;
	d32 m_LastTime;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2

#endif // ZFXCE2_INFRASTRUCTURE_SYSTEM_WIN32TIMER_H_INC
