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

#include "Common/osre_common.h"

namespace OSRE {
namespace Animation {
    
struct VectorKey {
    f32 Time;
    glm::vec3 Value; 

    VectorKey() : Time(1.0f), Value(1) {
        // empty
    }

    ~VectorKey() = default;
};

struct VectorChannel {
    size_t NumVectorKeys;
    VectorKey *VectorKeys;

    VectorChannel() : NumVectorKeys(0), VectorKeys(nullptr) {
        // empty
    }

    ~VectorChannel() {
        delete [] VectorKeys;
        VectorKeys = nullptr;
    }
};

struct AnimationTrack {
    f32 Duration;
    f32 mTicksPerSecond;    
    size_t NumVectorChannels;
    VectorChannel *VectorChannels;

    AnimationTrack() : 
            Duration(1.0f), mTicksPerSecond(1.0f), NumVectorChannels(0l), VectorChannels(nullptr) {
        // empty
    }

    ~AnimationTrack() {
        delete [] VectorChannels;
        VectorChannels = nullptr;
    } 
};

}
}
