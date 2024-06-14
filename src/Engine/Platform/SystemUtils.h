/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Platform/PlatformCommon.h>

namespace OSRE {
namespace System {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This class offers some system-specific functions like sleep etc. .
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT SystemUtils {
public:
	///	@brief	The class default constructor.
	SystemUtils() = default;

	///	@brief	The class destructor, non virtual.
	~SystemUtils() = default;

	///	@brief	The current thread will be sleeping for the given number of ms.
	///	@param[in] ms	ms to sleep.
	static void sleep( ui32 ms );
};

} // Namespace System
} // Namespace OSRE
