/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "OGLRenderBackend.h"
#include "OGLShader.h"
#include "OGLCommon.h"
#include "OGLEnum.h"
#include <osre/RenderBackend/FontBase.h>
#include <osre/RenderBackend/ClearState.h>
#include <osre/Platform/AbstractRenderContext.h>
#include <osre/Profiling/PerformanceCounters.h>
#include <osre/Common/Logger.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/Stream.h>
#include <osre/IO/Uri.h>

#include <cppcore/CPPCoreCommon.h>

#include "SOIL.h"

#include <iostream>

namespace OSRE {
namespace RenderBackend {

using namespace ::CPPCore;

static const String Tag             = "OGLRenderBackend";
static const ui32   NotInitedHandle = 9999999;

OGLRenderBackend::OGLRenderBackend( )
: m_renderCtx( nullptr )
, m_buffers()
, m_activeVB( NotInitedHandle )
, m_activeIB( NotInitedHandle )
, m_vertexarrays()
, m_activeVertexArray( OGLNotSetId )
, m_shaders()
, m_textures()
, m_freeTexSlots()
, m_fonts()
, m_activeFont( nullptr )
, m_texLookupMap()
, m_parameters()
, m_shaderInUse( nullptr )
, m_freeBufferSlots()
, m_primitives()
, m_fpsCounter( nullptr ) {
    // empty
}

OGLRenderBackend::~OGLRenderBackend( ) {
    releaseAllShaders();
    releaseAllFonts();
    releaseAllTextures();
    releaseAllVertexArrays( );
    releaseAllBuffers();
    releaseAllParameters();
    releaseAllPrimitiveGroups();
}

void OGLRenderBackend::setTimer( Platform::AbstractTimer *timer ) {
    if ( nullptr == m_fpsCounter ) {
        m_fpsCounter = new Profiling::FPSCounter( timer );
    }
}

void OGLRenderBackend::setRenderContext( Platform::AbstractRenderContext *renderCtx ) {
    if ( m_renderCtx != renderCtx ) {
        m_renderCtx = renderCtx;
        if ( nullptr != m_renderCtx ) {
            m_renderCtx->activate();
        }
    }
}

void OGLRenderBackend::clearRenderTarget( const ClearState &clearState ) {
    GLbitfield glTarget( 0 );
    const ui32 clear( clearState.getClearState() );
    if( clear & ClearState::ColorBit ) {
        glTarget |= GL_COLOR_BUFFER_BIT;
    }
    if( clear & ClearState::DepthBit ) {
        glTarget |= GL_DEPTH_BUFFER_BIT;
    }
    if( clear & ClearState::StencilBit ) {
        glTarget |= GL_STENCIL_BUFFER_BIT;
    }

    glClear( glTarget );
}

void OGLRenderBackend::setViewport( i32 x, i32 y, i32 w, i32 h ) {
    glViewport( x, y, w, h );
}

OGLBuffer *OGLRenderBackend::createBuffer( BufferType type ) {
    ui32 handle( OGLNotSetId );
    GLuint bufferId( OGLNotSetId );
    glGenBuffers( 1, &bufferId );
    OGLBuffer *buffer( nullptr );
    if ( m_freeBufferSlots.isEmpty() ) {
        buffer = new OGLBuffer;
        handle = m_buffers.size();
        m_buffers.add( buffer );
    } else {
        handle = m_freeBufferSlots.back();
        buffer = m_buffers[ handle ];
        m_freeBufferSlots.removeBack();
    }
    buffer->m_handle = handle;
    buffer->m_type = type;
    buffer->m_oglId   = bufferId;
    buffer->m_size = 0;

    return buffer;
}

OGLBuffer *OGLRenderBackend::getBufferById( ui32 geoId ) {
    OGLBuffer *buffer( nullptr );
    for ( ui32 i = 0; i < m_buffers.size(); i++ ) {
        if ( m_buffers[ i ]->m_geoId == geoId ) {
            buffer = m_buffers[ i ];
            break;
        }
    }
    return buffer;
}

void OGLRenderBackend::bindBuffer( OGLBuffer *buffer ) {
    if ( nullptr == buffer ) {
        osre_debug( Tag, "Pointer to buffer is nullptr" );
        return;
    }

    /*if ( BufferType::VertexBuffer == buffer->m_type ) {
        if ( m_activeVB == buffer->m_oglId ) {
            return;
        }
        m_activeVB = buffer->m_oglId;
    } else if ( BufferType::IndexBuffer == buffer->m_type ) {
        if ( m_activeIB == buffer->m_oglId ) {
            return;
        }
        m_activeIB = buffer->m_oglId;
    }*/
 
    GLenum target = OGLEnum::getGLBufferType( buffer->m_type );
    glBindBuffer( target, buffer->m_oglId );

    //CHECKOGLERRORSTATE();
}

void OGLRenderBackend::bindBuffer( ui32 handle ) {
    OGLBuffer *buf( m_buffers[ handle ] );
    if( nullptr != buf ) {
        bindBuffer( buf );
    }
}

void OGLRenderBackend::unbindBuffer( OGLBuffer *buffer ) {
    if ( nullptr == buffer ) {
        osre_debug( Tag, "Pointer to buffer is nullptr" );
        return;
    }
    m_activeVB = NotInitedHandle;
    m_activeIB = NotInitedHandle;
    GLenum target = OGLEnum::getGLBufferType( buffer->m_type );
    glBindBuffer( target, 0 );

    //CHECKOGLERRORSTATE();
}

void OGLRenderBackend::bufferData( OGLBuffer *buffer, void *data, ui32 size, BufferAccessType usage ) {
    if ( nullptr == buffer ) {
        osre_debug( Tag, "Pointer to buffer is nullptr" );
        return;
    }
    GLenum target = OGLEnum::getGLBufferType( buffer->m_type );
    glBufferData( target, size, data, OGLEnum::getGLBufferAccessType( usage ) );
    
    //CHECKOGLERRORSTATE();
}

void OGLRenderBackend::releaseBuffer( OGLBuffer *buffer ) {
    if ( nullptr == buffer ) {
        osre_debug( Tag, "Pointer to buffer instance is nullptr" );
        return;
    }
    const ui32 slot = buffer->m_handle;
    glDeleteBuffers( 1, &buffer->m_oglId );
    buffer->m_handle = OGLNotSetId;
    buffer->m_type   = BufferType::EmptyBuffer;
    buffer->m_oglId     = OGLNotSetId;
    m_freeBufferSlots.add( slot );
}

void OGLRenderBackend::releaseAllBuffers() {
    for ( ui32 i=0; i<m_buffers.size(); ++i ) {
        OGLBuffer *buffer = m_buffers[ i ];
        if ( nullptr != buffer ) {
            if ( buffer->m_type != BufferType::EmptyBuffer ) {
                releaseBuffer( buffer );
            }
        }
    }
    m_buffers.clear();
    m_freeBufferSlots.clear();
}

bool OGLRenderBackend::createVertexCompArray( const VertexLayout *layout, OGLShader *shader, VertAttribArray &attributes ) {
    if( nullptr == layout ) {
        osre_debug( Tag, "Pointer to layout instance is nullptr" );
        return false;
    }

    if( nullptr == shader ) {
        osre_debug( Tag, "Pointer to shader instance is nullptr" );
        return false;
    }

    ui32 index( 0 );
    OGLVertexAttribute *attribute( nullptr );
    for( ui32 i = 0; i < layout->numComponents(); i++ ) {
        VertComponent &comp( layout->getAt( i ) );
        attribute = new OGLVertexAttribute;
        attribute->m_pAttributeName = getVertCompName( comp.m_attrib ).c_str();
        attribute->m_index = ( ( *shader )[ attribute->m_pAttributeName ] );
        attribute->m_size = OGLEnum::getOGLSizeForFormat( comp.m_format );
        attribute->m_type = OGLEnum::getOGLTypeForFormat( comp.m_format );
        attribute->m_ptr = (GLvoid*) index;
        attributes.add( attribute );
        index += attribute->m_size;
    }

    return true;
}

bool OGLRenderBackend::createVertexCompArray( VertexType type, OGLShader *shader, VertAttribArray &attributes ) {
    if( nullptr == shader ) {
        osre_debug( Tag, "Pointer to shader instance is nullptr" );
        return false;
    }

    OGLVertexAttribute *attribute( nullptr );
    switch( type ) {
        case VertexType::ColorVertex:
            attribute                   = new OGLVertexAttribute;
            attribute->m_pAttributeName = getVertCompName( VertexAttribute::Position ).c_str();
            attribute->m_index = ( ( *shader )[ attribute->m_pAttributeName ] );
            attribute->m_size           = 3;
            attribute->m_type           = GL_FLOAT;
            attribute->m_ptr            = 0;
            attributes.add( attribute );

            attribute = new OGLVertexAttribute;
            attribute->m_pAttributeName = getVertCompName( VertexAttribute::Normal ).c_str();
            attribute->m_index = ( *shader )[ attribute->m_pAttributeName ];
            attribute->m_size = 3;
            attribute->m_type = GL_FLOAT;
            attribute->m_ptr = ( const GLvoid* ) offsetof( ColorVert, normal );
            attributes.add( attribute );

            attribute = new OGLVertexAttribute;
            attribute->m_pAttributeName = getVertCompName( VertexAttribute::Color0 ).c_str();
            attribute->m_index = ( *shader )[ attribute->m_pAttributeName ];
            attribute->m_size = 3;
            attribute->m_type = GL_FLOAT;
            attribute->m_ptr = ( const GLvoid* ) offsetof( ColorVert, color0 );
            attributes.add( attribute );
            break;

        case VertexType::RenderVertex:
            attribute = new OGLVertexAttribute;
            attribute->m_pAttributeName = getVertCompName( VertexAttribute::Position ).c_str();
            attribute->m_index = ( ( *shader )[ attribute->m_pAttributeName ] );
            attribute->m_size = 3;
            attribute->m_type = GL_FLOAT;
            attribute->m_ptr = 0;
            attributes.add( attribute );

            attribute = new OGLVertexAttribute;
            attribute->m_pAttributeName = getVertCompName( VertexAttribute::Normal ).c_str();
            attribute->m_index = ( *shader )[ attribute->m_pAttributeName ];
            attribute->m_size = 3;
            attribute->m_type = GL_FLOAT;
            attribute->m_ptr = ( const GLvoid* ) offsetof( RenderVert, normal );
            attributes.add( attribute );

            attribute = new OGLVertexAttribute;
            attribute->m_pAttributeName = getVertCompName( VertexAttribute::Color0 ).c_str();
            attribute->m_index = ( *shader )[ attribute->m_pAttributeName ];
            attribute->m_size = 3;
            attribute->m_type = GL_FLOAT;
            attribute->m_ptr = ( const GLvoid* ) offsetof( RenderVert, color0 );
            attributes.add( attribute );

            attribute = new OGLVertexAttribute;
            attribute->m_pAttributeName = getVertCompName( VertexAttribute::TexCoord0 ).c_str();
            attribute->m_index = ( *shader )[ attribute->m_pAttributeName ];
            attribute->m_size = 2;
            attribute->m_type = GL_FLOAT;
            attribute->m_ptr = ( const GLvoid* ) offsetof( RenderVert, tex0 );
            attributes.add( attribute );
            break;

        default:
            break;
    }

    return true;
}

void OGLRenderBackend::releaseVertexCompArray( TArray<OGLVertexAttribute*> &attributes ) {
    if( !attributes.isEmpty() ) {
        for ( ui32 i = 0; i < attributes.size(); ++i ) {
            if ( nullptr != attributes[ i ] ) {
                delete attributes[ i ];
            }
        }
        attributes.clear();
    }
}

OGLVertexArray *OGLRenderBackend::createVertexArray() {
    OGLVertexArray *vertexArray = new OGLVertexArray;
    glGenVertexArrays( 1, &vertexArray->m_id );
    const ui32 id = m_vertexarrays.size();
    m_vertexarrays.add( vertexArray );
    vertexArray->m_slot = id;

    return vertexArray;
}

bool OGLRenderBackend::bindVertexLayout( OGLVertexArray *va, OGLShader *shader, ui32 stride, GLint loc, 
                                         OGLVertexAttribute* attrib ) {
    if( nullptr == va || nullptr == shader || nullptr == attrib ) {
        return false;
    }

    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, attrib->m_size,
                           attrib->m_type,
                           GL_FALSE,
                           stride,
                           attrib->m_ptr 
                         );

    return true;
}

bool OGLRenderBackend::bindVertexLayout( OGLVertexArray *va, OGLShader *shader, ui32 stride, 
                                             const TArray<OGLVertexAttribute*> &attributes ) {
    if( nullptr == va || nullptr == shader ) {
        return false;
    }

    for( ui32 i = 0; i < attributes.size(); ++i ) {
        const c8 *attribName = attributes[ i ]->m_pAttributeName;
        OSRE_ASSERT( nullptr != attribName );
        const GLint loc = ( *shader )[ attribName ];
        if( -1 != loc ) {
            glEnableVertexAttribArray( loc );
            glVertexAttribPointer( loc, attributes[ i ]->m_size,
                                   attributes[ i ]->m_type,
                                   GL_FALSE,
                                   stride,
                                   attributes[ i ]->m_ptr 
                                 );
        } else {
            String msg = "Cannot find " + String( attribName );
            osre_debug( Tag, msg );
        }
    }

    return true;
}

void OGLRenderBackend::destroyVertexArray( OGLVertexArray *vertexArray ) {
    if ( nullptr == vertexArray ) {
        return;
    }

    glDeleteVertexArrays( 1, &vertexArray->m_id );
    vertexArray->m_id = NotInitedHandle;
}

OGLVertexArray *OGLRenderBackend::getVertexArraybyId( ui32 id ) const {
    OGLVertexArray *va( nullptr );
    if ( id < m_vertexarrays.size() ) {
        va = m_vertexarrays[ id ];
    }

    return va;
}

void OGLRenderBackend::bindVertexArray( OGLVertexArray *vertexArray ) {
	if ( nullptr == vertexArray ) {
        osre_debug( Tag, "Pointer to vertex array is nullptr" );
        return;
	}

	if ( ( m_activeVertexArray == OGLNotSetId ) || ( m_activeVertexArray != vertexArray->m_id) ){
		m_activeVertexArray = vertexArray->m_id;
		glBindVertexArray( m_activeVertexArray );
	}
    CHECKOGLERRORSTATE();
}

void OGLRenderBackend::unbindVertexArray() {
    glBindVertexArray( 0 );
    m_activeVertexArray = OGLNotSetId;
}

void OGLRenderBackend::releaseAllVertexArrays( ) {
    for ( ui32 i=0; i<m_vertexarrays.size(); ++i ) {
        destroyVertexArray( m_vertexarrays[ i ] );
    }
    m_vertexarrays.clear();
}

OGLShader *OGLRenderBackend::createShader( const String &name, Shader *shaderInfo ) {
	if( name.empty() ) {
        osre_debug( Tag, "Name for shader is nullptr" );
        return nullptr;
    }

    OGLShader *oglShader = getShader( name );
    if ( nullptr != oglShader ) {
        return oglShader;
    }
    
    oglShader = new OGLShader( name );
    m_shaders.add( oglShader );
    if( shaderInfo ) {
        bool result( false );
        if( !shaderInfo->m_src[ static_cast<int>( ShaderType::SH_VertexShaderType ) ].empty() ) {
            result = oglShader->loadFromSource( ShaderType::SH_VertexShaderType, shaderInfo->m_src[ static_cast<int>( ShaderType::SH_VertexShaderType ) ] );
            if( !result ) {
                osre_error( Tag, "Error while compiling VertexShader." );
            }
        }

        if( !shaderInfo->m_src[ static_cast<int>( ShaderType::SH_FragmentShaderType ) ].empty( ) ) {
            result = oglShader->loadFromSource( ShaderType::SH_FragmentShaderType, shaderInfo->m_src[ static_cast<int>( ShaderType::SH_FragmentShaderType ) ] );
            if( !result ) {
                osre_error( Tag, "Error while compiling FragmentShader." );
            }
        }

        if( !shaderInfo->m_src[ static_cast<int>( ShaderType::SH_GeometryShaderType ) ].empty( ) ) {
            result = oglShader->loadFromSource( ShaderType::SH_GeometryShaderType, shaderInfo->m_src[ static_cast<int>( ShaderType::SH_GeometryShaderType ) ] );
            if( !result ) {
                osre_error( Tag, "Error while compiling GeometryShader." );
            }
        }

        result = oglShader->createAndLink();
        if( !result ) {
            osre_error( Tag, "Error while linking shader" );
        }
    }

    return oglShader;
}

OGLShader *OGLRenderBackend::getShader( const String &name ) {	
	if ( name.empty() ) {
        return nullptr;
    }

    OGLShader *shader( nullptr );
    for( ui32 i = 0; i < m_shaders.size(); ++i ) {
        if( m_shaders[ i ]->getName() == name ) {
            shader = m_shaders[ i ];
            break;
        }
    }

    return shader;
}

bool OGLRenderBackend::useShader( OGLShader *shader ) {
    // shader already in use
    if ( m_shaderInUse == shader ) {
        return true;
    }

    // unuse an older shader 
    if ( nullptr != m_shaderInUse ) {
        m_shaderInUse->unuse();
    }

    // 
    m_shaderInUse = shader;
    if ( nullptr != m_shaderInUse ) {
        m_shaderInUse->use();
    }

    return true;
}

OGLShader *OGLRenderBackend::getActiveShader() const {
    return m_shaderInUse;
}

bool OGLRenderBackend::releaseShader( OGLShader *shader ) {
	if( nullptr != shader ) {
        return false;
    }

    // look for the shader
    ui32 idx( 0 );
    bool found( false  );
    for( ui32 i = 0; i < m_shaders.size(); ++i ) {
        if( m_shaders[ i ] == shader ) {
            found = true;
            idx = i;
            break;
        }
    }
    
    // remove shader from list
    if( found ) {
        delete m_shaders[ idx ];
        m_shaders.remove( idx );
    }

    return found;
}

void OGLRenderBackend::releaseAllShaders( ) {
    for( ui32 i = 0; i < m_shaders.size(); ++i ) {
        if( nullptr != m_shaders[ i ] ) {
            if ( m_shaderInUse == m_shaders[ i ] ) {
                useShader( nullptr );
            }
            delete m_shaders[ i ];
        }
    }
    m_shaders.clear();
}

OGLTexture *OGLRenderBackend::createEmptyTexture( const String &name, TextureTargetType target,
                                                  ui32 width, ui32 height, ui32 channels ) {
	if( name.empty() ) {
        osre_debug( Tag, "Texture anme is empty." );
        return nullptr;
    }

    // lookup for texture
    OGLTexture *tex( findTexture( name ) );
    if( tex ) {
        return tex;
    }

    // get texture slot
    ui32 slot( 0 );
    if( m_freeTexSlots.isEmpty() ) {
        slot = m_textures.size();
        tex = new OGLTexture;
        m_textures.add( tex );
    } else {
        slot = m_freeTexSlots.back();
        m_freeTexSlots.removeBack();
        tex = m_textures[ slot ];
    }
    tex->m_slot = slot;
    m_texLookupMap[ name ] = slot;

    GLuint textureId;
    glGenTextures( 1, &textureId );
    tex->m_textureId = textureId;
    tex->m_name      = name;
    tex->m_width     = static_cast< ui32 >( width );
    tex->m_height    = static_cast< ui32 >( height );
    tex->m_channels  = static_cast< ui32 >( channels );
    tex->m_format    = GL_RGB;

    glActiveTexture( GL_TEXTURE0 );
    tex->m_target = OGLEnum::getGLTextureTarget( TextureTargetType::Texture2D );
    glBindTexture( tex->m_target, textureId );
    
    glTexParameteri( tex->m_target, OGLEnum::getGLTextureParameterName( TextureParameterName::TextureParamMinFilter ), GL_LINEAR );
    glTexParameteri( tex->m_target, OGLEnum::getGLTextureParameterName( TextureParameterName::TextureParamMagFilter ), GL_LINEAR );
    glTexParameteri( tex->m_target, OGLEnum::getGLTextureParameterName( TextureParameterName::TextureParamWrapS ), GL_CLAMP );
    glTexParameteri( tex->m_target, OGLEnum::getGLTextureParameterName( TextureParameterName::TextureParamWrapT ), GL_CLAMP );

    return tex;
}

void OGLRenderBackend::updateTexture( OGLTexture *oglTextue, ui32 offsetX, ui32 offsetY, c8 *data,
                                      ui32 size ) {
	if( !oglTextue ) {
        osre_error( Tag, "Pointer to texture is a nullptr." );
        return;
    }

    const ui32 diffX( oglTextue->m_width - offsetX );
    const ui32 diffY( oglTextue->m_height - offsetY );
    const ui32 subSize( diffX * diffY * oglTextue->m_channels );
	OSRE_ASSERT( size < subSize );
    glTexSubImage2D( oglTextue->m_target, 0, offsetX, offsetY, oglTextue->m_width,
                     oglTextue->m_height, oglTextue->m_format, GL_UNSIGNED_BYTE, data );
}

OGLTexture *OGLRenderBackend::createTextureFromFile( const String &name, const IO::Uri &fileloc ) {
    OGLTexture *tex( findTexture( name ) );
    if( tex ) {
        return tex;
    }

    // import the texture
    const String filename = fileloc.getAbsPath();
    i32 width( 0 ), height( 0 ), channels( 0 );
    GLubyte *data = SOIL_load_image( filename.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO );
    if( !data ) {
        osre_debug( Tag, "Cannot load texture " + filename );
        return nullptr;
    }

    // swap the texture data
    // todo: must be done by my codec
    for( i32 j = 0; j * 2 < height; ++j ) {
        i32 index1 = j * width * channels;
        i32 index2 = ( height - 1 - j ) * width * channels;
        for( i32 i = width*channels; i > 0; --i ) {
            GLubyte temp = data[ index1 ];
            data[index1] = data[ index2 ];
            data[ index2 ] = temp;
            ++index1;
            ++index2;
        }
    }

    // create texture and fill it
    tex = createEmptyTexture( name, TextureTargetType::Texture2D, width, height, channels );
    glTexImage2D( tex->m_target, 0, GL_RGB, width, height, 0, tex->m_format, GL_UNSIGNED_BYTE, data );
    glBindTexture( tex->m_target, 0 );

    SOIL_free_image_data( data );

    return tex;
}

OGLTexture *OGLRenderBackend::createTextureFromStream( const String &name, IO::Stream &stream, 
                                                       ui32 width, ui32 height, ui32 channels ) {
    OGLTexture *tex( findTexture( name ) );
    if( tex ) {
        return tex;
    }

    const ui32 size = stream.getSize();
    uc8 *data = new uc8[ size ];
    stream.read( data, size );

    // create texture and fill it
    tex = createEmptyTexture( name, TextureTargetType::Texture2D, width, height, channels );
    glTexImage2D( tex->m_target, 0, GL_RGB, width, height, 0, tex->m_format, GL_UNSIGNED_BYTE, data );
    delete [] data;

    return tex;
}

OGLTexture *OGLRenderBackend::findTexture( const String &name ) const {
	if( name.empty() ) {
        return nullptr;
    }

    std::map<String, ui32>::const_iterator it( m_texLookupMap.find( name ) );
    if( it == m_texLookupMap.end() ) {
        return nullptr;
    }

    return m_textures[ it->second ];
}

bool OGLRenderBackend::bindTexture( OGLTexture *oglTexture, TextureStageType stageType ) {
	if( !oglTexture ) {
        return false;
    }

    GLenum glStageType = OGLEnum::getGLTextureStage( stageType );
    glActiveTexture( glStageType );
    glBindTexture( oglTexture->m_target, oglTexture->m_textureId );

    return true;
}

void OGLRenderBackend::releaseTexture( OGLTexture *oglTexture ) {
    if( m_textures[ oglTexture->m_slot ] ) {
        glDeleteTextures( 1, &oglTexture->m_textureId );
        oglTexture->m_textureId = OGLNotSetId;
        oglTexture->m_width     = 0;
        oglTexture->m_height    = 0;
        oglTexture->m_channels  = 0;

        m_freeTexSlots.add( oglTexture->m_slot );

        std::map<String, ui32>::iterator it( m_texLookupMap.find( oglTexture->m_name ) );
        if( m_texLookupMap.end() != it ) {
            it = m_texLookupMap.erase( it );
        }
        oglTexture->m_slot = 0;
    }
}

void OGLRenderBackend::releaseAllTextures( ) {
    for( ui32 i = 0; i < m_textures.size(); ++i ) {
        if( m_textures[ i ]->m_textureId != OGLNotSetId ) {
            releaseTexture( m_textures[ i ] );
        }
    }
    m_freeTexSlots.clear();
    m_textures.clear();
    m_texLookupMap.clear();
}

OGLParameter *OGLRenderBackend::createParameter( const String &name, ParameterType type,  UniformDataBlob *blob, ui32 numItems ) {    
    // Check if the parameter is already there
    OGLParameter *param = getParameter( name );
    if ( nullptr != param ) {
        return param;
    }

    // We need to create it 
    param               = new OGLParameter;
    param->m_name       = name;
    param->m_type       = type;
    param->m_loc        = NoneLocation;
    param->m_numItems   = numItems;
    param->m_data       = UniformDataBlob::create( type, param->m_numItems );
    if( nullptr != blob ) {
        if ( 0 != blob->m_size ) {
            ::memcpy( param->m_data->getData(), blob->getData(), blob->m_size );
        }
    }
    m_parameters.add( param );

    return param;
}

OGLParameter *OGLRenderBackend::getParameter( const String &name ) const {
    if( name.empty() ) {
        return nullptr;
    }

    for( ui32 i = 0; i < m_parameters.size(); ++i ) {
        if( m_parameters[ i ]->m_name == name ) {
            return m_parameters[ i ];
        }
    }

    return nullptr;
}

void OGLRenderBackend::setParameter( OGLParameter *param ) {
    if ( nullptr == param ) {
        osre_debug( Tag, "Cannot set parameter, invalid param pointer." );
        return;
    }

    if( nullptr == m_shaderInUse ) {
        osre_debug( Tag, "Cannot set parameter, no shader in use." );
        return;
    }

    OGLShader *shader( m_shaderInUse );
    if ( NoneLocation == param->m_loc ) {
        param->m_loc = ( *shader )( param->m_name );
        if ( NoneLocation == param->m_loc ) {
            osre_debug( Tag, "Cannot location for parameter " 
                + param->m_name 
                + " in shader " 
                + shader->getName() 
                + "." );
            return;
        }
    }

    switch ( param->m_type ) {
        case ParameterType::PT_Int:
        {
            GLint data;
            ::memcpy( &data, param->m_data->getData(), sizeof( GLint ) );
            glUniform1i( param->m_loc, data );
        }
        break;
    
        case ParameterType::PT_Float:
        {
            GLfloat value;
            ::memcpy( &value, param->m_data->getData(), sizeof( GLfloat ) );
            glUniform1f( param->m_loc, value );
        }
        break;

        case ParameterType::PT_Float2:
        {
            GLfloat value[ 2 ];
            ::memcpy( &value[ 0 ], param->m_data->getData(), sizeof( GLfloat ) * 2 );
            glUniform2f( param->m_loc, value[ 0 ], value[ 1 ] );
        }
        break;

        case ParameterType::PT_Float3:
        {
            GLfloat value[ 3 ];
            ::memcpy( &value[ 0 ], param->m_data->getData(), sizeof( GLfloat ) * 3 );
            glUniform3f( param->m_loc, value[ 0 ], value[ 1 ], value[ 2 ] );
        }
        break;
    
        case ParameterType::PT_Mat4: {
            glm::mat4 mat;
            ::memcpy( &mat, param->m_data->getData(), sizeof( glm::mat4 ) );
            glUniformMatrix4fv( param->m_loc, 1, GL_FALSE, glm::value_ptr( mat ) );
        }
        break;

        case ParameterType::PT_Mat4Array:
        {
            glUniformMatrix4fv( param->m_loc, param->m_numItems, GL_FALSE, ( f32* ) param->m_data->getData() );
        }
        break;

        default:
            break;
    }
    CHECKOGLERRORSTATE();
}

void OGLRenderBackend::releaseAllParameters() {
    ContainerClear( m_parameters );
}

void OGLRenderBackend::setParameter( OGLParameter **param, ui32 numParam ) {
    if( nullptr == m_shaderInUse ) {
        return;
    }

    for( ui32 i = 0; i < numParam; ++i ) {
        OGLParameter *currentParam = param[ i ];
        if( nullptr != currentParam ) {
            setParameter( currentParam );
        }
    }
}

ui32 OGLRenderBackend::addPrimitiveGroup( PrimitiveGroup *grp ) {
    if ( nullptr == grp ) {
        osre_error( Tag, "Group pointer is nullptr" );
        return NotInitedHandle;
    }

    OGLPrimGroup *oglGrp    = new OGLPrimGroup;
    oglGrp->m_primitive     = OGLEnum::getGLPrimitiveType( grp->m_primitive );
    oglGrp->m_indexType     = OGLEnum::getGLIndexType( grp->m_indexType );
    oglGrp->m_startIndex    = grp->m_startIndex;
    oglGrp->m_numPrimitives = grp->m_numPrimitives;
    
    const ui32 idx( m_primitives.size() );
    m_primitives.add( oglGrp );

    return idx;
}

void OGLRenderBackend::releaseAllPrimitiveGroups() {
    ContainerClear( m_primitives );
}

void OGLRenderBackend::render( ui32 primpGrpIdx ) {
    OGLPrimGroup *grp( m_primitives[ primpGrpIdx ] );
    if( nullptr != grp ) {
        glDrawElements( grp->m_primitive, 
                        grp->m_numPrimitives, 
                        grp->m_indexType, 
                        ( const GLvoid* ) grp->m_startIndex );
    }
}

void OGLRenderBackend::render( ui32 primpGrpIdx, ui32 numInstances ) {
    OGLPrimGroup *grp( m_primitives[ primpGrpIdx ] );
    if ( nullptr != grp ) {
        glDrawArraysInstanced( grp->m_primitive, 
                               grp->m_startIndex, 
                               grp->m_numPrimitives, 
                               numInstances );
    }
}

void OGLRenderBackend::renderFrame() {
    OSRE_ASSERT( nullptr != m_renderCtx );    
    
	m_renderCtx->update();
    if ( nullptr != m_fpsCounter ) {
        const ui32 fps = m_fpsCounter->getFPS();
        Profiling::PerformanceCounters::setCounter( "fps", fps );
    }
}

FontBase *OGLRenderBackend::createFont( const IO::Uri &font ) {
    FontBase *fontInst = new FontBase( font.getResource() );
    fontInst->setUri( font );
    if ( fontInst->loadFromStream( this ) ) {
        m_fonts.add( fontInst );
        m_activeFont = fontInst;
    }
    
    return fontInst;
}

void OGLRenderBackend::selectFont( FontBase *font ) {
    if ( nullptr == font ) {
        m_activeFont = nullptr;
        return;
    }

    if ( nullptr != findFont( font->getTextureName() ) ) {
        m_activeFont = font;
    } else {
        osre_debug( Tag, "Cannot set font " + font->getName() + "." );
    }
}

FontBase *OGLRenderBackend::findFont( const String &name ) const {
    FontBase *font( nullptr );
    for (ui32 i = 0; i < m_fonts.size(); i++) {
        if ( m_fonts[ i ]->getTextureName() == name ) {
            font = m_fonts[ i ];
            break;
        }
    }

    return font;
}

bool OGLRenderBackend::relaseFont( FontBase *font ) {
    if (nullptr == font) {
        return false;
    }

    bool ok( false );
    for ( ui32 i = 0; i < m_fonts.size(); i++) {
        if (m_fonts[ i ] == font) {
            m_fonts.remove( i );
            font->release();
            ok = true;
            break;
        }
    }

    return ok;
}

void OGLRenderBackend::releaseAllFonts() {
    for ( ui32 i = 0; i < m_fonts.size(); i++ ) {
        if ( nullptr != m_fonts[ i ] ) {
            m_fonts[ i ]->release();
        }
    }
    m_fonts.clear();
}

ui32 OGLRenderBackend::getVertexSize( VertexType vertextype ) {
    ui32 vertexSize( 0 );
    switch( vertextype ) {
        case VertexType::ColorVertex:
            vertexSize = sizeof( ColorVert );
            break;

        case VertexType::RenderVertex:
            vertexSize = sizeof( RenderVert );
            break;

        default:
            break;
    }

    return vertexSize;
}

} // Namespace RenderBackend
} // Namespace OSRE
