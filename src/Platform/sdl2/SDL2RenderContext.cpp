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
#include <Code/Infrastructure/Platform/sdl2/SDL2RenderContext.h>
#include <Code/Infrastructure/Platform/sdl2/SDL2Surface.h>
#include <osre2/Infrastructure/Common/Logger.h>

#include <SDL.h>
#include <GL/glew.h>

namespace ZFXCE2 {
namespace Platform {

//-------------------------------------------------------------------------------------------------
SDL2RenderContext::SDL2RenderContext()
: AbstractRenderContext()
, m_pContext( nullptr )
, m_pSurface( nullptr )
, m_isActive( false ) {
// empty
}

//-------------------------------------------------------------------------------------------------
SDL2RenderContext::~SDL2RenderContext( ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
bool SDL2RenderContext::onCreate( AbstractSurface *pSurface ) {
    if( !pSurface ) {
        ce_error( "Surface pointer is a nullptr." );
        return false;
    }

    m_pSurface = reinterpret_cast< SDL2Surface* >( pSurface );
    m_pContext = SDL_GL_CreateContext( m_pSurface->getSDLSurface() );
    if( !m_pContext ) {
        ce_error( "Error while creating GL-context.!" );
        return false;
    }
    
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if( GLEW_OK != err ) {
        ce_error( "GLEW is not initialized!" );
        return false;
    }

    const GLubyte *version = glGetString( GL_VERSION );
    ce_log( "OpenGL renderer initiated.")
    ce_log( "Version : " + ce_string( (c8*) version ) );

    return true;
}

//-------------------------------------------------------------------------------------------------
bool SDL2RenderContext::onDestroy( ) {
    if( !m_pContext ) {
        ce_error( "Context pointer is a nullptr." );
        return false;
    }

    SDL_GL_DeleteContext( m_pContext );
    m_pContext = nullptr;

    return true;
}

//-------------------------------------------------------------------------------------------------
bool SDL2RenderContext::onUpdate( ) {
    if ( !m_isActive ) {
        ce_debug( "No active render context." );
    }
    SDL_GL_SwapWindow( m_pSurface->getSDLSurface() );

    return true;
}

//-------------------------------------------------------------------------------------------------
bool SDL2RenderContext::onActivate( ) {
    m_isActive = true;
    const i32 retCode( SDL_GL_MakeCurrent( m_pSurface->getSDLSurface(), m_pContext ) );
    return ( retCode == 0 );
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
