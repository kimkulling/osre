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
#include <osre/Platform/AbstractPlatformEventHandler.h>

#include <map>

struct SDL_Window;

namespace OSRE {
namespace Platform {

struct AbstractSDL2InputUpdate;

//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Platform::SDL2EventHandler
///	@ingroup	Infrastructure
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class SDL2EventHandler : public AbstractPlatformEventHandler {
public:
    SDL2EventHandler();
    virtual ~SDL2EventHandler();
    virtual bool onEvent( const Common::Event &event, const Common::EventData *pEventData );
    virtual void registerEventListener( const CPPCore::TArray<const Common::Event*> &events, OSEventListener *pListener );
    virtual void unregisterEventListener( const CPPCore::TArray<const Common::Event*> &events, OSEventListener *pListener );
    virtual void enablePolling( bool enabled );

protected:
    virtual bool onAttached( const Common::EventData *pEventData );
    virtual bool onDetached( const Common::EventData *pEventData );

private:
    static std::map<SDL_Window*, SDL2EventHandler*> s_windowsServerMap;
    bool m_isPolling;
    bool m_shutdownRequested;
    AbstractSDL2InputUpdate *m_pInputUpdate;
    Common::EventTriggerer *m_pEventTriggerer;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
