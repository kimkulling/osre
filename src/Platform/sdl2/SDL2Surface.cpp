/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-2015, ZFXCE2 Development Team
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
#include <Code/Infrastructure/Platform/sdl2/SDL2Surface.h>
#include <osre2/Infrastructure/Common/Logger.h>

#include <SDL.h>

namespace ZFXCE2 {
namespace Platform {

//-------------------------------------------------------------------------------------------------
SDL2Surface::SDL2Surface( SurfaceProperties *props )
: AbstractSurface( props )
, m_surface( nullptr ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
SDL2Surface::~SDL2Surface( ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
bool SDL2Surface::onCreate() {
    // Turn on double buffering with a 24bit Z buffer
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );

    // Create our window centered at 512x512 resolution
    SurfaceProperties *prop = getProperties();
    if( !prop ) {
        return false;
    }
    
    ui32 w = prop->m_width;
    ui32 h = prop->m_height;
    m_surface = SDL_CreateWindow( "zfxce2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h,
                                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    if( !m_surface ) {
        ce_error( "Error while creating window." );
        return false;
    }
    SDL_ShowWindow( m_surface );

    return true;
}

//-------------------------------------------------------------------------------------------------
bool SDL2Surface::onDestroy( ) {
    if( !m_surface ) {
        return false;
    }

    SDL_DestroyWindow( m_surface );
    m_surface = nullptr;

    return true;
}
//-------------------------------------------------------------------------------------------------
bool SDL2Surface::onUpdateProperies() {
    return true;
}

//-------------------------------------------------------------------------------------------------
SDL_Window *SDL2Surface::getSDLSurface( ) const {
    return m_surface;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
