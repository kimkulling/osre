#pragma once

#include <osre/Platform/AbstractThreadFactory.h>

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::Win32ThreadFactory
///	@ingroup	Infrastructure
///
///	@brief 
//-------------------------------------------------------------------------------------------------
class Win32ThreadFactory : public AbstractThreadFactory {
public:
    Win32ThreadFactory();
    virtual ~Win32ThreadFactory( );
    virtual AbstractThread *createThread( const String &name, ui32 stacksize );
    virtual AbstractCriticalSection *createCriticalSection( );
    virtual AbstractThreadEvent *createThreadEvent( );
    virtual AbstractAtomic *createAtomic( i32 val );
};

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2
