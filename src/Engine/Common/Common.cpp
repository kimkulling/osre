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
#include "Common/osre_common.h"
#include "Common/Logger.h"

namespace OSRE {

static constexpr c8 Tag[] = "Common";

size_t MemoryStatistics::allocated = 0;
size_t MemoryStatistics::numNew = 0;
size_t MemoryStatistics::activeAllocs = 0;

void MemoryStatistics::addAllocated( size_t allocSize ) {
    numNew++;
    activeAllocs++;
    allocated += allocSize;
}

void MemoryStatistics::releaseAlloc() {
    activeAllocs--;
}

void MemoryStatistics::showStatistics() {
    osre_info(Tag, "Memory-Statistics\n=================");
    std::stringstream stream;
    stream << "\nSum Allocs      : " << numNew << "\n";
    stream << "Number of leaks : " << activeAllocs << "\n";
    osre_info(Tag, stream.str());
}

}

void *operator new(size_t size) {
#ifdef _DEBUG
    OSRE::MemoryStatistics::addAllocated(size);
#endif

    return std::malloc(size);
}

void operator delete(void *ptr) noexcept {
#ifdef _DEBUG
    OSRE::MemoryStatistics::releaseAlloc();
#endif

    std::free(ptr);
}
