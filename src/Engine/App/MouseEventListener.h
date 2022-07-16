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

#include <osre/App/AppBase.h>
#include <osre/Common/TObjPtr.h>
#include <osre/Common/osre_common.h>
#include <osre/Platform/PlatformInterface.h>

namespace OSRE {
namespace App {

//-------------------------------------------------------------------------------------------------
///	@ingroup    Editor
///
/// @brief
//-------------------------------------------------------------------------------------------------
class MouseEventListener : public Platform::OSEventListener {
public:
    static const ui32 LeftButton = 1;
    static const ui32 MiddleButton = 2;
    static const ui32 RightButton = 3;

    ///	@brief  The default class constructor.
    MouseEventListener();

    ///	@brief  The class destructor.
    ~MouseEventListener() override;

    /// @brief  The event-callback.
    /// @param osEvent  The event id.
    /// @param data     The event data.
    void onOSEvent(const Common::Event &osEvent, const Common::EventData *data) override;

    ///	@brief  Will return true, if the left mouse-button was pressed,
    /// @return true if the left mouse button was pressed.
    bool leftButttonPressed() const;

    ///	@brief  Will return true, if the middle mouse-button was pressed,
    /// @return true if the middle mouse button was pressed.
    bool middleButttonPressed() const;
    
    ///	@brief  Will return true, if the right mouse-button was pressed,
    /// @return true if the right mouse button was pressed.
    bool rightButttonPressed() const;

    /// @brief Will return the relative X-movement.
    /// @return The relative X-movement.
    i32 getRelativeX() const;

    /// @brief Will return the relative Y-movement.
    /// @return The relative Y-movement.
    i32 getRelativeY() const;

    /// @brief Will return the absolute X-movement.
    /// @return The absolute X-movement.
    i32 getAbsoluteX() const;

    /// @brief  Will return the absolute Y-movement.
    /// @return The absolute Y-movement.
    i32 getAbsoluteY() const;

    /// @brief Will return the mouse input state as a const reference.
    /// @return  The mouse input state.
    const MouseInputState &getMouseInputState() const;

private:
    MouseInputState mMouseInputState;
};

inline bool MouseEventListener::leftButttonPressed() const {
    return mMouseInputState.mMouseButtonState.getBit(LeftButton);
}

inline bool MouseEventListener::middleButttonPressed() const {
    return mMouseInputState.mMouseButtonState.getBit(MiddleButton);
}

inline bool MouseEventListener::rightButttonPressed() const {
    return mMouseInputState.mMouseButtonState.getBit(RightButton);
}

inline i32 MouseEventListener::getRelativeX() const {
    return mMouseInputState.mRelX;
}

inline i32 MouseEventListener::getRelativeY() const {
    return mMouseInputState.mRelY;
}

inline i32 MouseEventListener::getAbsoluteX() const {
    return mMouseInputState.mAbsX;
}

inline i32 MouseEventListener::getAbsoluteY() const {
    return mMouseInputState.mAbsY;
}

inline const MouseInputState &MouseEventListener::getMouseInputState() const {
    return mMouseInputState;
}
} // Namespace App
} // Namespace OSRE
