/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <src/Engine/RenderBackend/OGLRenderer/OGLEnum.h>
#include <osre/Debugging/osre_debugging.h>

namespace OSRE {
namespace RenderBackend {

GLenum OGLEnum::getGLBufferType( BufferType type ) {
    switch( type ) {
        case BufferType::VertexBuffer:
        case BufferType::InstanceBuffer:
            return GL_ARRAY_BUFFER;
        case BufferType::IndexBuffer:
            return GL_ELEMENT_ARRAY_BUFFER;
        case BufferType::ConstantBuffer:
            return GL_UNIFORM_BUFFER;
        case BufferType::EmptyBuffer:
        default:
            OSRE_ASSERT2( false, "Unknown enum for BufferType." );
            break;
    }

    return GL_ARRAY_BUFFER;
}

GLenum OGLEnum::getGLBufferAccessType( BufferAccessType type ) {
    switch( type ) {
        case BufferAccessType::ReadOnly:
            return GL_STATIC_DRAW;
        case BufferAccessType::WriteOnly:
            return GL_DYNAMIC_DRAW;
        case BufferAccessType::ReadWrite:
            return GL_DYNAMIC_DRAW;
        default:
            OSRE_ASSERT2( false, "Unknown enum for buffer BufferAccessType." );
            break;
    }

    return GL_STATIC_DRAW;
}

GLenum OGLEnum::getGLPrimitiveType( PrimitiveType primType ) {
    switch( primType ) {
        case PrimitiveType::PointList:
            return GL_POINTS;
        case PrimitiveType::LineList:
            return GL_LINES;
        case PrimitiveType::LineStrip:
            return GL_LINE_STRIP;
        case PrimitiveType::TriangleList:
            return GL_TRIANGLES;
        case PrimitiveType::TriangelStrip:
            return GL_TRIANGLE_STRIP;
        case PrimitiveType::TriangleFan:
            return GL_TRIANGLE_FAN;
        default:
            OSRE_ASSERT2( false, "Unknown enum for PrimitiveType." );
            break;
    }

    return GL_TRIANGLES;
}

GLenum OGLEnum::getGLIndexType( IndexType indexType ) {
    switch( indexType ) {
        case IndexType::UnsignedByte:
            return GL_UNSIGNED_BYTE;
        case IndexType::UnsignedShort:
            return GL_UNSIGNED_SHORT;
        case IndexType::UnsignedInt:
            return GL_UNSIGNED_INT;
        default:
            OSRE_ASSERT2( false, "Unknown enum for IndexType." );
            break;
    }

    return GL_UNSIGNED_SHORT;
}

GLenum OGLEnum::getGLTextureTarget( TextureTargetType type ) {
    switch( type ) {
        case TextureTargetType::Texture1D:
            return GL_TEXTURE_1D;
        case TextureTargetType::Texture2D:
            return GL_TEXTURE_2D;
        case TextureTargetType::Texture3D:
            return GL_TEXTURE_3D;
        default:
            OSRE_ASSERT2( false, "Unknown enum for TextureTargetType." );
            break;
    }

    return GL_TEXTURE_2D;
}

GLenum OGLEnum::getGLTextureEnum( TextureParameterName name ) {
    switch( name ) {
        case TextureParameterName::TextureParamMinFilter:
            return GL_TEXTURE_MIN_FILTER;
        case TextureParameterName::TextureParamMagFilter:
            return GL_TEXTURE_MAG_FILTER;
        case TextureParameterName::TextureParamWrapS:
            return GL_TEXTURE_WRAP_S;
        case TextureParameterName::TextureParamWrapT:
            return GL_TEXTURE_WRAP_T;
        default:
            OSRE_ASSERT2( false, "Unknown enum for TextureParameterName." );
            break;
    }

    return GL_TEXTURE_MIN_FILTER;
}

GLenum  OGLEnum::getGLTextureFormat(TextureFormatType texFormat) {
    switch (texFormat ) {
        case TextureFormatType::R8G8B8:
            return GL_RGB;
        case TextureFormatType::R8G8B8A8:
            return GL_RGBA;
        case TextureFormatType::InvaliTextureType:
        default:
            OSRE_ASSERT2( false, "Unknown enum for TextureParameterName." );
            break;
    }

    return GL_RGB;
}

GLenum OGLEnum::getGLTextureStage( TextureStageType texType ) {
    switch ( texType ) {
        case TextureStageType::TextureStage0:
            return GL_TEXTURE0;
        case TextureStageType::TextureStage1:
            return GL_TEXTURE1;
        case TextureStageType::TextureStage2:
            return GL_TEXTURE2;
        case TextureStageType::TextureStage3:
            return GL_TEXTURE3;
        default:
            OSRE_ASSERT2( false, "Unknown enum for TextureStageType." );
            break;
    }

    return GL_TEXTURE0;
}

GLenum OGLEnum::getOGLTypeForFormat( VertexFormat format ) {
    switch ( format ) {
        case VertexFormat::Float:
        case VertexFormat::Float2:
        case VertexFormat::Float3:
        case VertexFormat::Float4:
            return GL_FLOAT;
        case VertexFormat::Byte4:
            return GL_BYTE;
        case VertexFormat::UByte4:
            return GL_UNSIGNED_BYTE;
        case VertexFormat::Short2:
        case VertexFormat::Short4:
            return GL_SHORT;
        case VertexFormat::NumVertexFormats:
        case VertexFormat::InvalidVertexFormat:
        default:
            OSRE_ASSERT2( false, "Unknown enum for VertexFormat." );
            break;
    }

    return GL_INVALID_ENUM;
}

ui32 OGLEnum::getOGLSizeForFormat( VertexFormat format ) {
    switch ( format ) {
        case VertexFormat::Float:
            return 1;
        case VertexFormat::Float2:
        case VertexFormat::Short2:
            return 2;
        case VertexFormat::Float3:
            return 3;
        case VertexFormat::Byte4:
        case VertexFormat::UByte4:
        case VertexFormat::Float4:
        case VertexFormat::Short4:
            return 4;
        case VertexFormat::NumVertexFormats:
        case VertexFormat::InvalidVertexFormat:
            return 0;
        default:
            OSRE_ASSERT2( false, "Unknown enum for VertexFormat." );
            break;
    }

    return 0;
}

GLenum OGLEnum::getOGLCullState( CullState::CullMode cullMode ) {
    switch ( cullMode ) {
        case CullState::CullMode::CW:
            return GL_CW;
        case CullState::CullMode::CCW:
            return GL_CCW;
        default:
            OSRE_ASSERT2( false, "Unknown enum for CullState::CullMode." );
            break;
    }

    return GL_CW;
}

GLenum OGLEnum::getOGLCullFace( CullState::CullFace cullFace ) {
    switch ( cullFace ) {
        case CullState::CullFace::Front:
            return GL_FRONT;
        case CullState::CullFace::Back:
            return GL_BACK;
        case CullState::CullFace::FrontAndBack:
            return GL_FRONT_AND_BACK;
        default:
            OSRE_ASSERT2( false, "Unknown enum for CullState::CullFace." );
            break;
    }
    
    return GL_BACK;
}

GLenum OGLEnum::getOGLPolygonMode( PolygonState::PolygonMode polyMode ) {
    switch( polyMode ) {
        case PolygonState::PolygonMode::Point:
            return GL_POINT;
        case PolygonState::PolygonMode::Line:
            return GL_LINE;
        case PolygonState::PolygonMode::Fill:
            return GL_FILL;
        default:
            OSRE_ASSERT2(false, "Unknown enum for PolygonState::PolygonMode.");
            break;
    }

    return GL_FILL;
}

GLuint OGLEnum::getOGLShaderType( ShaderType type ) {
    switch ( type ) {
    case ShaderType::SH_VertexShaderType:
        return GL_VERTEX_SHADER;
    case ShaderType::SH_FragmentShaderType:
        return GL_FRAGMENT_SHADER;
    case ShaderType::SH_GeometryShaderType:
        return GL_GEOMETRY_SHADER;
    case ShaderType::SH_TesselationShaderType:
        return GL_TESS_EVALUATION_SHADER;
    default:
        OSRE_ASSERT2( false, "Unknown enum for ShaderType." );
        break;
    }

    return 0;
}

} // Namespace RenderBackend
} // Namespace OSRE
