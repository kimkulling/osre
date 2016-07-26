#pragma once

#include <osre/Platform/AbstractDynamicLoader.h>

namespace OSRE {
namespace Platform {
        
class SDL2DynamicLoader : public AbstractDynamicLoader {
public:
    SDL2DynamicLoader();
    virtual ~SDL2DynamicLoader();
    virtual LibHandle *load( const String &libName ) override;
    virtual LibHandle *lookupLib( const String &libName )  override;
    virtual void unload( const String &libName )  override;
    virtual void *loadFunction( const String &name ) override;
};

}
}
