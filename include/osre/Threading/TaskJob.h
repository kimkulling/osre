#pragma once

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
#ifndef CE_INFRASTRUCTURE_THREADING_TASKJOB_H_INC
#define CE_INFRASTRUCTURE_THREADING_TASKJOB_H_INC

#include <osre2/Types.h>
#include <osre2/Infrastructure/Debugging/ce_assert.h>

namespace ZFXCE2 {

namespace Common {
    class Event;
    class EventData;
}

namespace Threading {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Threading::TaskJob
///	@ingroup	Infrastructure
///
///	@brief This class implements a simple container for events as ids and the assigned data, which 
///	are implemented as subclasses from the Common::EventData class ( @see Common::EventData ).
//-------------------------------------------------------------------------------------------------
class TaskJob {
public:
    ///	@brief	The class constructor with the event and the event data.
    ///	@param	pEvent		[in] A pointer showing to the event.
    ///	@param	pEventData	[in] A pointer showing to the event data.
    TaskJob( const Common::Event *pEvent, const Common::EventData *pEventData );

    ///	@brief	The class destructor.
    ~TaskJob();

    ///	@brief	Returns a pointer showing to the assigned event.
    ///	@return	The event 
    const Common::Event *getEvent() const;

    ///	@brief	Returns a pointer showing to the assigned event data.
    ///	@return	The event data.
    const Common::EventData *getEventData() const;

    ///	@brief	Set new data.
    ///	@param	pEvent		A pointer showing to the event.
    ///	@param	pEventData	A pointer showing to the event data.
    void set( const Common::Event *pEvent, const Common::EventData *pEventData );

    ///	@brief	Clears the TaskJob-instance.
    void clear();

private:
    TaskJob();
    TaskJob( const TaskJob & );
    TaskJob &operator = ( const TaskJob & );

private:
    const Common::Event *m_pEvent;
    const Common::EventData *m_pEventData;
};

//-------------------------------------------------------------------------------------------------
inline
TaskJob::TaskJob( const Common::Event *pEvent, const Common::EventData *pEventData ) 
: m_pEvent( pEvent )
, m_pEventData( pEventData ) {
    ce_assert( nullptr != pEvent );
}

//-------------------------------------------------------------------------------------------------
inline
TaskJob::~TaskJob() {
    clear();
}

//-------------------------------------------------------------------------------------------------
inline
const Common::Event *TaskJob::getEvent() const {
    return m_pEvent;
}

//-------------------------------------------------------------------------------------------------
inline
const Common::EventData *TaskJob::getEventData() const {
    return m_pEventData;
}

//-------------------------------------------------------------------------------------------------
inline
void TaskJob::set(  const Common::Event *pEvent, const Common::EventData *pEventData ) {
    m_pEvent = pEvent;
    m_pEventData = pEventData;
}

//-------------------------------------------------------------------------------------------------
inline
void TaskJob::clear() {
    m_pEvent = nullptr;
    m_pEventData = nullptr;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2

#endif // CE_INFRASTRUCTURE_THREADING_TASKJOB_H_INC
