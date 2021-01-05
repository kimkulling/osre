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
#include <osre/RenderBackend/THWBufferManager.h>
#include <osre/Common/StringUtils.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;


const c8 *getVertexCompShortCut( VertexAttribute &attrib ) {
    switch (attrib) {
        case VertexAttribute::Position:
            return "p";
        case VertexAttribute::Normal:
            return "n";
        case VertexAttribute::TexCoord0:
            return "t0";
        case VertexAttribute::TexCoord1:
            return "t1";
        case VertexAttribute::TexCoord2:
            return "t2";
        case VertexAttribute::TexCoord3:
            return "t3";
        case VertexAttribute::Tangent:
            return "ta";
        case VertexAttribute::Binormal:
            return "bt";
        case VertexAttribute::Weights:
            return "w";
        case VertexAttribute::Indices:
            return "i";
        case VertexAttribute::Color0:
            return "c0";
        case VertexAttribute::Color1:
            return "c1";
        case VertexAttribute::Instance0:
            return "i0";
        case VertexAttribute::Instance1:
            return "i1";
        case VertexAttribute::Instance2:
            return "i2";
        case VertexAttribute::Instance3:
            return "i3";
        default:
            break;
    }

    return nullptr;
}

const c8 *getAccessShortCut( BufferAccessType access ) {
    switch (access) {
        case BufferAccessType::ReadOnly:
            return "r";
        case BufferAccessType::WriteOnly:
            return "w";
        default:
            break;
    }
    
    return nullptr;
}



}
}
