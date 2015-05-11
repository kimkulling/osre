#pragma once

#include <osre/Common/Types.h>
#include <osre/Common/Object.h>
#include <osre/RenderBackend/RenderCommon.h>

#include <GL/glew.h>

#include <string>
#include <map>

namespace OSRE {
namespace RenderBackend {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderBackend::OGLShader
///	@ingroup	RenderSystem
///
///	@brief  This class is used to implement a shader object for OpenGL. It combines the several
/// shader stages like vertex-, fragment or geometry-shader.
/// You can load a shader for a given type from a file or from in-memory string buffer.
//-------------------------------------------------------------------------------------------------
class OGLShader : public Common::Object {
public:
    OGLShader( const String &name );
    ~OGLShader();
    bool loadFromSource( ShaderType type, const String &src );
    bool loadFromFile( ShaderType type, const String &file );
    bool createAndLink();
    void use();
    void unuse();
    void addAttribute( const String& attribute );
    void addUniform( const String& uniform );
    GLint operator[] ( const String& attribute );
    GLint operator() ( const String& uniform );

private:
    OGLShader( const OGLShader & );
    OGLShader &operator = ( const OGLShader & );

private:
    ui32 m_shaderprog;
    ui32 m_numShader;
    ui32 m_shaders[ MaxShaderTypes ];
    std::map<String, GLint> m_attributeList;
    std::map<String, GLint> m_uniformLocationList;
};

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace ZFXCE2
