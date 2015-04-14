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

//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Platform::AbstractTimer
///	@ingroup	Infrastructure
///
///	@brief This class declares the abstract interface for timer implementations.
///
///	A timer offers
///	the milliseconds, which are past since starting the application. You can use it as a way to
///	synchronize your application.
///
///	@remark	If you are implementing your own timer be careful with the resolution of your selected
///	timer interface.
//-------------------------------------------------------------------------------------------------
class DLL_EXPORT AbstractTimer : public Common::Object {
public:
    ///	@brief	Destructor, virtual.
    virtual ~AbstractTimer();

    ///	@brief	Returns the seconds since starting the application.
    ///	@return	Seconds past since starting the application.
    virtual d32 getCurrentSeconds() = 0;

    ///	@brief	Returns the difference since the last call of getTimeDiff.
    ///	@return	The time difference.
    virtual d32 getTimeDiff() = 0;

protected:
    ///	@brief	The constructor with the name of the timer instance.
    ///	@param	name        [in] The name for the timer instance.
    /// @param  reqTimeStep [in] The time-step for the target FPS-value.
    AbstractTimer( const String &name, d32 reqTimeStep = 1.0 / 30.0 );

    /// @brief  Will return the target time slice for 
    d32 getRequestedTimeStep() const;

private:
    d32 m_reqTimeSlice;
};

//-------------------------------------------------------------------------------------------------
inline
AbstractTimer::AbstractTimer( const String &name, d32 reqTimeSlice )
: Object( name )
, m_reqTimeSlice( reqTimeSlice ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline
AbstractTimer::~AbstractTimer() {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline
d32 AbstractTimer::getRequestedTimeStep() const {
    return m_reqTimeSlice;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
