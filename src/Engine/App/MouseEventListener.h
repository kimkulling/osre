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

#include <cppcore/Common/TBitField.h>
#include <osre/App/AppBase.h>
#include <osre/Common/TObjPtr.h>
#include <osre/Common/osre_common.h>
#include <osre/Platform/PlatformInterface.h>

namespace OSRE {
namespace App {

class MouseEventListener : public Platform::OSEventListener {
public:
    MouseEventListener();
    ~MouseEventListener() override;
    void onOSEvent(const Common::Event &osEvent, const Common::EventData *data) override;
    bool leftButttonPressed() const;
    bool middleButttonPressed() const;
    bool rightButttonPressed() const;
    i32 getRelativeX() const;
    i32 getRelativeY() const;
    i32 getAbsoluteX() const;
    i32 getAbsoluteY() const;

private:
    const ui32 LeftButton = 1;
    const ui32 MiddleButton = 2;
    const ui32 RightButton = 3;
    i32 mRelX, mRelY, mAbsX, mAbsY;
    ui32 mLastX, mLastY;
    CPPCore::TBitField<ui32> mMouseButtonState;
};

inline bool MouseEventListener::leftButttonPressed() const {
    return mMouseButtonState.getBit(LeftButton);
}

inline bool MouseEventListener::middleButttonPressed() const {
    return mMouseButtonState.getBit(MiddleButton);
}

inline bool MouseEventListener::rightButttonPressed() const {
    return mMouseButtonState.getBit(RightButton);
}

inline i32 MouseEventListener::getRelativeX() const {
    return mRelX;
}

inline i32 MouseEventListener::getRelativeY() const {
    return mRelY;
}

inline i32 MouseEventListener::getAbsoluteX() const {
    return mAbsX;
}

inline i32 MouseEventListener::getAbsoluteY() const {
    return mAbsY;
}

} // Namespace App
} // Namespace OSRE
