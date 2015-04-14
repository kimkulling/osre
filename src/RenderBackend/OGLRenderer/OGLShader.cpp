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
#include "OGLShader.h"

#include <zfxce2/Infrastructure/Core/Logger.h>

namespace ZFXCE2 {
namespace RenderBackend {

static const GLint ErrorId = -1;

//-------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------
OGLShader::OGLShader( const ce_string &name )
: Object( name )
, m_shaderprog( 0 )
, m_numShader( 0 )
, m_attributeList()
, m_uniformLocationList() {
    ::memset( m_shaders, 0, sizeof( unsigned int ) * 3 );
}

//-------------------------------------------------------------------------------------------------
OGLShader::~OGLShader( ) {
    for ( unsigned int i=0; i<3; ++i ) {
        if( m_shaders[ i ] ) {
            glDeleteShader( m_shaders[ i ] );
            m_shaders[ i ] = 0;
        }
    }
}

//-------------------------------------------------------------------------------------------------
bool OGLShader::loadFromSource( ShaderType type, const ce_string &src ) {
    if ( src.empty() ) {
        return false;
    }
    GLuint shader = glCreateShader( getOGLShaderType( type ) );
    m_shaders[ type ] = shader;

    const char *tmp = src.c_str();
    glShaderSource( shader, 1, &tmp, nullptr );

    return true;
}

//-------------------------------------------------------------------------------------------------
bool OGLShader::loadFromFile( ShaderType type, const ce_string &file ) {
    if ( file.empty() ) {
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
bool OGLShader::createAndLink() {
    m_shaderprog = glCreateProgram();
    if ( 0 == m_shaderprog ) {
        ce_error( "Error while creating shader program." );
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
        ce_string error( infoLog );
        ce_debug( "Link log: " + error + "\n" );
        delete [] infoLog;
        result = false;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------
void OGLShader::use( ) {
    glUseProgram( m_shaderprog );
}

//-------------------------------------------------------------------------------------------------
void OGLShader::unuse( ) {
    glUseProgram( 0 );
}

//-------------------------------------------------------------------------------------------------
void OGLShader::addAttribute( const std::string& attribute ) {
    const GLint location = glGetAttribLocation( m_shaderprog, attribute.c_str( ) );
    m_attributeList[ attribute ] = location;
    if( ErrorId == location ) {
        ce_debug( "Cannot find attribute " + attribute + " in shader." );
    }
}

//-------------------------------------------------------------------------------------------------
void OGLShader::addUniform( const std::string& uniform ) {
    const GLint location = glGetUniformLocation( m_shaderprog, uniform.c_str( ) );
    m_uniformLocationList[ uniform ] = location;
    if( ErrorId == location ) {
        ce_debug( "Cannot find uniform variable " + uniform + " in shader." );
    }
}

//-------------------------------------------------------------------------------------------------
GLint OGLShader::operator[] ( const std::string &attribute ) {
    const GLint loc( m_attributeList[ attribute ] );
    return loc;
}

//-------------------------------------------------------------------------------------------------
GLint OGLShader::operator() ( const std::string &uniform ) {
    const GLint loc( m_uniformLocationList[ uniform ] );
    return loc;
}

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace ZFXCE2
