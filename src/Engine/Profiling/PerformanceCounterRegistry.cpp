/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Profiling/PerformanceCounterRegistry.h>
#include <osre/Common/StringUtils.h>

namespace OSRE {
namespace Profiling {

using namespace ::OSRE::Common;

PerformanceCounterRegistry *PerformanceCounterRegistry::s_instance = nullptr;

PerformanceCounterRegistry::CounterMeasure::CounterMeasure()
: m_count( 0 ) {
    // empty
}

PerformanceCounterRegistry::CounterMeasure::~CounterMeasure() {
    // empty
}

PerformanceCounterRegistry::PerformanceCounterRegistry()
: m_counters() {

}
    
PerformanceCounterRegistry::~PerformanceCounterRegistry() {

}
    
bool PerformanceCounterRegistry::create() {
    if ( nullptr != s_instance ) {
        return false;
    }

    s_instance = new PerformanceCounterRegistry;

    return true;
}

bool PerformanceCounterRegistry::destroy() {
    if ( nullptr == s_instance ) {
        return false;
    }

    delete s_instance;
    s_instance = nullptr;

    return true;
}

bool PerformanceCounterRegistry::registerCounter( const String &name ) {
    if ( nullptr == s_instance ) {
        return false;
    }

    const ui32 hash( StringUtils::hashName( name.c_str() ) );
    if ( s_instance->m_counters.hasKey( hash ) ) {
        return false;
    }
    CounterMeasure *cm( new CounterMeasure );
    s_instance->m_counters.insert( hash, cm );

    return true;
}
    
bool PerformanceCounterRegistry::unregisterCounter( const String &name ) {
    if ( nullptr == s_instance ) {
        return false;
    }

    const ui32 hash( StringUtils::hashName( name.c_str() ) );
    if ( !s_instance->m_counters.hasKey( hash ) ) {
        return false;
    }

    CounterMeasure *cm( nullptr );
    s_instance->m_counters.getValue( hash, cm );

    if ( nullptr == cm ) {
        return false;
    } else {
        delete cm;
    }

    return s_instance->m_counters.remove( hash );
}

bool PerformanceCounterRegistry::setCounter( const String &name, ui32 value ) {
    if ( nullptr == s_instance ) {
        return false;
    }

    const ui32 hash( StringUtils::hashName( name.c_str() ) );
    if ( !s_instance->m_counters.hasKey( hash ) ) {
        return false;
    }
    CounterMeasure *cm( nullptr );
    s_instance->m_counters.getValue( hash, cm );
    cm->m_count = value;

    return true;
}

bool PerformanceCounterRegistry::resetCounter( const String &name ) {
    return setCounter( name, 0 );
}

bool PerformanceCounterRegistry::addValueToCounter( const String &name, ui32 value ) {
    if ( nullptr == s_instance ) {
        return false;
    }

    const ui32 hash( StringUtils::hashName( name.c_str() ) );
    if ( !s_instance->m_counters.hasKey( hash ) ) {
        return false;
    }

    CounterMeasure *cm( nullptr );
    s_instance->m_counters.getValue( hash, cm );
    if ( nullptr == cm ) {
        return false;
    }

    cm->m_count += value;

    return true;
}

bool PerformanceCounterRegistry::queryCounter( const String &name, ui32 &counterValue ) {
    if ( nullptr == s_instance ) {
        return false;
    }

    const ui32 hash( StringUtils::hashName( name.c_str() ) );
    if ( !s_instance->m_counters.hasKey( hash ) ) {
        return false;
    }

    CounterMeasure *cm( nullptr );
    s_instance->m_counters.getValue( hash, cm );
    if ( nullptr == cm ) {
        return false;
    }

    counterValue = cm->m_count;

    return true;
}

} // Namespace Profiling
} // Namespace OSRE
