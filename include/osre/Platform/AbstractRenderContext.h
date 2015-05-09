#pragma once

#include <osre/Common/Types.h>

namespace OSRE {
namespace Platform {

class AbstractSurface;

//-------------------------------------------------------------------------------------------------
///	@class		
///	@ingroup	
///
///	@brief
//-------------------------------------------------------------------------------------------------
class AbstractRenderContext {
public:
    virtual ~AbstractRenderContext();
    bool create( AbstractSurface *pSurface );
    bool destroy();
    bool activate();
    bool update();

protected:
    virtual bool onCreate( AbstractSurface *pSurface ) = 0;
    virtual bool onDestroy() = 0;
    virtual bool onUpdate() = 0;
    virtual bool onActivate() = 0;

protected:
    AbstractRenderContext();
};

//-------------------------------------------------------------------------------------------------
inline
AbstractRenderContext::AbstractRenderContext( ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline
AbstractRenderContext::~AbstractRenderContext( ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline
bool AbstractRenderContext::create( AbstractSurface *pSurface ) {
    return onCreate( pSurface );
}

//-------------------------------------------------------------------------------------------------
inline
bool AbstractRenderContext::destroy() {
    return onDestroy();
}


//-------------------------------------------------------------------------------------------------
inline
bool AbstractRenderContext::activate() {
    return onActivate();
}

//-------------------------------------------------------------------------------------------------
inline
bool AbstractRenderContext::update() {
    return onUpdate();
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
