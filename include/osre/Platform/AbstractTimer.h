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
class AbstractTimer : public Common::Object {
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
