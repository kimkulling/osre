/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Platform/sdl2/SDL2Window.h"
#include "Common/Logger.h"

#include <SDL.h>

namespace OSRE {
namespace Platform {

static constexpr c8 Tag[] = "SDL2Surface";

SDL2Surface::SDL2Surface(WindowsProperties *props) :
        AbstractWindow(props),
        mSurface(nullptr) {
    // empty
}

void SDL2Surface::setWindowsTitle(const String &title) {
    if (mSurface == nullptr) {
        return;
    }

    ::SDL_SetWindowTitle(mSurface, title.c_str());
}

void SDL2Surface::setWindowsMouseCursor(DefaultMouseCursorType ct) {

}

void SDL2Surface::showWindow(ShowState showState) {
    if ( nullptr == mSurface ) {
        return;
    }
        
    if (showState == ShowState::Visible) {
        ::SDL_ShowWindow(mSurface);
    } else {
        ::SDL_HideWindow(mSurface);
    }
}
        
SDL_Window *SDL2Surface::getSDLSurface() const {
    return mSurface;
}

bool SDL2Surface::onCreate() {
    WindowsProperties *prop = getProperties();
    if(prop == nullptr) {
        return false;
    }

    // Turn on double buffering with a 24bit Z buffer
    ::SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    ::SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, prop->m_depthbufferdepth);

    // Create our window centered at 512x512 resolution
    const ui32 w = prop->mRect.getWidth();
    const ui32 h = prop->mRect.getHeight();
    ui32 sdl2Flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    if ( prop->m_resizable ) {
        sdl2Flags |= SDL_WINDOW_RESIZABLE;
    }
        
    mSurface = ::SDL_CreateWindow(prop->m_title.c_str(),
                                  SDL_WINDOWPOS_CENTERED, 
                                  SDL_WINDOWPOS_CENTERED, 
                                  w, h, sdl2Flags );
    if( nullptr == mSurface ) {
        osre_error(Tag, "Error while creating window, error: " + std::string(SDL_GetError()));
        return false;
    }
    ::SDL_ShowWindow(mSurface);

    return true;
}

bool SDL2Surface::onDestroy( ) {
    if (mSurface == nullptr) {
        return false;
    }

    ::SDL_DestroyWindow( mSurface );
    mSurface = nullptr;

    return true;
}

bool SDL2Surface::onUpdateProperies() {
    return true;
}

void SDL2Surface::onResize(ui32 x, ui32 y, ui32 w, ui32 h) {
    if (mSurface == nullptr ) {
        return;
    }

    ::SDL_SetWindowPosition(mSurface, x, y);
    ::SDL_SetWindowSize(mSurface, w, h);
    WindowsProperties *props = AbstractWindow::getProperties();
    if (props != nullptr) {
        props->mRect.x1 = x;
        props->mRect.y1 = y;
        props->mRect.width = w;
        props->mRect.height = h;
        AbstractWindow::setProperties(props);
    }
}

} // Namespace Platform
} // Namespace OSRE
