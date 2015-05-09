#include <src/Platform/sdl2/SDL2Surface.h>
#include <osre/Common/Logger.h>

#include <SDL.h>

namespace OSRE {
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
        osre_error( "Error while creating window." );
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
