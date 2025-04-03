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

#include "App/AppCommon.h"
#include "Platform/PlatformInterface.h"
#include "Platform/KeyTypes.h"

namespace OSRE::App {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
/// @brief This class implements the keyboard event listener.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT KeyboardEventListener : public Platform::OSEventListener {
public:
    /// @brief The class constructor.
    KeyboardEventListener();

    /// @brief The class destructor.
    ~KeyboardEventListener() override = default;

    /// @brief The event handler.
    /// @param osEvent  The os-specific event.
    /// @param data     The event-related data.
    void onOSEvent(const Common::Event &osEvent, const Common::EventData *data) override;

    /// @brief Returns true, when the key is pressed
    /// @param key      The key to look for
    /// @return true for is pressed.
    bool isKeyPressed(Platform::Key key) const;

    /// @brief Will return the latest pressed key.
    /// @return The latest pressed key.
    Platform::Key getLastKey() const;

    /// @brief Clearn the map.
    void clearKeyMap();

private:
    KeyboardInputState mKeyboardInputState;
}; 

} // namespace OSRE::App
