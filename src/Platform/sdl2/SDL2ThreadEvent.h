#pragma once

#include <osre/Platform/AbstractThreadEvent.h>

struct SDL_cond;
struct SDL_mutex;

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::SDL2ThreadEvent
///	@ingroup	Infrastructure
///
///	@brief
//-------------------------------------------------------------------------------------------------
class SDL2ThreadEvent : public AbstractThreadEvent {
public:
    ///	The class constructor.
    SDL2ThreadEvent( );
    ///	The class destructor.
    ~SDL2ThreadEvent( );
    ///	The event will be signaled.
    void signal();
    ///	Wait until the event is signaled.
    void wait();
    ///	Wait until the event is signaled until a given timeout.
    void waitForTimeout( ui32 ms );

private:
    i32 m_bool;
    SDL_mutex *m_lock;
    SDL_cond *m_event;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
