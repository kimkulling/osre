#pragma once

#include <osre/Common/Object.h>

namespace OSRE {
namespace Platform {

class AbstractCriticalSection;
class AbstractThreadEvent;
class AbstractThread;
class AbstractAtomic;

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Platform::IThreadFactory
///	@ingroup	Infrastructure
///
///	@brief	This abstract interface declares the factory methods for any thread implementation.
//-------------------------------------------------------------------------------------------------
class AbstractThreadFactory : public Common::Object {
public:
    ///	@brief	The destructor, virtual.
    virtual ~AbstractThreadFactory();

    virtual AbstractThread *createThread( const String &name, ui32 stacksize ) = 0;

    ///	@brief	Creates a new critical section.
    ///	@return	A pointer showing to the new critical section.
    virtual AbstractCriticalSection *createCriticalSection( ) = 0;

    ///	@brief	Creates a new thread event.
    ///	@return	A pointer showing to the new thread event.
    virtual AbstractThreadEvent *createThreadEvent() = 0;
    
    virtual AbstractAtomic *createAtomic( i32 val ) = 0;

    ///	@brief	Installs a new thread factory instance.
    ///	@param	pInstance	[in] A pointer showing to the new thread factory instance.
    static void setInstance( AbstractThreadFactory *pInstance );

    ///	@brief	Returns the installed thread factory.
    ///	@return	A pointer showing to the installed thread factory, equal NULL if no one is installed.
    static AbstractThreadFactory *getInstance();

protected:
    ///	@brief	The class constructor.
    ///	@param	name	[in] The name.
    AbstractThreadFactory( const String &name );

private:
    static AbstractThreadFactory *s_pThreadFactory;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
