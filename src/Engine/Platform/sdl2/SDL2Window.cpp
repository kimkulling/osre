/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <src/Engine/Platform/sdl2/SDL2Window.h>
#include <osre/Common/Logger.h>

#include <SDL.h>

namespace OSRE {
namespace Platform {

static const c8 *Tag = "SDL2Surface";

SDL2Surface::SDL2Surface( WindowsProperties *props )
: AbstractWindow( props )
, m_surface( nullptr ) {
    // empty
}

SDL2Surface::~SDL2Surface( ) {
    // empty
}

void SDL2Surface::setWindowsTitle( const String &title ) {
    if ( nullptr == m_surface ) {
        return;
    }

    ::SDL_SetWindowTitle( m_surface, title.c_str() );
}

SDL_Window *SDL2Surface::getSDLSurface() const {
    return m_surface;
}

bool SDL2Surface::onCreate() {
    WindowsProperties *prop = getProperties();
    if( nullptr == prop  ) {
        return false;
    }

    // Turn on double buffering with a 24bit Z buffer
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, prop->m_depthbufferdepth );

    // Create our window centered at 512x512 resolution


    const ui32 w( prop->m_width );
    const ui32 h( prop->m_height );
    ui32 sdl2Flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    if ( prop->m_resizable ) {
        sdl2Flags |= SDL_WINDOW_RESIZABLE;
    }
        
    m_surface = ::SDL_CreateWindow( prop->m_title.c_str(),
                                  SDL_WINDOWPOS_CENTERED, 
                                  SDL_WINDOWPOS_CENTERED, 
                                  w, h,
                                  sdl2Flags );
    if( nullptr == m_surface ) {
        osre_error( Tag, "Error while creating window, error: " + std::string( SDL_GetError() ) );
        return false;
    }
    ::SDL_ShowWindow( m_surface );

    return true;
}

bool SDL2Surface::onDestroy( ) {
    if( !m_surface ) {
        return false;
    }

    SDL_DestroyWindow( m_surface );
    m_surface = nullptr;

    return true;
}

bool SDL2Surface::onUpdateProperies() {
    return true;
}

void SDL2Surface::onResize( ui32 x, ui32 y, ui32 w, ui32 h ) {
    if ( nullptr == m_surface ) {
        return;
    }

    SDL_SetWindowPosition( m_surface, x, y );
    SDL_SetWindowSize( m_surface, w, h );
    WindowsProperties *props( AbstractWindow::getProperties() );
    if ( nullptr != props ) {
        props->m_x = x;
        props->m_y = y;
        props->m_width = w;
        props->m_height = h;
        AbstractWindow::setProperties( props );
    }
}

} // Namespace Platform
} // Namespace OSRE
