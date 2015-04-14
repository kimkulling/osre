#pragma once

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
#ifndef ZFXCE2_RS_RENDERBACKEND_OGLRENDERER_OGLSHADER_H_INC
#define ZFXCE2_RS_RENDERBACKEND_OGLRENDERER_OGLSHADER_H_INC

#include <zfxce2/Types.h>
#include <zfxce2/Infrastructure/Core/Object.h>
#include <zfxce2/RenderSystem/RenderBackend/RenderCommon.h>

#include <GL/glew.h>

#include <string>
#include <map>

namespace ZFXCE2 {
namespace RenderBackend {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::RenderBackend::OGLShader
///	@ingroup	RenderSystem
///
///	@brief  This class is used to implement a shader object for OpenGL. It combines the several
/// shader stages like vertex-, fragment or geometry-shader.
/// You can load a shader for a given type from a file or from in-memory string buffer.
//-------------------------------------------------------------------------------------------------
class DLL_EXPORT OGLShader : public Core::Object {
public:
    OGLShader( const ce_string &name );
    ~OGLShader();
    bool loadFromSource( ShaderType type, const ce_string &src );
    bool loadFromFile( ShaderType type, const ce_string &file );
    bool createAndLink();
    void use();
    void unuse();
    void addAttribute( const ce_string& attribute );
    void addUniform( const ce_string& uniform );
    GLint operator[] ( const ce_string& attribute );
    GLint operator() ( const ce_string& uniform );

private:
    OGLShader( const OGLShader & );
    OGLShader &operator = ( const OGLShader & );

private:
    ui32 m_shaderprog;
    ui32 m_numShader;
    ui32 m_shaders[ MaxShaderTypes ];
    std::map<ce_string, GLint> m_attributeList;
    std::map<ce_string, GLint> m_uniformLocationList;
};

//-------------------------------------------------------------------------------------------------

} // Namespace RenderBackend
} // Namespace ZFXCE2

#endif // ZFXCE2_RS_RENDERBACKEND_OGLRENDERER_OGLSHADER_H_INC
