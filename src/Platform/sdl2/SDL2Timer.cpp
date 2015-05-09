#include <src/Platform/sdl2/SDL2Timer.h>

#include <SDL.h>

namespace OSRE {
namespace Platform {

static d32 InvSecToMSec = 1.0/1000.0;

//-------------------------------------------------------------------------------------------------
SDL2Timer::SDL2Timer()
: AbstractTimer( "platform/sdl2timer" )
, m_lasttick( 0 ){
    // empty
}

//-------------------------------------------------------------------------------------------------
SDL2Timer::~SDL2Timer( ) {

}

//-------------------------------------------------------------------------------------------------
d32 SDL2Timer::getCurrentSeconds( ) {
    m_lasttick = ( ( d32 ) SDL_GetTicks() ) * InvSecToMSec;
    return (  m_lasttick ) ;
}

//-------------------------------------------------------------------------------------------------
d32 SDL2Timer::getTimeDiff( ) {
    d32 last( m_lasttick );
    d32 current = getCurrentSeconds();
    d32 diff = current - last;
    if( diff > 1000.0 ) {
        return AbstractTimer::getRequestedTimeStep();
    } else {
        return current - last;
    }
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
