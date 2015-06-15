/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace IO {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::IO::SystemInfo
///	@ingroup	Infrastructure
///
///	@brief This helper class returns IO-specific system informations.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT IOSystemInfo {
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
