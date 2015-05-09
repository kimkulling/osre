#pragma once

#include <osre/Common/Types.h>

namespace OSRE {
namespace Platform  {

//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Platform::AbstractThreadEvent
///	@ingroup	Infrastructure
///
///	@brief
//-------------------------------------------------------------------------------------------------
class AbstractThreadEvent {
public:
	///	@brief	The class destructor, virtual.
	virtual ~AbstractThreadEvent();

	///	@brief	Signals the class.
	virtual void signal() = 0;

	///	@brief	Waits until the class will be signaled.
	virtual void wait() = 0;

	///	@brief	Wait for a signal or the timeout has passed.
	///	@param	ms	[in] The timeout in ms.
	virtual void waitForTimeout( ui32 ms ) = 0;
};

//-------------------------------------------------------------------------------------------------
inline
AbstractThreadEvent::~AbstractThreadEvent( ) {
	// empty
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
