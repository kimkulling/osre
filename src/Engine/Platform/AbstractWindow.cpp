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
#include <osre/Platform/AbstractWindow.h>
#include <osre/Common/Logger.h>

namespace OSRE {
namespace Platform {

static const c8 *Tag = "AbstractSurface";

Resolution::Resolution( ResRequest req )
: m_width( 0 )
, m_height( 0 ) {
    if (ResRequest::Res640x480 == req) {
        m_width = 640;
        m_height = 480;
    } else if (ResRequest::Res800x600 == req) {
        m_width = 800;
        m_height = 600;
    } else if (ResRequest::Res1024x768 == req) {
        m_width = 1024;
        m_height = 768;
    } else if (ResRequest::Res1176x664 == req) {
        m_width = 1176;
        m_height = 664;
    } else if (ResRequest::Res1768x992 == req) {
        m_width = 1768;
        m_height = 992;
    } else if (ResRequest::Res1920x1080 == req) {
        m_width = 1920;
        m_height = 1080;
    }
}

Resolution::Resolution()
: m_width( 0 )
, m_height( 0 ) {
    // empty
}

Resolution::~Resolution() {
    // empty
}

AbstractWindow::AbstractWindow( WindowsProperties *properties ) :
        mFlags((ui32)SurfaceFlagType::SF_PropertiesClean), mProperties( properties )
, mIsCreated( false ) {
    // empty    
}

AbstractWindow::~AbstractWindow( ) {
    delete mProperties;
    mProperties = nullptr;
}

bool AbstractWindow::create( ) {
    if ( mIsCreated ) {
        osre_warn( Tag, "Surface already created." );
        return true;
    }

    mIsCreated = onCreate();

    return mIsCreated;
}

bool AbstractWindow::destroy( ) {
    if ( !mIsCreated ) {
        osre_warn( Tag, "Surface not valid, cannot be destoyed." );
        return false;
    }
    
    if ( onDestroy() ) {
        mIsCreated = false;
    }

    return ( false == mIsCreated );
}

bool AbstractWindow::isCeated() const {
    return mIsCreated;
}

void AbstractWindow::resize( ui32 x, ui32 y, ui32 w, ui32 h ) {
    if ( !mIsCreated ) {
        osre_warn( Tag, "Surface not valid, cannot be resized." );
        return;
    }

    onResize( x, y, w, h );
}

bool AbstractWindow::updateProperties() {
    return onUpdateProperies();
}

void AbstractWindow::setFlags( SurfaceFlagType flags ) {
    if ( mFlags == static_cast<ui32>( flags ) ) {
        return;
    }
    mFlags = (ui32)flags;
}

ui32 AbstractWindow::getFlags() const {
    return mFlags;
}

void AbstractWindow::setProperties( WindowsProperties *pProperties ) {
    mProperties = pProperties;
}

WindowsProperties *AbstractWindow::getProperties( ) {
    return mProperties;
}

void AbstractWindow::getWindowsRect(Rect2ui &rect) const {
    if (nullptr == mProperties) {
        return;
    }

    rect.set(mProperties->m_x, mProperties->m_y, mProperties->m_width, mProperties->m_height);
}

} // Namespace Platform
} // Namespace OSRE
