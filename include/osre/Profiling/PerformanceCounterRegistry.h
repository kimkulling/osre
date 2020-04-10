/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Profiling/ProfilingCommon.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {
namespace Profiling {
        
//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class is used to set performance counters like FPS. You can register your own 
/// counters as well.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT PerformanceCounterRegistry {
public:
    /// @brief  
    static bool create();

    /// @brief  
    static bool destroy();

    /// @brief  
    static bool registerCounter( const String &name );

    /// @brief  
    static bool unregisterCounter( const String &name );

    /// @brief  
    static bool setCounter( const String &name, ui32 value );

    /// @brief  
    static bool resetCounter( const String &name );

    /// @brief  
    static bool addValueToCounter( const String &name, ui32 value );

    /// @brief  
    static bool queryCounter( const String &name, ui32 &counterValue );

private:
    PerformanceCounterRegistry();
    ~PerformanceCounterRegistry();

private:
    struct CounterMeasure {
        ui32 m_count;

        CounterMeasure();
        ~CounterMeasure();
    };
    static PerformanceCounterRegistry *s_instance;
    CPPCore::THashMap<ui32, CounterMeasure*> m_counters;
};

} // Namespace Profiling
} // Namespace OSRE
