/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <cppcore/Container/THashMap.h>

namespace OSRE {
namespace Profiling {
        
class OSRE_EXPORT PerformanceCounters {
public:
    static bool create();
    static bool destroy();
    static bool registerCounter( const String &name );
    static bool unregisterCounter( const String &name );
    static bool addValueToCounter( const String &name, ui32 value );
    static bool queryCounter( const String &name, ui32 &counterValue );

private:
    PerformanceCounters();
    ~PerformanceCounters();

private:
    struct CounterMeasure {
        ui32 m_count;
    };
    static PerformanceCounters *s_instance;
    CPPCore::THashMap<ui32, CounterMeasure*> m_counters;
};

} // Namespace Profiling
} // Namespace OSRE
