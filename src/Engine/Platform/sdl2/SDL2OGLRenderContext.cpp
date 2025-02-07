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
#include "Platform/sdl2/SDL2OGLRenderContext.h"
#include "Platform/sdl2/SDL2Window.h"
#include "Common/Logger.h"

#include <SDL.h>
#include <GL/glew.h>

namespace OSRE {
namespace Platform {

static constexpr c8 Tag[] = "SDL2RenderContext";

SDL2RenderContext::SDL2RenderContext() : 
        AbstractOGLRenderContext(), mRenderContext( nullptr ), mSurface( nullptr ), mIsActive( false ) {
    // empty
}

bool SDL2RenderContext::onCreate(AbstractWindow *window) {
    printf("SDL2RenderContext::onCreate(AbstractWindow *window)\n");
    if (window == nullptr) {
        osre_error( Tag, "Surface pointer is a nullptr." );
        return false;
    }

    mSurface = reinterpret_cast<SDL2Surface*>( window );
    mRenderContext = SDL_GL_CreateContext( mSurface->getSDLSurface() );
    if( mRenderContext == nullptr) {
        osre_error( Tag, "Error while creating GL-context.!" );
        return false;
    }
    
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if( GLEW_OK != err ) {
        osre_error( Tag, "GLEW is not initialized!" );
        return false;
    }

    const GLubyte *version = glGetString( GL_VERSION );
    osre_info(Tag, "OpenGL renderer initiated.");
    osre_info(Tag, "Version : " + String((c8*)version));

    return true;
}

//-------------------------------------------------------------------------------------------------
bool SDL2RenderContext::onDestroy( ) {
    if (mRenderContext == nullptr) {
        osre_error( Tag, "Context pointer is a nullptr." );
        return false;
    }

    SDL_GL_DeleteContext( mRenderContext );
    mRenderContext = nullptr;

    return true;
}

//-------------------------------------------------------------------------------------------------
bool SDL2RenderContext::onUpdate( ) {
    if ( !mIsActive ) {
        osre_debug( Tag, "No active render context." );
        return false;
    }
    SDL_GL_SwapWindow(mSurface->getSDLSurface());

    return true;
}

//-------------------------------------------------------------------------------------------------
bool SDL2RenderContext::onActivate( ) {
    mIsActive = true;
    const i32 retCode = SDL_GL_MakeCurrent( mSurface->getSDLSurface(), mRenderContext );
    return ( retCode == 0 );
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace OSRE
