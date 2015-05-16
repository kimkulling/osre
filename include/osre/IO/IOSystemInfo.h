#pragma once

#include <osre/Common/Types.h>

namespace OSRE {
namespace IO {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::IO::SystemInfo
///	@ingroup	Infrastructure
///
///	@brief This helper class returns IO-specific system informations.
//-------------------------------------------------------------------------------------------------
class IOSystemInfo {
public:
	///	@brief	The class constructor.
	IOSystemInfo();

	///	@brief	The class destructor.
	~IOSystemInfo();

	///	@brief	Returns the directory separator of the current platform.
	///	@return	The directory separator ( for instance using a Unix platform / will be returned ).
	static String getDirSeparator();

	///	@brief	Returns the token to mark the local directory.
	///	@return	The token for the local directory.
	static String getCurrentDirToken();

	///	@brief	The current directory will be returned.
	///	@return	The current directory.
	static String getCurrentDirectory();
};

//-------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
