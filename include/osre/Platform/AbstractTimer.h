/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/Object.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
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
class OSRE_EXPORT AbstractTimer : public Common::Object {
public:
    ///	@brief	Destructor, virtual.
    virtual ~AbstractTimer() = default;

    ///	@brief	Returns the milli-seconds since starting the application.
    ///	@return	Seconds past since starting the application.
    virtual i64 getMilliCurrentSeconds() = 0;

    ///	@brief	Returns the difference since the last call of getTimeDiff.
    ///	@return	The time difference.
    Time getTimeDiff() {
        const i64 currentTime = getMilliCurrentSeconds();
        if (mLastTime == 0L) {
            mLastTime = currentTime;
            return 0L;
        }

        i64 diff = currentTime - mLastTime;
        if (diff > 1000L) {
            diff = AbstractTimer::getRequestedTimeStep();
        }
        mLastTime = currentTime;
        Time dt(diff);

        return dt;
    }

protected:
    ///	@brief	The constructor with the name of the timer instance.
    ///	@param	name        [in] The name for the timer instance.
    /// @param  reqTimeStep [in] The time-step for the target FPS-value.
    AbstractTimer(const String &name, i64 reqTimeStep = 1000L/60L);

    /// @brief  Will return the target time slice for 
    i64 getRequestedTimeStep() const;

private:
    i64 mReqTimeSlice;
    i64 mLastTime;
};

inline AbstractTimer::AbstractTimer(const String &name, i64 reqTimeSlice) :
        Object(name), mReqTimeSlice(reqTimeSlice), mLastTime(0l) {
    // empty
}

inline i64 AbstractTimer::getRequestedTimeStep() const {
    return mReqTimeSlice;
}

} // Namespace Platform
} // Namespace OSRE
