#pragma once

/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>
#include <osre/Common/Object.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <GL/glew.h>

#include <map>

namespace OSRE {

// Forward declarations
namespace IO {
    class Stream;
}

namespace RenderBackend {

static constexpr GLint InvalidLocationId = -1;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class is used to implement a shader object for OpenGL. It combines the several
/// shader stages like vertex-, fragment or geometry-shader.
/// You can load a shader for a given type from a file or from in-memory string buffer.
//-------------------------------------------------------------------------------------------------
class OGLShader : public Common::Object {
public:
    static constexpr ui32 MaxLen = 64u;

    struct ActiveParameter {
        c8 m_name[MaxLen];
        GLint m_location;

        ActiveParameter() :
                m_name(), m_location(InvalidLocationId) {
            ::memset(m_name, '\0', sizeof(c8) * MaxLen);
        }
    };

    ///	@brief  Type for parameter shader arrays in OpenGL.
    using ParameterArray = ::cppcore::TArray<ActiveParameter *>;

    /// @brief  The class constructor.
    /// @param  name    [in9 The name for the shader.
    OGLShader( const String &name );

    /// @brief  The class destructor.
    virtual ~OGLShader();
    
    /// @brief  Will load the shader type from a given string.
    /// @param  type    [in] The shader type.
    /// @param  src     [in] The shader source to compile.
    /// @return true, if compile was successful, false in case of an error.
    bool loadFromSource( ShaderType type, const String &src );
    
    /// @brief	
    /// @param  type    [in] The shader type.
    /// @param  stream  [in] The stream  containing the source.
    /// @return true, if compile was successful, false in case of an error.
    bool loadFromStream( ShaderType type, IO::Stream &stream );

    /// @brief  Will create and link a shader program.
    /// @return true, if create & link was successful, false in case of an error.
    bool createAndLink();
    
    /// @brief  Will bind this program to the current render context.
    void use();

    /// @brief  Will unbind this program to the current render context.
    void unuse();
    
	///	@brief	Will perform a lookup if the attribute is used in the shader program. 
	///         The shader program must be compiled before.
	///	@param	attribute	[in] The name of the attribute to look for.
	///	@return	true, if the attribute is used in the shader program, false if not.
    bool hasAttribute( const String& attribute );

    /// @brief  Adds a new attribute to the shader.
    /// @param  attribute   [in] The name of the attribute.
    void addAttribute( const String& attribute );

	///	@brief	Will perform a lookup if the uniform is used in the shader program. 
	///         The shader program must be compiled before.
	///	@param	uniform   	[in] The name of the attribute to look for.
	///	@return	true, if the uniform is used in the shader program, false if not.
	bool hasUniform( const String& uniform );

    /// @brief  Adds a new uniform to the shader.
    /// @param  uniform     [in] The name of the uniform.
    void addUniform( const String& uniform );
    
    /// @brief  Will create a list with all active attributes.
    void getActiveAttributeList();

    /// @brief  Will create a list with all active uniforms.
    void getActiveUniformList();

    /// @brief  Logs a compile and link error.
    /// @param  shaderprog  [in] The shader program handle.
    static void logCompileOrLinkError( ui32 shaderprog );

    ///	@brief	Will return the current compile state.
	///	@return	true, if the shader is compiled with success, false if not.
	bool isCompiled() const;

    GLint getAttributeLocation(const String &attribute);
    GLint getUniformLocation(const String &uniform);

    // No copying
    OGLShader( const OGLShader & ) = delete;
    OGLShader &operator = ( const OGLShader & ) = delete;

private:
    ParameterArray m_attribParams;
    ParameterArray m_uniformParams;

    ui32 m_shaderprog;
    ui32 m_numShader;
    ui32 m_shaders[ MaxShaderTypes ];
    std::map<String, GLint> m_attributeMap;
    std::map<String, GLint> m_uniformLocationMap;
    bool m_isCompiledAndLinked;
	bool m_isInUse;
};

} // Namespace RenderBackend
} // Namespace OSRE
