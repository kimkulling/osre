#pragma once

#include "OGLCommon.h"

namespace OSRE {
namespace RenderBackend {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderBackend::OGLEnum
///	@ingroup	RenderSystem
///
///	@brief  This utility class implements the OpenGL enum lookup.
//-------------------------------------------------------------------------------------------------
class OGLEnum {
public:
    ///	@brief  Translates the buffer type to OpenGL.
    static GLenum getGLBufferType( BufferType type );
    ///	@brief  Translates the access type to OpenGL.
    static GLenum getGLBufferAccessType( BufferAccessType type );
    ///	@brief  Translates the primitive type to OpenGL.
    static GLenum getGLPrimitiveType( PrimitiveType primType );
    ///	@brief  Translates the index type to OpenGL.
    static GLenum getGLIndexType( IndexType indexType );
    ///	@brief  Translates the texture type to OpenGL.
    static GLenum getGLTextureTarget( TextureTargetType type );
    ///	@brief  Translates the texture parameter type to OpenGL.
    static GLenum getGLTextureParameterName( TextureParameterName name );

private:
    OGLEnum();
    ~OGLEnum();
    OGLEnum( const OGLEnum & );
};

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace OSRE
