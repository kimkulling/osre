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
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@enum	::OSRE::System::CPUProp
///	@brief	Describes several properties of the CPU.
//-------------------------------------------------------------------------------------------------
enum CPUProp {
	CPUID_None							= 0x00000,
	CPUID_Unsupported					= 0x00001,	//< unsupported (386/486)
	CPUID_Generic						= 0x00002,	//< unrecognized processor
	CPUID_Intel							= 0x00004,	//< Intel
	CPUID_AMD							= 0x00008,	//< AMD
	CPUID_MMX							= 0x00010,	//< Multi Media Extensions
	CPUID_3DNOW							= 0x00020,	//< 3DNow!
	CPUID_SSE							= 0x00040,	//< Streaming SIMD Extensions
	CPUID_SSE2							= 0x00080,	//< Streaming SIMD Extensions 2
	CPUID_SSE3							= 0x00100	//< Streaming SIMD Extensions 3 aka Prescott's New Instructions
};

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::System::CPUInfo
///	@ingroup	Infrastructure
///
///	@brief 
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT CPUInfo {
public:
	///	@brief	The class default constructor.
	CPUInfo();

	///	@brief	The class destructor.
	~CPUInfo();

	///	@brief	Returns the number of CPUs of the target system.
	///	@return	The number of detected CPUs.
	ui32 getNumCPUs() const;

	///	@brief	Returns the properties of the CPU.
	///	@return	Flag with the detected properties.
	i32 getCPUProperties() const;

	///	@brief	Returns true, if the CPUInfo was already initiated, false if not.
	///	@return	The init state of the CPU info data.
	static bool isInited();

	///	@brief	Initiates the CPU info data.
	///	@return	true, if init was called at first, false if the CPU was not detected.
	static bool init();

protected:
	static bool initCPUProperties();

private:
	static bool m_IsInited;
	static i32 m_CPUFlags;
	static ui32 m_NumCPUs;
};

//-------------------------------------------------------------------------------------------------

} // Namespace System
} // Namespace OSRE
