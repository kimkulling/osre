/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-214, ZFXCE2 Development Team
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
#ifndef ZFXCE2_INFRASTRUCTURE_PLATFORM_WIN32EVENTHANDLER_H_INC
#define ZFXCE2_INFRASTRUCTURE_PLATFORM_WIN32EVENTHANDLER_H_INC

#include <osre2/Infrastructure/Platform/AbstractPlatformEventHandler.h>
#include <cppCommon/Container/TArray.h>

#include <windows.h>
#include <map>

namespace ZFXCE2 {

namespace Common {
    class Event;
    class EventData;
    class EventTriggerer;
}

namespace Platform {

class AbstractSurface;
class OSEventListener;

struct IInputUpdate;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::Win32Eventhandler
///	@ingroup	Infrastructure
///
///	@brief  This class implements the win32-specific event handler for OS-events.
//-------------------------------------------------------------------------------------------------
class DLL_EXPORT Win32Eventhandler : public AbstractPlatformEventHandler {
public:
    Win32Eventhandler();
    virtual ~Win32Eventhandler();
    virtual bool onEvent( const Common::Event &ev, const Common::EventData *pEventData );
    static LRESULT CALLBACK winproc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );
    void setRootSurface( AbstractSurface *pWIndow );
    AbstractSurface *getRootSurface() const;
    void enablePolling( bool enabled );
    bool isPolling() const;
    void registerEventListener( const CPPCommon::TArray<const Common::Event*> &rEvents, OSEventListener *pListener );
    void unregisterEventListener( const CPPCommon::TArray<const Common::Event*> &rEvents, OSEventListener *pListener );
    static void registerEventServer( Win32Eventhandler *pServer, HWND hWnd );
    static void unregisterEventServer( Win32Eventhandler *pServer, HWND hWnd );
    static Win32Eventhandler *getInstance( HWND hWnd );

protected:
    virtual bool onAttached( const Common::EventData *pEventData );
    virtual bool onDetached( const Common::EventData *pEventData );
    virtual bool onQuit();


private:
    static std::map<HWND, Win32Eventhandler*> s_WindowsServerMap;
    IInputUpdate *m_pUpdateInstance;
    Common::EventTriggerer *m_pOSEventTriggerer;
    AbstractSurface *m_pRootSurface;
    bool m_shutdownRequested;
    bool m_isPolling;

};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2

#endif // ZFXCE2_INFRASTRUCTURE_PLATFORM_WIN32EVENTHANDLER_H_INC
