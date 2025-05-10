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
#include "Common/EventTriggerer.h"
#include "Common/Logger.h"
#include "Platform/PlatformInterface.h"
#include "RenderBackend/RenderBackendService.h"
#include "Platform/sdl2/SDL2EventQueue.h"

#include "SDL2Initializer.h"
#include "SDL2Window.h"

#include <SDL.h>

namespace OSRE {
namespace Platform {

using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

using namespace ::cppcore;

static constexpr c8 Tag[] = "SDL2EventHandler";

//-------------------------------------------------------------------------------------------------
/// @brief  The abstract interface for sdl2-based event updates.
//-------------------------------------------------------------------------------------------------
struct AbstractSDL2InputUpdate {
    /// @brief  The virtual destructor.
    virtual ~AbstractSDL2InputUpdate() = default;

    /// @brief Will perform the update.
    /// @param[out] ev   The next event or a nullptr, if none.
    virtual bool update(SDL_Event *ev) = 0;
};

//-------------------------------------------------------------------------------------------------
//  Implements a get update, waits for the next event.
//-------------------------------------------------------------------------------------------------
struct SDL2GetInputUpdate final : public AbstractSDL2InputUpdate {
    //  The default constructor.
    SDL2GetInputUpdate() = default;

    //  The destructor.
    ~SDL2GetInputUpdate() override = default;

    //  Update implemented as a wait operation, will get the next upcoming event.
    bool update(SDL_Event *ev) override {
        const i32 ret = SDL_WaitEvent(ev);
        if (0 == ret) {
            return false;
        }

        return false;
    }
};

//-------------------------------------------------------------------------------------------------
//  Implements a peek update, polls for the next event.
//-------------------------------------------------------------------------------------------------
struct SDL2PeekInputUpdate final : public AbstractSDL2InputUpdate {
    //  The default constructor.
    SDL2PeekInputUpdate() = default;

    //  The destructor.
    ~SDL2PeekInputUpdate() override = default;

    //  Update implemented as a poll operation, will check for a new event.
    bool update(SDL_Event *ev) override {
        const i32 ret = ::SDL_PollEvent(ev);
        if (ret == 0) {
            return false;
        }

        return true;
    }
};

std::map<SDL_Window *, SDL2EventHandler *> SDL2EventHandler::s_windowsServerMap;

SDL2EventHandler::SDL2EventHandler(AbstractWindow *window) :
        AbstractPlatformEventQueue(),
        m_isPolling(false),
        mShutdownRequested(false),
        m_inputUpdate(nullptr),
        mWindow(nullptr),
        m_eventTriggerer(nullptr) {
    osre_assert(nullptr != window);
    mWindow = (SDL2Surface *)window;
    osre_assert(nullptr != mWindow);

    m_inputUpdate = new SDL2GetInputUpdate;
    m_eventTriggerer = new EventTriggerer;
    m_eventTriggerer->addTriggerableEvent(KeyboardButtonDownEvent);
    m_eventTriggerer->addTriggerableEvent(KeyboardButtonUpEvent);
    m_eventTriggerer->addTriggerableEvent(MouseButtonDownEvent);
    m_eventTriggerer->addTriggerableEvent(MouseButtonUpEvent);
    m_eventTriggerer->addTriggerableEvent(WindowsResizeEvent);
    m_eventTriggerer->addTriggerableEvent(MouseMoveEvent);
    m_eventTriggerer->addTriggerableEvent(QuitEvent);
    m_eventTriggerer->addTriggerableEvent(AppFocusEvent);

    SDL2Initializer::init();
}

SDL2EventHandler::~SDL2EventHandler() {
    SDL2Initializer::release();

    unregisterAllMenuCommands();
    
    delete m_eventTriggerer;
    delete m_inputUpdate;
}

static inline bool isLowerCaseKey(Uint16 mod) {
    if (mod & KMOD_RSHIFT || mod &KMOD_LSHIFT) {
        return true;
    }

    return false;
}

bool SDL2EventHandler::update() {
    EventDataList *activeEventQueue(getActiveEventDataList());
    if (nullptr == activeEventQueue) {
        OSRE_CHECK_NOENTRY2("Active event queue is nullptr.");
        return false;
    }

    SDL_Event ev;
    if (!mShutdownRequested && m_inputUpdate->update(&ev)) {
        const Uint32 windowID = SDL_GetWindowID(mWindow->getSDLSurface());

        switch (ev.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                KeyboardButtonEventData *data = new KeyboardButtonEventData(SDL_KEYDOWN == ev.type, m_eventTriggerer);
                const char *c = SDL_GetKeyName(ev.key.keysym.sym);
                if (!isLowerCaseKey(ev.key.keysym.mod)) {
                    const char l = tolower(*c);
                    data->m_key = (Key) l;
                } else {
                    data->m_key = (Key) *c;
                }
                activeEventQueue->addBack(data);
            } break;

            case SDL_MOUSEMOTION: {
                MouseMoveEventData *data = new MouseMoveEventData(m_eventTriggerer);
                int x, y;
                SDL_GetMouseState(&x, &y);
                data->m_absX = x;
                data->m_absY = y;
                activeEventQueue->addBack(data);
            } break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                MouseButtonEventData *data = new MouseButtonEventData(ev.type == SDL_MOUSEBUTTONDOWN, m_eventTriggerer);
                activeEventQueue->addBack(data);
            } break;

            case SDL_QUIT: {
                mShutdownRequested = true;
            } break;

            case SDL_WINDOWEVENT: {
                if (ev.window.windowID == windowID) {
                    switch (ev.window.event) {
                        case SDL_WINDOWEVENT_EXPOSED: {
                            const auto &rect = mWindow->getProperties()->mRect;
                            getRenderBackendService()->resize(mWindow->getId(), rect.getX1(), rect.getY1(), rect.width, rect.height);
                        } break;
                        
                        case SDL_WINDOWEVENT_SHOWN:
                        case SDL_WINDOWEVENT_SIZE_CHANGED: {
                            const ui32 w = static_cast<ui32>(ev.window.data1);
                            const ui32 h = static_cast<ui32>(ev.window.data2);
                            getRenderBackendService()->resize(mWindow->getId(), 0, 0, w, h);
                        } break;
                    }
                }
            } break;

            default:
                break;
        }

        processEvents(m_eventTriggerer);
    }

    return !mShutdownRequested;
}

void SDL2EventHandler::registerEventListener(const EventArray &events, OSEventListener *listener) {
    if (nullptr == m_eventTriggerer) {
        osre_error(Tag, "Pointer to event-triggerer is nullptr.");
        return;
    }

    if (nullptr == listener) {
        osre_error(Tag, "Pointer to listener is nullptr.");
        return;
    }

    m_eventTriggerer->addEventListener(events, EventFunctor::Make(listener, &OSEventListener::onOSEvent));
}

void SDL2EventHandler::unregisterEventListener(const EventArray &events, OSEventListener *listener) {
    if (nullptr == m_eventTriggerer) {
        osre_error(Tag, "Pointer to event-triggerer is nullptr.");
        return;
    }

    if (nullptr == listener) {
        osre_error(Tag, "Pointer to listener is nullptr.");
        return;
    }

    m_eventTriggerer->removeEventListener(events, EventFunctor::Make(listener, &OSEventListener::onOSEvent));
}

void SDL2EventHandler::unregisterAllEventHandler(const EventPtrArray &events) {
    if (nullptr == m_eventTriggerer) {
        osre_error(Tag, "Pointer to event-triggerer is nullptr.");
        return;
    }

    m_eventTriggerer->removeAllEventListeners(events);
}

void SDL2EventHandler::registerMenuCommand(ui32 id, MenuFunctor func) {
}

void SDL2EventHandler::unregisterAllMenuCommands() {
}

void SDL2EventHandler::enablePolling(bool enabled) {
    if (enabled == m_isPolling) {
        return;
    }

    delete m_inputUpdate;
    if (enabled) {
        m_inputUpdate = new SDL2PeekInputUpdate;
    } else {
        m_inputUpdate = new SDL2GetInputUpdate;
    }
    m_isPolling = enabled;
}

bool SDL2EventHandler::isPolling() const {
    return m_isPolling;
}

} // Namespace Platform
} // Namespace OSRE
