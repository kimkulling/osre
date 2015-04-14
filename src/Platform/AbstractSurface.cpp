/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-214, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions
and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
and the following disclaimer in the documentation and/or other materials provided with the
distribution.

* Neither the name of the ZFXCE2 team, nor the names of its contributors may be used to endorse or
promote products derived from this software without specific prior written permission of the
ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
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
} // Namespace ZFXCE2
