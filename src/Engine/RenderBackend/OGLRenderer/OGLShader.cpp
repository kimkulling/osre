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
#include "OGLShader.h"

#include <osre/Common/Logger.h>
#include <osre/IO/Stream.h>
#include <osre/Debugging/osre_debugging.h>

namespace OSRE {
namespace RenderBackend {

static const GLint  ErrorId = -1;
static const String Tag    = "OGLShader";

static GLuint getOGLShaderType( ShaderType type ) {
    switch( type ) {
        case ShaderType::SH_VertexShaderType:
            return GL_VERTEX_SHADER;
        case ShaderType::SH_FragmentShaderType:
            return GL_FRAGMENT_SHADER;
        case ShaderType::SH_GeometryShaderType:
            return GL_GEOMETRY_SHADER;
        default:
            OSRE_VALIDATE( false, "Unknown enum.");
            break;
    }

    return 0;
}

OGLShader::OGLShader( const String &name )
: Object( name )
, m_shaderprog( 0 )
, m_numShader( 0 )
, m_attributeList()
, m_uniformLocationList()
, m_isCompiledAndLinked( false )
, m_isInUse( false ) {
    ::memset( m_shaders, 0, sizeof( unsigned int ) * 3 );
}

OGLShader::~OGLShader( ) {
	if ( m_isInUse ) {
		osre_warn(Tag, "Destroying shader which is still in use.");
	}

	for ( unsigned int i=0; i<3; ++i ) {
        if ( 0 != m_shaders[ i ] ) {
            glDeleteShader( m_shaders[ i ] );
            m_shaders[ i ] = 0;
        }
    }
	if ( 0 != m_shaderprog ) {
		glDeleteProgram( m_shaderprog );
		m_shaderprog = 0;
	}
}

bool OGLShader::loadFromSource( ShaderType type, const String &src ) {
    if ( src.empty() ) {
        return false;
    }
    GLuint shader = glCreateShader( getOGLShaderType( type ) );
    m_shaders[ static_cast<int>( type ) ] = shader;

    const char *tmp = src.c_str();
    glShaderSource( shader, 1, &tmp, nullptr );

    return true;
}

bool OGLShader::loadFromStream( ShaderType type, IO::Stream &stream ) {
    if ( !stream.isOpen() ) {
        return false;
    }

    const ui32 filesize( stream.getSize() );
    if ( 0 == filesize ) {
        return true;
    }

    c8 *data = new c8[ filesize ];
    stream.read( data, filesize);
    
    const bool retCode( loadFromSource( type, String( data ) ) );
    delete [] data;

    return retCode;
}

bool OGLShader::createAndLink() {
	if ( isCompiled() ) {
		osre_info(Tag, "Trying to compile shader program, which was compiled before.");
		return true;
	}
    
	m_shaderprog = glCreateProgram();
    if ( 0 == m_shaderprog ) {
        osre_error( Tag, "Error while creating shader program." );
        return false;
    }
    if ( 0 != m_shaders[ static_cast<i32>( ShaderType::SH_VertexShaderType ) ] ) {
        glAttachShader( m_shaderprog, m_shaders[ static_cast<i32>( ShaderType::SH_VertexShaderType  )] );
    }
    if ( 0 != m_shaders[ static_cast<i32>( ShaderType::SH_FragmentShaderType ) ] ) {
        glAttachShader( m_shaderprog, m_shaders[ static_cast<i32>( ShaderType::SH_FragmentShaderType ) ] );
    }
    if ( 0 != m_shaders[ static_cast<i32>( ShaderType::SH_GeometryShaderType  ) ] ) {
        glAttachShader( m_shaderprog, m_shaders[ static_cast<i32>( ShaderType::SH_GeometryShaderType ) ] );
    }

    bool result( true );
    GLint status( 0 );
    glLinkProgram( m_shaderprog );
    glGetProgramiv( m_shaderprog, GL_LINK_STATUS, &status );
    if (status == GL_FALSE) {
        logCompileOrLinkError(m_shaderprog);
        result = false;
    } 
    m_isCompiledAndLinked = result;

    return result;
}

void OGLShader::use( ) {
	m_isInUse = true;
    glUseProgram( m_shaderprog );
}

void OGLShader::unuse() {
	m_isInUse = false;
    glUseProgram( 0 );
}

bool OGLShader::hasAttribute( const String& attribute ) {
    if ( 0 == m_shaderprog ) {
        return false;
    }

    const GLint location = glGetAttribLocation( m_shaderprog, attribute.c_str() );
    return -1 != location;
}

void OGLShader::addAttribute( const String& attribute ) {
    const GLint location = glGetAttribLocation( m_shaderprog, attribute.c_str( ) );
    m_attributeList[ attribute ] = location;
    if( ErrorId == location ) {
        osre_debug( Tag, "Cannot find attribute " + attribute + " in shader." );
    }
}

bool OGLShader::hasUniform( const String& uniform ) {
    if ( 0 == m_shaderprog ) {
        return false;
    }
    const GLint location = glGetUniformLocation( m_shaderprog, uniform.c_str() );
    return -1 != location;
}

void OGLShader::addUniform( const String& uniform ) {
    const GLint location = glGetUniformLocation( m_shaderprog, uniform.c_str( ) );
    m_uniformLocationList[ uniform ] = location;
    if( ErrorId == location ) {
        osre_debug( Tag, "Cannot find uniform variable " + uniform + " in shader." );
    }
}

void OGLShader::logCompileOrLinkError( ui32 shaderprog ) {
    GLint infoLogLength( 0 );
    glGetProgramiv( shaderprog, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar *infoLog= new GLchar[infoLogLength];
    ::memset( infoLog, 0, infoLogLength );
    glGetProgramInfoLog( shaderprog, infoLogLength, NULL, infoLog);
    String error( infoLog );
    osre_debug( Tag, "Link log: " + error + "\n" );
    delete [] infoLog;
}

bool OGLShader::isCompiled() const {
    return m_isCompiledAndLinked;
}

GLint OGLShader::operator[] ( const String &attribute ) {
    const GLint loc( m_attributeList[ attribute ] );
    return loc;
}

GLint OGLShader::operator() ( const String &uniform ) {
	std::map<String, GLint>::iterator it( m_uniformLocationList.find( uniform ) );
	if (m_uniformLocationList.end() == it ) {
		osre_error(Tag, "Cannot find uniform " + uniform + ".");
	}
    const GLint loc( m_uniformLocationList[ uniform ] );
    return loc;
}

} // Namespace RenderBackend
} // Namespace OSRE
