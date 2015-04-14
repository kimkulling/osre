/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-2015, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions
and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
and the following disclaimer in the documentation and/or other materials provided with the
distribution.

* Neither the name of the ZFXCE2 team, nor the names of its contributors may be used to endorse or
promote products derived from this software without specific prior written permission of the
ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
#include <Code/RenderSystem/RenderBackend/OGLRenderer/OGLEnum.h>

namespace ZFXCE2 {
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
} // Namespace ZFXCE2
