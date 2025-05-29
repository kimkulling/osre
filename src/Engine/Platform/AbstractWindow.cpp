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
#include "Common/Logger.h"
#include "Platform/AbstractWindow.h"

namespace OSRE {
namespace Platform {

static constexpr c8 Tag[] = "AbstractSurface";

Resolution::Resolution(ResRequest req) :
        width(0), height(0) {
    if (ResRequest::Res640x480 == req) {
        width = 640;
        height = 480;
    } else if (ResRequest::Res800x600 == req) {
        width = 800;
        height = 600;
    } else if (ResRequest::Res1024x768 == req) {
        width = 1024;
        height = 768;
    } else if (ResRequest::Res1176x664 == req) {
        width = 1176;
        height = 664;
    } else if (ResRequest::Res1768x992 == req) {
        width = 1768;
        height = 992;
    } else if (ResRequest::Res1920x1080 == req) {
        width = 1920;
        height = 1080;
    }
}

Resolution::Resolution() :
        width(0), height(0) {
    // empty
}

AbstractWindow::AbstractWindow(guid id, WindowsProperties *properties, AbstractWindow *parent) :
        mId(id),
        mParent(parent),
        mFlags((ui32)SurfaceFlagType::SF_PropertiesClean),
        mShowState(ShowState::Hidden),
        mProperties(properties),
        mIsCreated(false) {
    // empty
}

AbstractWindow::~AbstractWindow() {
    delete mProperties;
    mProperties = nullptr;
}

bool AbstractWindow::create() {
    if (mIsCreated) {
        osre_warn(Tag, "Surface already created.");
        return true;
    }

    mIsCreated = onCreate();

    return mIsCreated;
}

bool AbstractWindow::destroy() {
    if (!mIsCreated) {
        osre_warn(Tag, "Surface not valid, cannot be destoyed.");
        return false;
    }

    if (onDestroy()) {
        mIsCreated = false;
    }

    return (false == mIsCreated);
}

bool AbstractWindow::isCreated() const {
    return mIsCreated;
}

void AbstractWindow::resize(ui32 x, ui32 y, ui32 w, ui32 h) {
    if (!mIsCreated) {
        osre_warn(Tag, "Surface not valid, cannot be resized.");
        return;
    }

    onResize(x, y, w, h);
}

bool AbstractWindow::updateProperties() {
    return onUpdateProperies();
}

void AbstractWindow::setFlags(SurfaceFlagType flags) {
    if (mFlags == static_cast<ui32>(flags)) {
        return;
    }
    mFlags = static_cast<ui32>(flags);
}

ui32 AbstractWindow::getFlags() const {
    return mFlags;
}

void AbstractWindow::setProperties(WindowsProperties *properties) {
    mProperties = properties;
}

WindowsProperties *AbstractWindow::getProperties() {
    return mProperties;
}

void AbstractWindow::getWindowsRect(Rect2i &rect) const {
    if (nullptr == mProperties) {
        return;
    }

    rect = mProperties->mRect;
}

} // Namespace Platform
} // Namespace OSRE
