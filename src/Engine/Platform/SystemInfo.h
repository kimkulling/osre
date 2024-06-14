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
#include <osre/Platform/Threading.h>
#include <map>

#ifdef OSRE_WINDOWS
    typedef unsigned long DWORD;
#endif

namespace OSRE {
namespace Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief This class implements system-info services, that are platform specific.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT SystemInfo {
public:
    ///	@enum	Platform
    ///	@brief	This enum describes the platform.
    enum class Platform {
        Windows,		///< Windows platform
        GNU_Linux	///< GNU Linux platform.
    };

public:
    ///	@brief	The class default constructor.
    SystemInfo() = default;

    ///	@brief	The class destructor.
    ~SystemInfo() = default;

    ///	@brief	Returns the platform type.
    ///	@return	The platform type.
    static Platform getPlatform();

    ///	@brief	The memory states will be read out.
    ///	@param	rTotalPhysicMem		[ out ] The total available physical memory.
    ///	@param	rMemInUse			[ out ] The memory in use in percent.
    static void getMemoryStatus( ui32 &rTotalPhysicMem, ui32 &rMemInUse );


    static bool registerThreadName( const ThreadId &id, const String &name );
    static bool unregisterThreadName( const ThreadId &id );
    static String getThreadName( const ThreadId &id );
    static String getCurrentThreadName();

protected:
    static bool init();

private:
    SystemInfo( const SystemInfo & ) = delete;
    SystemInfo &operator = ( const SystemInfo & ) = delete;

private:
    static bool m_IsInited;

#ifdef OSRE_WINDOWS
    typedef std::map<DWORD, String> ThreadNameMap;
#else
    typedef std::map<ui32, String> ThreadNameMap;
#endif
    static ThreadNameMap s_threadNames;
};

} // Namespace Platform
} // Namespace OSRE
