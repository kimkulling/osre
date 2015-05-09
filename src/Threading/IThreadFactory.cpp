#include <osre/Platform/AbstractThreadFactory.h>

namespace OSRE {
namespace Platform {

AbstractThreadFactory *AbstractThreadFactory::s_pThreadFactory = nullptr;

//-------------------------------------------------------------------------------------------------
AbstractThreadFactory::AbstractThreadFactory(  const String &rFactoryName ) :
	Object( rFactoryName )
{
	// empty
}

//-------------------------------------------------------------------------------------------------
AbstractThreadFactory::~AbstractThreadFactory() {
	// empty
}

//-------------------------------------------------------------------------------------------------
void AbstractThreadFactory::setInstance( AbstractThreadFactory *pInstance ) {
	s_pThreadFactory = pInstance;
}

//-------------------------------------------------------------------------------------------------
AbstractThreadFactory *AbstractThreadFactory::getInstance() {
	return s_pThreadFactory;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Platform
} // Namespace ZFXCE2
