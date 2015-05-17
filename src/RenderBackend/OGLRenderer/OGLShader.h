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
} // Namespace OSRE
