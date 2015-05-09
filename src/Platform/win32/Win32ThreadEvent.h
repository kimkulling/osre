#pragma once

#include <osre/Platform/AbstractThreadEvent.h>

#include <windows.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Platform::ITaskManager
///	@ingroup	Infrastructure
///
///	@brief
//-------------------------------------------------------------------------------------------------
class Win32ThreadEvent : public AbstractThreadEvent {
public:
	///	The constructor.
	Win32ThreadEvent();
	///	The destructor.
	~Win32ThreadEvent();
	///	The event will be signaled.
	void signal();
	///	Wait until the event is signaled.
	void wait();
	///	Wait until the event is signaled until a given timeout.
	void waitForTimeout( ui32 ms );
	///	The handle of the event will be returned.
	HANDLE getHandle() const;

private:
	HANDLE m_EventHandle;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
