/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Platform/AbstractSurface.h>
#include <osre/Common/Logger.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
AbstractSurface::AbstractSurface( SurfaceProperties *properties )
: m_flags( SF_PropertiesClean )
, m_pProperties( properties )
, m_isCreated( false ) {
    // empty    
}

//-------------------------------------------------------------------------------------------------
AbstractSurface::~AbstractSurface( ) {
    delete m_pProperties;
    m_pProperties = nullptr;
}

//-------------------------------------------------------------------------------------------------
bool AbstractSurface::create( ) {
    if ( m_isCreated ) {
        osre_warn( "Surfac already created." );
        return true;
    }

    m_isCreated = onCreate();

    return m_isCreated;
}

//-------------------------------------------------------------------------------------------------
bool AbstractSurface::destroy( ) {
    if ( !m_isCreated ) {
        osre_warn( "Surface not valid, cannot be destoyed." );
        return false;
    }
    if ( onDestroy() ) {
        m_isCreated = false;
    }

    return ( false == m_isCreated );
}

//-------------------------------------------------------------------------------------------------
bool AbstractSurface::updateProperties() {
    return onUpdateProperies();
}

//-------------------------------------------------------------------------------------------------
void AbstractSurface::setFlags( SurfaceFlagType flags ) {
    m_flags = flags;
}

//-------------------------------------------------------------------------------------------------
ui32 AbstractSurface::getFlags() const {
    return m_flags;
}

//-------------------------------------------------------------------------------------------------
void AbstractSurface::setProperties( SurfaceProperties *pProperties ) {
    m_pProperties = pProperties;
}

//-------------------------------------------------------------------------------------------------
SurfaceProperties *AbstractSurface::getProperties( ) {
    return m_pProperties;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace OSRE
