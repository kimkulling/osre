/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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

static const String Tag = "AbstractSurface";

Resolution::Resolution( ResRequest req )
: m_width( 0 )
, m_height( 0 ) {
    if ( Res640x480 == req ) {
        m_width = 640;
        m_height = 480;
    } else if ( Res800x600 == req ) {
        m_width = 800;
        m_height = 600;
    } else if ( Res1024x768 == req ) {
        m_width = 1024;
        m_height = 768;
    } else if ( Res1176x664 == req ) {
        m_width = 1176;
        m_height = 664;

    } else if ( Res1768x992 == req ) {
        m_width = 1768;
        m_height = 992;
    } else if ( Res1920x1080 == req ) {
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

AbstractWindow::AbstractWindow( WindowsProperties *properties )
: m_flags( SF_PropertiesClean )
, m_properties( properties )
, m_isCreated( false ) {
    // empty    
}

AbstractWindow::~AbstractWindow( ) {
    delete m_properties;
    m_properties = nullptr;
}

bool AbstractWindow::create( ) {
    if ( m_isCreated ) {
        osre_warn( Tag, "Surface already created." );
        return true;
    }

    m_isCreated = onCreate();

    return m_isCreated;
}

bool AbstractWindow::destroy( ) {
    if ( !m_isCreated ) {
        osre_warn( Tag, "Surface not valid, cannot be destoyed." );
        return false;
    }
    
    if ( onDestroy() ) {
        m_isCreated = false;
    }

    return ( false == m_isCreated );
}

void AbstractWindow::resize( ui32 x, ui32 y, ui32 w, ui32 h ) {
    if ( !m_isCreated ) {
        osre_warn( Tag, "Surface not valid, cannot be resized." );
        return;
    }

    onResize( x, y, w, h );
}

bool AbstractWindow::updateProperties() {
    return onUpdateProperies();
}

void AbstractWindow::setFlags( SurfaceFlagType flags ) {
    if ( m_flags == static_cast<ui32>( flags ) ) {
        return;
    }
    m_flags = flags;
}

ui32 AbstractWindow::getFlags() const {
    return m_flags;
}

void AbstractWindow::setProperties( WindowsProperties *pProperties ) {
    m_properties = pProperties;
}

WindowsProperties *AbstractWindow::getProperties( ) {
    return m_properties;
}

Rect2ui AbstractWindow::getWindowsRect() const {
    Rect2ui r;
    if (nullptr == m_properties) {
        return r;
    }

    r.set(m_properties->m_x, m_properties->m_y, m_properties->m_width, m_properties->m_height);

    return r;
}

} // Namespace Platform
} // Namespace OSRE
