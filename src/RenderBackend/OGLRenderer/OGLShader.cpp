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
#include "OGLShader.h"

#include <osre/Common/Logger.h>

namespace OSRE {
namespace RenderBackend {

static const GLint ErrorId = -1;
static const String Tag    = "OGLShader";

static GLuint getOGLShaderType( ShaderType type ) {
    switch( type ) {
        case SH_VertexShaderType:
            return GL_VERTEX_SHADER;
        case SH_FragmentShaderType:
            return GL_FRAGMENT_SHADER;
        case SH_GeometryShaderType:
            return GL_GEOMETRY_SHADER;
        default:
           break;
    }

    return 0;
}

OGLShader::OGLShader( const String &name )
: Object( name )
, m_shaderprog( 0 )
, m_numShader( 0 )
, m_attributeList()
, m_uniformLocationList() {
    ::memset( m_shaders, 0, sizeof( unsigned int ) * 3 );
}

OGLShader::~OGLShader( ) {
    for ( unsigned int i=0; i<3; ++i ) {
        if( m_shaders[ i ] ) {
            glDeleteShader( m_shaders[ i ] );
            m_shaders[ i ] = 0;
        }
    }
}

bool OGLShader::loadFromSource( ShaderType type, const String &src ) {
    if ( src.empty() ) {
        return false;
    }
    GLuint shader = glCreateShader( getOGLShaderType( type ) );
    m_shaders[ type ] = shader;

    const char *tmp = src.c_str();
    glShaderSource( shader, 1, &tmp, nullptr );

    return true;
}

bool OGLShader::loadFromFile( ShaderType type, const String &file ) {
    if ( file.empty() ) {
        return false;
    }

    return true;
}

bool OGLShader::createAndLink() {
    m_shaderprog = glCreateProgram();
    if ( 0 == m_shaderprog ) {
        osre_error( Tag, "Error while creating shader program." );
        return false;
    }
    if ( 0 != m_shaders[ SH_VertexShaderType ] ) {
        glAttachShader( m_shaderprog, m_shaders[ SH_VertexShaderType ] );
    }
    if ( 0 != m_shaders[ SH_FragmentShaderType ] ) {
        glAttachShader( m_shaderprog, m_shaders[ SH_FragmentShaderType ] );
    }
    if ( 0 != m_shaders[ SH_GeometryShaderType ] ) {
        glAttachShader( m_shaderprog, m_shaders[ SH_GeometryShaderType ] );
    }

    bool result( true );
    GLint status( 0 );
    glLinkProgram( m_shaderprog );
    glGetProgramiv( m_shaderprog, GL_LINK_STATUS, &status );
    if (status == GL_FALSE) {
        GLint infoLogLength( 0 );
        glGetProgramiv( m_shaderprog, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar *infoLog= new GLchar[infoLogLength];
        ::memset( infoLog, 0, infoLogLength );
        glGetProgramInfoLog( m_shaderprog, infoLogLength, NULL, infoLog);
        String error( infoLog );
        osre_debug( Tag, "Link log: " + error + "\n" );
        delete [] infoLog;
        result = false;
    }

    return result;
}

void OGLShader::use( ) {
    glUseProgram( m_shaderprog );
}

void OGLShader::unuse( ) {
    glUseProgram( 0 );
}

void OGLShader::addAttribute( const std::string& attribute ) {
    const GLint location = glGetAttribLocation( m_shaderprog, attribute.c_str( ) );
    m_attributeList[ attribute ] = location;
    if( ErrorId == location ) {
        osre_debug( Tag, "Cannot find attribute " + attribute + " in shader." );
    }
}

void OGLShader::addUniform( const std::string& uniform ) {
    const GLint location = glGetUniformLocation( m_shaderprog, uniform.c_str( ) );
    m_uniformLocationList[ uniform ] = location;
    if( ErrorId == location ) {
        osre_debug( Tag, "Cannot find uniform variable " + uniform + " in shader." );
    }
}

GLint OGLShader::operator[] ( const std::string &attribute ) {
    const GLint loc( m_attributeList[ attribute ] );
    return loc;
}

GLint OGLShader::operator() ( const std::string &uniform ) {
    const GLint loc( m_uniformLocationList[ uniform ] );
    return loc;
}

} // Namespace RenderBackend
} // Namespace OSRE
