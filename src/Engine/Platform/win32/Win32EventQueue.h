/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "Platform/AbstractPlatformEventQueue.h"
#include "Platform/Windows/MinWindows.h"

#include <cppcore/Container/TArray.h>
#include <map>

namespace OSRE {

// Forward declarations
namespace Common {
    struct Event;
    struct EventData;
    class EventTriggerer;
} // namespace Common

namespace Platform {

class AbstractWindow;
class OSEventListener;

struct AbstractInputUpdate;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements the win32-specific event handler for OS-events.
//-------------------------------------------------------------------------------------------------
class Win32EventQueue : public AbstractPlatformEventQueue {
public:
    /// The class constructor with the win32-based root-window.
    Win32EventQueue(AbstractWindow *rootWindow);
    /// The class destructor.
    ~Win32EventQueue() override;
    /// The update method.
    bool update() override;
    /// The root surface setter.
    void setRootSurface(AbstractWindow *window);
    /// The root surface getter.
    AbstractWindow *getRootSurface() const;
    /// Will enable the event polling mode.
    void enablePolling(bool enabled) override;
    /// Returns the event polling mode enabled.
    bool isPolling() const override;
    /// Registers an event listener.
    void registerEventListener(const Common::EventPtrArray &events, OSEventListener *listener) override;
    /// Unregisters an event listener.
    void unregisterEventListener(const Common::EventPtrArray &events, OSEventListener *listener) override;
    /// Will unregister all registered handler at once.
    void unregisterAllEventHandler(const Common::EventPtrArray &events) override;
    ///
    void registerMenuCommand(ui32 id, MenuFunctor func) override;
    ///
    void unregisterAllMenuCommands() override;
    ///
    static void registerEventQueue(Win32EventQueue *server, HWND hWnd);
    ///
    static void unregisterEventQueue(Win32EventQueue *server, HWND hWnd);
    ///
    static Win32EventQueue *getQueueByWndInstance(HWND hWnd);
    ///
    static LRESULT CALLBACK winproc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);

    // No copying
    Win32EventQueue() = delete;
    Win32EventQueue(const Win32EventQueue &) = delete;
    Win32EventQueue &operator=(const Win32EventQueue &) = delete;

protected:
    ///
    void onQuit() override;

private:
    using WindowServerMap = std::map<HWND, Win32EventQueue *>;

    static WindowServerMap s_WindowsServerMap;
    static MenuFuncMap s_MenuFunctorMap;
    AbstractInputUpdate *m_updateInstance;
    Common::EventTriggerer *m_eventTriggerer;
    AbstractWindow *m_rootWindow;
    bool mShutdownRequested;
    bool m_isPolling;
};

} // Namespace Platform
} // Namespace OSRE
