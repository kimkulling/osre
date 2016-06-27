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
