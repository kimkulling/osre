/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Profiling/PerformanceCounterRegistry.h"
#include "Common/StringUtils.h"

namespace OSRE {
namespace Profiling {

using namespace ::OSRE::Common;

PerformanceCounterRegistry *PerformanceCounterRegistry::sInstance = nullptr;

PerformanceCounterRegistry::PerformanceCounterRegistry() : mCounters() {
    // empty
}

bool PerformanceCounterRegistry::create() {
    if ( nullptr != sInstance ) {
        return false;
    }

    sInstance = new PerformanceCounterRegistry;

    return true;
}

bool PerformanceCounterRegistry::destroy() {
    if ( nullptr == sInstance ) {
        return false;
    }

    delete sInstance;
    sInstance = nullptr;

    return true;
}

bool PerformanceCounterRegistry::registerCounter(const String &name) {
    if ( nullptr == sInstance ) {
        return false;
    }

    const HashId hash = StringUtils::hashName(name.c_str());
    if (sInstance->mCounters.hasKey(hash)) {
        return false;
    }
    CounterMeasure *cm = new CounterMeasure;
    sInstance->mCounters.insert(hash, cm);

    return true;
}
    
bool PerformanceCounterRegistry::unregisterCounter(const String &name) {
    if (nullptr == sInstance) {
        return false;
    }

    const HashId hash = StringUtils::hashName(name.c_str());
    if (!sInstance->mCounters.hasKey(hash)) {
        return false;
    }

    CounterMeasure *cm = nullptr; 
    sInstance->mCounters.getValue( hash, cm );

    if ( nullptr == cm ) {
        return false;
    } 
    delete cm;

    return sInstance->mCounters.remove(hash);
}

bool PerformanceCounterRegistry::setCounter(const String &name, ui32 value) {
    if ( nullptr == sInstance ) {
        return false;
    }

    const HashId hash = StringUtils::hashName(name.c_str());
    if (!sInstance->mCounters.hasKey(hash)) {
        return false;
    }
    CounterMeasure *cm = nullptr;
    sInstance->mCounters.getValue(hash, cm);
    cm->count = value;

    return true;
}

bool PerformanceCounterRegistry::resetCounter( const String &name ) {
    return setCounter( name, 0 );
}

bool PerformanceCounterRegistry::addValueToCounter( const String &name, ui32 value ) {
    if ( nullptr == sInstance ) {
        return false;
    }

    const HashId hash = StringUtils::hashName(name.c_str());
    if ( !sInstance->mCounters.hasKey(hash)) {
        return false;
    }

    CounterMeasure *cm = nullptr;
    sInstance->mCounters.getValue( hash, cm );
    if ( nullptr == cm ) {
        return false;
    }

    cm->count += value;

    return true;
}

bool PerformanceCounterRegistry::queryCounter( const String &name, ui32 &counterValue ) {
    if (nullptr == sInstance) {
        return false;
    }

    const HashId hash = StringUtils::hashName(name.c_str());
    if ( !sInstance->mCounters.hasKey(hash) ) {
        return false;
    }

    CounterMeasure *cm = nullptr;
    sInstance->mCounters.getValue( hash, cm );
    if ( nullptr == cm ) {
        return false;
    }

    counterValue = cm->count;

    return true;
}

} // Namespace Profiling
} // Namespace OSRE
