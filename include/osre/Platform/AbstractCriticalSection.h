#pragma once

#include <osre/Common/Types.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Platform::ICriticalSection
///	@ingroup	Infrastructure
///
///	@brief
//-------------------------------------------------------------------------------------------------
class AbstractCriticalSection {
public:
	///	@brief	The class destructor, virtual.
	virtual ~AbstractCriticalSection();

	///	@brief	The critical section will be entered.
	virtual void enter() = 0;

	///	@brief	Tries to enter the critical section.
	///	@return	true, if entering was successful, false if not.
	virtual bool tryEnter() = 0;

	///	@brief	The critical section will be leaved.
	virtual void leave() = 0;
};

//-------------------------------------------------------------------------------------------------
inline
AbstractCriticalSection::~AbstractCriticalSection( ) {
	// empty
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
