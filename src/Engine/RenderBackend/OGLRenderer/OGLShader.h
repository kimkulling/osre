/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#pragma once

#include <osre/Common/osre_common.h>
#include <osre/Common/Object.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <GL/glew.h>
#include <string>
#include <map>

namespace OSRE {

// Forward declarations
namespace IO {
    class Stream;
}

namespace RenderBackend {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class is used to implement a shader object for OpenGL. It combines the several
/// shader stages like vertex-, fragment or geometry-shader.
/// You can load a shader for a given type from a file or from in-memory string buffer.
//-------------------------------------------------------------------------------------------------
class OGLShader : public Common::Object {
public:
    /// @brief  The class constructor.
    /// @param  name    [in9 The name for the shader.
    OGLShader( const String &name );

    /// @brief  The class destructor.
    virtual ~OGLShader() override;
    
    /// @brief  Will load the shader type from a given string.
    /// @param  type    [in] The shader type.
    /// @param  src     [in] The shader source to compile.
    /// @return true, if compile was successful, false in case of an error.
    bool loadFromSource( ShaderType type, const String &src );
    
    /// @brief
    /// @param  type    [in] The shader type.
    /// @param  file    [in] The file containing the source.
    /// @return true, if compile was successful, false in case of an error.
    bool loadFromFile(ShaderType type, IO::Stream &stream );

    /// @brief  Will create and link a shader program.
    /// @return true, if create & link was successful, false in case of an error.
    bool createAndLink();
    
    /// @brief  Will bind this program to the current render context.
    void use();

    /// @brief  Will unbind this program to the current render context.
    void unuse();
    
    /// @brief  Adds a new attribute to the shader.
    /// @param  attribute   [in] The name of the attribute.
    void addAttribute( const String& attribute );
    
    /// @brief  Adds a new uniform to the shader.
    /// @param  attribute   [in] The name of the uniform.
    void addUniform( const String& uniform );
    
    /// @brief  returns the location of the attribute.
    /// @param  attribute   [in] The name of the attribute.
    /// @return Its location or -1 for an error.
    GLint operator[] (const String& attribute);
    
    /// @brief  returns the location of the uniform.
    /// @param  attribute   [in] The name of the uniform.
    /// @return Its location or -1 for an error.
    GLint operator() (const String& uniform);

private:
    OGLShader( const OGLShader & ) = delete;
    OGLShader &operator = ( const OGLShader & ) = delete;

private:
    ui32 m_shaderprog;
    ui32 m_numShader;
    ui32 m_shaders[ MaxShaderTypes ];
    std::map<String, GLint> m_attributeList;
    std::map<String, GLint> m_uniformLocationList;
};

} // Namespace RenderBackend
} // Namespace OSRE
