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
#include "SDL2DynamicLoader.h"

#include "SDL_loadso.h"

namespace OSRE {
namespace Platform {

SDL2DynamicLoader::SDL2DynamicLoader() 
: AbstractDynamicLoader() {
    // empty
}

SDL2DynamicLoader::~SDL2DynamicLoader() {
    // empty
}

LibHandle *SDL2DynamicLoader::load( const String &libName ) {
    if ( libName.empty() ) {
        return nullptr;
    }
    void *handle( SDL_LoadObject( libName.c_str() ) );
    if ( nullptr != handle ) {
        LibHandle *libHandle = new LibHandle;
        libHandle->m_handle = handle;
        AbstractDynamicLoader::addLib( libName, libHandle );
        AbstractDynamicLoader::setActiveLib( libHandle );
        return libHandle;
    }

    return nullptr;
}

LibHandle *SDL2DynamicLoader::lookupLib( const String &libName ) {
    LibHandle *handle = AbstractDynamicLoader::findLib( libName );

    return handle;
}

void SDL2DynamicLoader::unload( const String &libName ) {
    if ( libName.empty() ) {
        return;
    }

    LibHandle *handle = AbstractDynamicLoader::findLib( libName );
    if ( nullptr != handle ) {
        SDL_UnloadObject( handle->m_handle );
        AbstractDynamicLoader::removeLib( libName );
        AbstractDynamicLoader::setActiveLib( nullptr );
    }
}

void *SDL2DynamicLoader::loadFunction( const String &libName ) {
    return SDL_LoadFunction( getActiveLib()->m_handle, libName.c_str() );
}

} // Namespace Platform
} // Namespace OSRE
