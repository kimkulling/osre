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
#include <src/RenderBackend/OGLRenderer/OGLEnum.h>

namespace OSRE {
namespace RenderBackend {

//-------------------------------------------------------------------------------------------------
GLenum OGLEnum::getGLBufferType( BufferType type ) {
    switch( type ) {
        case VertexBuffer:
        case InstanceBuffer:
            return GL_ARRAY_BUFFER;
        case IndexBuffer:
            return GL_ELEMENT_ARRAY_BUFFER;
        case EmptyBuffer:
        default:
            break;
    }
    return GL_ARRAY_BUFFER;
}

//-------------------------------------------------------------------------------------------------
GLenum OGLEnum::getGLBufferAccessType( BufferAccessType type ) {
    switch( type ) {
        case ReadOnly:
            return GL_STATIC_DRAW;
        case WriteOnly:
            return GL_DYNAMIC_DRAW;
        case ReadWrite:
            return GL_DYNAMIC_DRAW;
    }
    return GL_STATIC_DRAW;
}

//-------------------------------------------------------------------------------------------------
GLenum OGLEnum::getGLPrimitiveType( PrimitiveType primType ) {
    switch( primType ) {
        case PointList:
            return GL_POINTS;
        case LineList:
            return GL_LINES;
        case LineStrip:
            return GL_LINE_STRIP;
        case TriangleList:
            return GL_TRIANGLES;
        case TriangelStrip:
            return GL_TRIANGLE_STRIP;
        case TriangleFan:
            return GL_TRIANGLE_FAN;
        default:
            break;
    }

    return GL_TRIANGLES;
}

//-------------------------------------------------------------------------------------------------
GLenum OGLEnum::getGLIndexType( IndexType indexType ) {
    switch( indexType ) {
        case UnsignedByte:
            return GL_UNSIGNED_BYTE;
        case UnsignedShort:
            return GL_UNSIGNED_SHORT;
        default:
            break;
    }
    return GL_UNSIGNED_SHORT;
}

//-------------------------------------------------------------------------------------------------
GLenum OGLEnum::getGLTextureTarget( TextureTargetType type ) {
    switch( type ) {
        case Texture1D:
            return GL_TEXTURE_1D;
        case Texture2D:
            return GL_TEXTURE_2D;
        case Texture3D:
            return GL_TEXTURE_3D;
        default:
            break;
    }

    return GL_TEXTURE_2D;
}

//-------------------------------------------------------------------------------------------------
GLenum OGLEnum::getGLTextureParameterName( TextureParameterName name ) {
    switch( name ) {
        case TextureParamMinFilter:
            return GL_TEXTURE_MIN_FILTER;
        case TextureParamMagFilter:
            return GL_TEXTURE_MAG_FILTER;
        case TextureParamWrapS:
            return GL_TEXTURE_WRAP_S;
        case TextureParamWrapT:
            return GL_TEXTURE_WRAP_T;
        default:
            break;
    }

    return GL_TEXTURE_MIN_FILTER;
}

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace OSRE
