/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Platform/PlatformCommon.h>

#include <osre/Common/AbstractEventHandler.h>
#include <osre/Common/Event.h>
#include <osre/Common/EventTriggerer.h>
#include <osre/Common/Object.h>
#include <osre/Debugging/osre_debugging.h>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace Common {
    class EventBus;
}

namespace RenderBackend {
    class RenderBackendService;
}

namespace Platform {

/// @brief
using MenuFunctor = Common::Functor<void, ui32, void *>;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This abstract class defines the event handler for a platform event loop handler.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractPlatformEventQueue : public Common::Object {
public:
    using MenuFuncMap = std::map<ui32, MenuFunctor>;

    ///	@brief  The class destructor, virtual.
    virtual ~AbstractPlatformEventQueue();

    ///	@brief  Used to register a event listener.
    ///	@param  events      [in] List of events.
    /// @param  listener    [in] The listener instance
    virtual void registerEventListener(const Common::EventPtrArray &events, OSEventListener *listener) = 0;

    ///	@brief  Used to unregister a event listener.
    ///	@param  events      [in] List of events.
    /// @param  listener    [in] The listener instance
    virtual void unregisterEventListener(const Common::EventPtrArray &events, OSEventListener *listener) = 0;

    /// @brief  Will unregister all registered event handler.
    virtual void unregisterAllEventHandler(const Common::EventPtrArray &events) = 0;

    /// @brief  Will register a new menu command.
    virtual void registerMenuCommand(ui32 id, MenuFunctor func) = 0;

    /// @brief  Will unregister all registered menu commands.
    virtual void unregisterAllMenuCommands() = 0;

    ///	@brief  Set the polling state.
    ///	@param  enabled     true for enabling polling.
    virtual void enablePolling(bool enabled) = 0;

    /// @brief  Returns the currently active polling state.
    /// @return The active polling state.
    virtual bool isPolling() const = 0;

    /// @brief  Will perform an update.
    /// @return Returns false in case of an error.
    virtual bool update() = 0;

    /// @brief  Will set the render back-end service.
    /// @param  rbSrv   [in] The pointer showing to the render back-end.
    virtual void setRenderBackendService(RenderBackend::RenderBackendService *rbSrv);

    /// @brief  Will return the render back-end.
    /// @return The render back-end service, pointer is nullptr if the service was not set.
    virtual RenderBackend::RenderBackendService *getRenderBackendService() const;

    /// @brief  Enqueues a new event into the active event queue.
    /// @param  ev      [in] The event to enqueue.
    virtual void enqueueEvent(const Common::Event &ev, Common::EventData *data);

protected:
    /// @brief  The class constructor.
    AbstractPlatformEventQueue();

    /// @brief  The quit handler.
    virtual void onQuit() = 0;

    ///	@brief  Will be called to process events.
    /// @param  pTriggerer  The event trigger.
    void processEvents(Common::EventTriggerer *pTriggerer);

    ///	@brief  Returns the active event data list.
    /// @return The active event data list.
    Common::EventDataList *getActiveEventDataList();

    ///	@brief  Returns the active event data list.
    /// @return The pending event data list.
    Common::EventDataList *getPendingEventDataList();

    ///	@brief  Toggles between the active and pending list.
    void switchEventDataList();

private:
    static const size_t numEventQueues = 2;
    Common::EventDataList m_eventQueues[numEventQueues];
    ui32 mActiveList;
    RenderBackend::RenderBackendService *mRenderBackendSrv;
    Common::EventBus *mEventBus;
};

} // Namespace Platform
} // Namespace OSRE
