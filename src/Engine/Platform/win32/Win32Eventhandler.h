/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Platform/AbstractPlatformEventHandler.h>
#include <cppcore/Container/TArray.h>

#include <windows.h>
#include <map>

namespace OSRE {

// Forward declarations
namespace Common {
    struct Event;
    struct EventData;
    class EventTriggerer;
}

namespace Platform {

class AbstractSurface;
class OSEventListener;

struct IInputUpdate;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the win32-specific event handler for OS-events.
//-------------------------------------------------------------------------------------------------
class Win32Eventhandler : public AbstractPlatformEventHandler {
public:
    Win32Eventhandler();
    virtual ~Win32Eventhandler();
    virtual bool onEvent( const Common::Event &ev, const Common::EventData *pEventData ) override;
    static LRESULT CALLBACK winproc( HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam );
    void setRootSurface( AbstractSurface *pWIndow );
    AbstractSurface *getRootSurface() const;
    void enablePolling( bool enabled ) override;;
    bool isPolling() const;
    void registerEventListener( const CPPCore::TArray<const Common::Event*> &rEvents, OSEventListener *pListener ) override;;
    void unregisterEventListener( const CPPCore::TArray<const Common::Event*> &rEvents, OSEventListener *pListener ) override;;
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
} // Namespace OSRE
