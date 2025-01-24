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

#include "Common/AbstractEventHandler.h"
#include "Common/AbstractService.h"
#include "Common/Event.h"
#include "Common/TResource.h"
#include "Common/TResourceCache.h"
#include "Platform/KeyTypes.h"
#include "Platform/PlatformCommon.h"
#include "Platform/PluginType.h"

#include <cppcore/Container/TList.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This class declares the abstract interface for all operation system events.
///	You can use it to get notified, when you want to react onto a mouse click for instance. Just
///	overload the method onOSEvent with your own code.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT OSEventListener : public Common::Object {
public:
    ///	@brief	The class destructor, virtual.
    virtual ~OSEventListener() = default;

    ///	@brief	The os-event callback, override this for your own implementation.
    ///	@param	osEvent			[in] The incoming event.
    ///	@param	pData			[in] A pointer showing to the event data.
    virtual void onOSEvent(const Common::Event &osEvent, const Common::EventData *pData) = 0;

protected:
    ///	@brief	The class constructor.
    ///	@param	OSListenerName	[in] The instance name.
    OSEventListener(const String &OSListenerName) : Object(OSListenerName) {}
};

DECL_EVENT(KeyboardButtonDownEvent);
DECL_EVENT(KeyboardButtonUpEvent);
DECL_EVENT(MouseButtonDownEvent);
DECL_EVENT(MouseButtonUpEvent);
DECL_EVENT(MouseMoveEvent);
DECL_EVENT(QuitEvent);
DECL_EVENT(AppFocusEvent);
DECL_EVENT(WindowsMoveEvent);
DECL_EVENT(WindowsResizeEvent);

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This class stores window-resizing events.
//-------------------------------------------------------------------------------------------------
class WindowsMoveEventData : public Common::EventData {
public:
    /// @brief The class constructor.
    /// @param c The event trigger.
    WindowsMoveEventData(Common::EventTriggerer *c) : Common::EventData(WindowsResizeEvent, c), m_x(0), m_y(0) {}

    ui32 m_x, m_y;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This class stores window-resizing events.
//-------------------------------------------------------------------------------------------------
class WindowsResizeEventData : public Common::EventData{
public:
    /// @brief The class constructor.
    /// @param c The event trigger.
    WindowsResizeEventData(Common::EventTriggerer *c) : Common::EventData(WindowsResizeEvent, c), m_x(0), m_y(0), m_w(0), m_h(0) {}

    ui32 m_x, m_y, m_w, m_h;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This class stored keyboard events.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT KeyboardButtonEventData : public Common::EventData {
public:
    ///	@brief	The class constructor.
    ///	@param	down	[in] true, if button is pressed, false if button is releases.
    ///	@param	c		[in] The event trigger sender.
    KeyboardButtonEventData(bool down, Common::EventTriggerer *c) :  Common::EventData(down ? KeyboardButtonDownEvent : KeyboardButtonUpEvent, c), m_key(KEY_UNKNOWN), m_unicode(0) {}

    Key m_key; ///< Key code for the pressed/released keyboard button
    ui16 m_unicode; ///< The Unicode character for the pressed/released key
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This class stores mouse button events.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT MouseButtonEventData : public Common::EventData {
public:
    /// @brief An enum to describe the button state.
    enum ButtonType {
        LeftButton = 0,
        MiddleButton,
        RightButton
    };

    ///	@brief	The class constructor.
    ///	@param	down	[in] true, if the mouse button was pressed, false if the mouse button was
    ///					released.
    ///	@param	c		[in] The event trigger sender.
    MouseButtonEventData(bool down, Common::EventTriggerer *c) :
            Common::EventData(down ? MouseButtonDownEvent : MouseButtonUpEvent, c),
            m_Button(0),
            m_AbsX(0),
            m_AbsY(0),
            m_Pressed(down) {
        // empty
    }

    ui32 m_Button; ///< pressed button (0=left, 1=middle, 2=right )
    i32 m_AbsX; ///< absolute X-position of the mouse cursor
    i32 m_AbsY; ///< absolute Y-position of the mouse cursor
    bool m_Pressed; ///< True, if the button was pressed
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This class stored mouse-move specific event data.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT MouseMoveEventData : public Common::EventData {
public:
    ///	@brief	The class constructor.
    ///	@param	c		[in] The event trigger sender.
    MouseMoveEventData(Common::EventTriggerer *c) :
            Common::EventData(MouseMoveEvent, c), m_absX(0), m_absY(0) {
        // empty
    }

    i32 m_absX; ///< The absolute X-position of the mouse cursor
    i32 m_absY; ///< The absolute Y-position of the mouse cursor
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
/// @brief This struct is a container for all application-specific data.
//-------------------------------------------------------------------------------------------------
struct ApplicationContext {
    const Properties::Settings *mSettings;          ///<
    PluginType m_type;
    AbstractWindow *m_rootSurface;
    AbstractPlatformEventQueue *m_oseventHandler;
    AbstractOGLRenderContext *m_renderContext;
    AbstractTimer *mTimer;
    AbstractDynamicLoader *m_dynLoader;
    AbstractSystemInfo *m_systemInfo;
    AbstractOSService *mAbstractOSService;

    ApplicationContext(const Properties::Settings *config);
    ~ApplicationContext() = default;
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class encapsulates platform-specific details.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT PlatformInterface : public Common::AbstractService {
public:
    /// @brief Will create the platform service provider.
    /// @param configuration   The configuration to use.
    /// @return The new created service provider.
    static PlatformInterface *create(const Properties::Settings *configuration);

    /// @brief Will destroy the platform interface.
    static void destroy();

    /// @brief Returns the only instance.
    /// @return The only instance-
    static PlatformInterface *getInstance();

    /// @brief Will return the type of plugin
    /// @return The type of plugin
    static PluginType getOSPluginType();

    /// @brief Will return the assigned name to a given plugin type.
    /// @param type The plugin type.
    /// @return The assigned name.
    static String getOSPluginName(PluginType type);

    /// @brief Will return the root window.
    /// @return The root window.
    AbstractWindow *getRootWindow() const;

    /// @brief Will return the platform event handler.
    /// @return The platform event handler.
    AbstractPlatformEventQueue *getPlatformEventHandler() const;
    
    /// @brief Will return the render context.
    /// @return The render context.
    AbstractOGLRenderContext *getRenderContext() const;
    
    /// @brief Will return the timer.
    /// @return The timer
    AbstractTimer *getTimer() const;
    
    /// @brief Will return the dynamic lib loader.
    /// @return The dynamic lib loader.
    AbstractDynamicLoader *getDynamicLoader() const;
    
    /// @brief Will return the system info.
    /// @return The system info.
    AbstractSystemInfo *getSystemInfo() const;
    
    /// @brief Will return special os-services.
    /// @return The os-services.
    AbstractOSService *getOSServices() const;
    
    /// @brief Will return the name of the default font.
    /// @return The name of the default font.
    const String &getDefaultFontName() const;

    // No copying.
    PlatformInterface(const PlatformInterface &) = delete;
    PlatformInterface &operator=(const PlatformInterface &) = delete;

protected:
    bool onOpen() override;
    bool onClose() override;
    bool onUpdate() override;
    virtual bool setupGfx(WindowsProperties *props, bool polls);

private:
    explicit PlatformInterface(const Properties::Settings *configuration);
    virtual ~PlatformInterface() override;

private:
    static PlatformInterface *sInstance;
    ApplicationContext *mContext;
};

} // Namespace Platform
} // Namespace OSRE
