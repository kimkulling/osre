#pragma once

#include <osre/Platform/AbstractThreadFactory.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Platform::Win32ThreadFactory
///	@ingroup	Infrastructure
///
///	@brief 
//-------------------------------------------------------------------------------------------------
class SDL2ThreadFactory : public AbstractThreadFactory {
public:
    SDL2ThreadFactory( );
    virtual ~SDL2ThreadFactory( );
    virtual AbstractThread *createThread( const String &name, ui32 stacksize );
    virtual AbstractCriticalSection *createCriticalSection( );
    virtual AbstractThreadEvent *createThreadEvent();
    virtual AbstractAtomic *createAtomic( i32 val );
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
