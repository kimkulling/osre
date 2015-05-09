#pragma once

#include <osre/Common/AbstractService.h>
#include <osre/Common/AbstractEventHandler.h>
#include <osre/Common/Logger.h>
#include <osre/Common/Event.h>
#include <osre/Platform/KeyTypes.h>
#include <osre/Platform/PluginType.h>

#include <cppcore/Container/TArray.h>
#include <cppcore/Container/TList.h>

#include <map>

namespace OSRE {

namespace Common {
    class EventTriggerer;
}

namespace Properties {
    class ConfigurationMap;
}

namespace Platform {

class AbstractSurface;
class AbstractTimer;
class AbstractPlatformEventHandler;
class AbstractRenderContext;

struct IInputUpdate;
struct SurfaceProperties;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Interface::OSEventListener
///	@ingroup	RenderSystem
///
///	@brief	This class declares the abstract interface for all operation system events.
///	You can use it to get notified, when you want to react onto a mouse click for instance. Just
///	overload the method onOSEvent with your own code.
//-------------------------------------------------------------------------------------------------
class OSEventListener : public Common::Object {
public:
    ///	@brief	The class destructor, virtual.
    virtual ~OSEventListener() {
        // empty
    }

    ///	@brief	The os-event callback, override this for your own implementation.
    ///	@param	osEvent			[in] The incoming event.
    ///	@param	pData			[in] A pointer showing to the event data.
    virtual void onOSEvent( const Common::Event &osEvent, const Common::EventData *pData ) = 0;

protected:
    ///	@brief	The class constructor.
    ///	@param	OSListenerName	[in] The instance name.
    OSEventListener( const String &OSListenerName ) 
    : Object( OSListenerName ) {
        // empty
    }
};

DECL_EVENT( KeyboardButtonDownEvent );
DECL_EVENT( KeyboardButtonUpEvent );
DECL_EVENT( MouseButtonDownEvent );
DECL_EVENT( MouseButtonUpEvent );
DECL_EVENT( MouseMoveEvent );
DECL_EVENT( QuitEvent );
DECL_EVENT( AppFocusEvent );

//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Interface::KeyboardButtonEventData
///	@ingroup	RenderSystem
///
///	@brief	This class stored keyboard events.
//-------------------------------------------------------------------------------------------------
class KeyboardButtonEventData : public Common::EventData {
public:
    ///	@brief	The class constructor.
    ///	@param	down	[in] true, if button is pressed, false if button is releases.
    ///	@param	c		[in] The event trigger sender.
    KeyboardButtonEventData( bool down, Common::EventTriggerer* c ) :
        Common::EventData( down ? KeyboardButtonDownEvent : KeyboardButtonUpEvent, c ),
        m_Key( KEY_UNKNOWN ),
        m_Unicode( 0 )
    {
            // empty
        }

    Key m_Key;		///< Key code for the pressed/released keyboard button
    ui16 m_Unicode;	///< The Unicode character for the pressed/released key
};


//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Interface::MouseButtonEventData
///	@ingroup	RenderSystem
///
///	@brief	This class stores mouse button events.
//-------------------------------------------------------------------------------------------------
class MouseButtonEventData : public Common::EventData {
public:
    enum ButtonType {
        LeftButton = 0,
        MiddleButton,
        RightButton
    };

public:
    ///	@brief	The class constructor.
    ///	@param	down	[in] true, if the mouse button was pressed, false if the mouse button was
    ///					released.
    ///	@param	c		[in] The event trigger sender.
    MouseButtonEventData( bool down, Common::EventTriggerer *c ) :
        Common::EventData( down ? MouseButtonDownEvent : MouseButtonUpEvent, c ),
        m_Button( 0 ),
        m_AbsX( 0 ),
        m_AbsY( 0 ),
        m_Pressed( false )
    {
            // empty
        }

    ui32 m_Button;	///< pressed button (0=left, 1=middle, 2=right )
    i32 m_AbsX;		///< absolute X-position of the mouse cursor
    i32 m_AbsY;		///< absolute Y-position of the mouse cursor
    bool m_Pressed;	///< True, if the button was pressed
};

//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Interface::MouseMoveEventData
///	@ingroup	RenderSystem
///
///	@brief	This class stored mouse-move specific event data.
//-------------------------------------------------------------------------------------------------
class MouseMoveEventData : public Common::EventData {
public:
    ///	@brief	The class constructor.
    ///	@param	c		[in] The event trigger sender.
    MouseMoveEventData( Common::EventTriggerer *c ) :
        Common::EventData( MouseMoveEvent, c ),
        m_AbsX( 0 ),
        m_AbsY( 0 ) {
            // empty
        }

    i32 m_AbsX;		///< The absolute X-position of the mouse cursor
    i32 m_AbsY;		///< The absolute Y-position of the mouse cursor
};


//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::PlatformInterface
///	@ingroup	Infrastructure
///
///	@brief  This class encapsulates platform-specific details.
//-------------------------------------------------------------------------------------------------
class PlatformInterface : public Common::AbstractService {
public:
    AbstractSurface *getRootSurface() const;
    static PlatformInterface *create( const Properties::ConfigurationMap *pConfiguration );
    static void destroy();
    static PlatformInterface *getInstance();
    AbstractPlatformEventHandler *getPlatformEventHandler() const;
    AbstractRenderContext *getRenderContext() const;
    AbstractTimer *getTimer() const;
    static PluginType getOSPluginType();
    static String getOSPluginName( PluginType type );

protected:
    virtual bool onOpen();
    virtual bool onClose();
    virtual bool onUpdate( d32 timediff );
    virtual bool setupGfx( SurfaceProperties *props, bool polls );

private:
    PlatformInterface( const Properties::ConfigurationMap *pConfiguration );
    virtual ~PlatformInterface();

private:
    static PlatformInterface *s_instance;
    const Properties::ConfigurationMap *m_pConfiguration;
    PluginType m_type;
    AbstractSurface *m_pRootSurface;
    AbstractPlatformEventHandler *m_pOSEventHandler;
    AbstractRenderContext *m_pRenderContext;
    AbstractTimer *m_pTimer;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
