/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "OGLEnum.h"
#include <osre/Common/Logger.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/Stream.h>

namespace OSRE {
namespace RenderBackend {

static constexpr c8 Tag[] = "OGLShader";

OGLShader::OGLShader(const String &name) :
        Object(name),
        m_attribParams(),
        m_uniformParams(),
        m_shaderprog(0),
        m_numShader(0),
        m_attributeMap(),
        m_uniformLocationMap(),
        m_isCompiledAndLinked(false),
        m_isInUse(false) {
    ::memset(m_shaders, 0, sizeof(unsigned int) * 3);
}

OGLShader::~OGLShader() {
    if (m_isInUse) {
        osre_warn(Tag, "Destroying shader which is still in use.");
    }

    cppcore::ContainerClear(m_attribParams);
    cppcore::ContainerClear(m_uniformParams);
    for (ui32 i = 0; i < static_cast<ui32>(ShaderType::NumShaderTypes); ++i) {
        if (0 != m_shaders[i]) {
            glDeleteShader(m_shaders[i]);
            m_shaders[i] = 0;
        }
    }

    if (0 != m_shaderprog) {
        glDeleteProgram(m_shaderprog);
        m_shaderprog = 0;
    }
}

bool OGLShader::loadFromSource(ShaderType type, const String &src) {
    if (src.empty()) {
        return false;
    }
    GLuint shader = glCreateShader(OGLEnum::getOGLShaderType(type));
    m_shaders[static_cast<int>(type)] = shader;

    const char *tmp = src.c_str();
    glShaderSource(shader, 1, &tmp, nullptr);

    return true;
}

bool OGLShader::loadFromStream(ShaderType type, IO::Stream &stream) {
    if (!stream.isOpen()) {
        return false;
    }

    const size_t filesize = stream.getSize();
    if (0 == filesize) {
        return true;
    }

    c8 *data = new c8[filesize];
    stream.read(data, filesize);

    const bool retCode = loadFromSource(type, String(data));
    delete[] data;

    return retCode;
}

bool OGLShader::createAndLink() {
    if (isCompiled()) {
        osre_info(Tag, "Trying to compile shader program, which was compiled before.");
        return true;
    }

    m_shaderprog = glCreateProgram();
    if (0 == m_shaderprog) {
        osre_error(Tag, "Error while creating shader program.");
        return false;
    }
    if (0 != m_shaders[static_cast<i32>(ShaderType::SH_VertexShaderType)]) {
        glAttachShader(m_shaderprog, m_shaders[static_cast<i32>(ShaderType::SH_VertexShaderType)]);
    }
    if (0 != m_shaders[static_cast<i32>(ShaderType::SH_FragmentShaderType)]) {
        glAttachShader(m_shaderprog, m_shaders[static_cast<i32>(ShaderType::SH_FragmentShaderType)]);
    }
    if (0 != m_shaders[static_cast<i32>(ShaderType::SH_GeometryShaderType)]) {
        glAttachShader(m_shaderprog, m_shaders[static_cast<i32>(ShaderType::SH_GeometryShaderType)]);
    }

    GLint status(0);
    glLinkProgram(m_shaderprog);
    glGetProgramiv(m_shaderprog, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        logCompileOrLinkError(m_shaderprog);
        m_isCompiledAndLinked = false;
        return false;
    } 
    
    getActiveAttributeList();
    getActiveUniformList();    
    m_isCompiledAndLinked = true;

    return m_isCompiledAndLinked;
}

void OGLShader::use() {
    m_isInUse = true;
    glUseProgram(m_shaderprog);
}

void OGLShader::unuse() {
    m_isInUse = false;
    glUseProgram(0);
}

bool OGLShader::hasAttribute(const String &attribute) {
    if (0 == m_shaderprog) {
        return false;
    }

    const GLint location = glGetAttribLocation(m_shaderprog, attribute.c_str());
    return InvalidLocationId != location;
}

void OGLShader::addAttribute(const String &attribute) {
    const GLint location = glGetAttribLocation(m_shaderprog, attribute.c_str());
    m_attributeMap[attribute] = location;
    if (InvalidLocationId == location) {
        osre_debug(Tag, "Cannot find attribute " + attribute + " in shader.");
    }
}

bool OGLShader::hasUniform(const String &uniform) {
    if (0 == m_shaderprog) {
        return false;
    }
    const GLint location = glGetUniformLocation(m_shaderprog, uniform.c_str());
    return InvalidLocationId != location;
}

void OGLShader::addUniform(const String &uniform) {
    const GLint location = glGetUniformLocation(m_shaderprog, uniform.c_str());
    m_uniformLocationMap[uniform] = location;
    if (InvalidLocationId == location) {
        osre_debug(Tag, "Cannot find uniform variable " + uniform + " in shader.");
    }
}

static i32 getActiveParam(ui32 progId, GLenum type) {
    if (0 == progId) {
        return InvalidLocationId;
    }

    i32 params(InvalidLocationId);
    glGetProgramiv(progId, type, &params);

    return params;
}

void OGLShader::getActiveAttributeList() {
    const i32 numAtttibs(getActiveParam(m_shaderprog, GL_ACTIVE_ATTRIBUTES));
    if (numAtttibs < 1) {
        return;
    }

    for (i32 i = 0; i < numAtttibs; i++) {
        GLint actual_length(0), size(0);
        GLenum type;
        c8 name[MaxLen];
        glGetActiveAttrib(m_shaderprog, i, MaxLen, &actual_length, &size, &type, name);
        if (size > 1) {
            for (i32 attribIdx = 0; attribIdx < size; attribIdx++) {
                ActiveParameter *attribParam = new ActiveParameter;
                std::stringstream stream;
                stream << name << attribIdx;
                strncpy(attribParam->m_name, stream.str().c_str(), stream.str().size());
                attribParam->m_location = glGetAttribLocation(m_shaderprog, attribParam->m_name);
                m_attribParams.add(attribParam);
            }
        } else {
            ActiveParameter *attribParam = new ActiveParameter;
            strncpy(attribParam->m_name, name, strlen(name));
            attribParam->m_location = glGetAttribLocation(m_shaderprog, attribParam->m_name);
            m_attribParams.add(attribParam);
        }
    }
}

void OGLShader::getActiveUniformList() {
    const i32 numUniforms(getActiveParam(m_shaderprog, GL_ACTIVE_UNIFORMS));
    if (numUniforms < 1) {
        return;
    }

    for (i32 i = 0; i < numUniforms; i++) {
        GLint actual_length(0), size(0);
        GLenum type;
        c8 name[MaxLen];
        ::memset(name, '\0', sizeof(c8) * MaxLen);
        glGetActiveUniform(m_shaderprog, i, MaxLen, &actual_length, &size, &type, name);
        ActiveParameter *attribParam = new ActiveParameter;
        strncpy(attribParam->m_name, name, strlen(name));
        attribParam->m_location = glGetUniformLocation(m_shaderprog, name);
        m_attribParams.add(attribParam);
    }
}

void OGLShader::logCompileOrLinkError(ui32 shaderprog) {
    GLint infoLogLength(0);
    glGetProgramiv(shaderprog, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar *infoLog = new GLchar[infoLogLength];
    ::memset(infoLog, 0, infoLogLength);
    glGetProgramInfoLog(shaderprog, infoLogLength, NULL, infoLog);
    String error(infoLog);
    Common::Logger::getInstance()->print("Link log:\n" + error + "\n");
    delete[] infoLog;
}

bool OGLShader::isCompiled() const {
    return m_isCompiledAndLinked;
}

GLint OGLShader::getAttributeLocation(const String &attribute) {
    const GLint loc = m_attributeMap[attribute];
    return loc;
}

GLint OGLShader::getUniformLocation(const String &uniform) {
    if (uniform.empty()) {
        return InvalidLocationId;
    }

    std::map<String, GLint>::iterator it = m_uniformLocationMap.find(uniform);
    if (m_uniformLocationMap.end() == it) {
        return InvalidLocationId;
    }
    const GLint loc = m_uniformLocationMap[uniform];
    return loc;
}

} // Namespace RenderBackend
} // Namespace OSRE
