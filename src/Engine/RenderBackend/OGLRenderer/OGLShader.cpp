/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Common/Logger.h"
#include "Debugging/osre_debugging.h"
#include "IO/Stream.h"

#include <iostream>

namespace OSRE::RenderBackend {

using namespace OSRE::Common;

DECL_OSRE_LOG_MODULE(OGLShader)

OGLShader::OGLShader(const String &name) :
        Object(name),
        mShaderprog(0),
        mNumShader(0),
        mIsCompiledAndLinked(false),
        mIsInUse(false) {
    memset(mShaders, 0, sizeof(ui32) * static_cast<size_t>(ShaderType::Count));
}

OGLShader::~OGLShader() {
    if (mIsInUse) {
        osre_warn(Tag, "Destroying shader which is still in use.");
        mIsInUse = false;
    }

    ContainerClear(mAttribParams);
    ContainerClear(mUniformParams);
    for (ui32 i = 0; i < static_cast<ui32>(ShaderType::Count); ++i) {
        if (0 != mShaders[i]) {
            glDeleteShader(mShaders[i]);
        }
    }

    if (0 != mShaderprog) {
        glDeleteProgram(mShaderprog);
    }
}

bool OGLShader::loadFromSource(ShaderType type, const String &src) {
    if (src.empty()) {
        osre_error(Tag, "Compilation failed: Shader source is empty.");
        return false;
    }

    GLuint shader = glCreateShader(OGLEnum::getOGLShaderType(type));
    mShaders[static_cast<int>(type)] = shader;
    const char *tmp = src.c_str();
    glShaderSource(shader, 1, &tmp, nullptr);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        osre_info(Tag, "Shadr source:\n" + src);
        char infoLog[512] = { '\0' };
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        osre_error(Tag, "Compilation failed: " + String(infoLog));
        return false;
    }

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

    MemoryBuffer buffer(filesize);
    stream.read(&buffer[0], filesize);
    const bool retCode = loadFromSource(type, String(&buffer[0]));

    return retCode;
}

bool OGLShader::createAndLink() {
    if (isCompiled()) {
        osre_warn(Tag, "Trying to compile shader program, which was compiled before.");
        return true;
    }

    mShaderprog = glCreateProgram();
    if (0 == mShaderprog) {
        osre_error(Tag, "Error while creating shader program.");
        return false;
    }
    if (0 != mShaders[static_cast<size_t>(ShaderType::SH_VertexShaderType)]) {
        glAttachShader(mShaderprog, mShaders[static_cast<i32>(ShaderType::SH_VertexShaderType)]);
    }
    if (0 != mShaders[static_cast<size_t>(ShaderType::SH_FragmentShaderType)]) {
        glAttachShader(mShaderprog, mShaders[static_cast<i32>(ShaderType::SH_FragmentShaderType)]);
    }
    if (0 != mShaders[static_cast<size_t>(ShaderType::SH_GeometryShaderType)]) {
        glAttachShader(mShaderprog, mShaders[static_cast<i32>(ShaderType::SH_GeometryShaderType)]);
    }

    GLint status(0);
    glLinkProgram(mShaderprog);
    glGetProgramiv(mShaderprog, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        logCompileOrLinkError(mShaderprog);
        mIsCompiledAndLinked = false;
        return false;
    }

    getActiveAttributeList();
    getActiveUniformList();
    mIsCompiledAndLinked = true;

    return mIsCompiledAndLinked;
}

void OGLShader::use() {
    if (mIsInUse) {
        return;
    }

    mIsInUse = true;
    glUseProgram(mShaderprog);
}

void OGLShader::unuse() {
    if (!mIsInUse) {
        return;
    }

    mIsInUse = false;
    glUseProgram(0);
}

bool OGLShader::hasAttribute(const String &attribute) {
    if (mShaderprog == 0) {
        return false;
    }

    const GLint location = glGetAttribLocation(mShaderprog, attribute.c_str());
    return InvalidLocationId != location;
}

void OGLShader::addAttribute(const String &attribute) {
    const GLint location = glGetAttribLocation(mShaderprog, attribute.c_str());
    mAttributeMap[attribute] = location;
    if (InvalidLocationId == location) {
        osre_debug(Tag, "Cannot find attribute " + attribute + " in shader.");
    }
}

bool OGLShader::hasUniform(const String &uniform) {
    if (0 == mShaderprog) {
        return false;
    }
    const GLint location = glGetUniformLocation(mShaderprog, uniform.c_str());
    return InvalidLocationId != location;
}

void OGLShader::addUniform(const String &uniform) {
    const GLint location = glGetUniformLocation(mShaderprog, uniform.c_str());
    mUniformLocationMap[uniform] = location;
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
    const i32 numAtttibs(getActiveParam(mShaderprog, GL_ACTIVE_ATTRIBUTES));
    if (numAtttibs < 1) {
        return;
    }

    for (i32 i = 0; i < numAtttibs; i++) {
        GLint actual_length(0), size(0);
        GLenum type;
        c8 name[MaxLen];
        glGetActiveAttrib(mShaderprog, i, MaxLen, &actual_length, &size, &type, name);
        if (size > 1) {
            for (i32 attribIdx = 0; attribIdx < size; attribIdx++) {
                ActiveParameter *attribParam = new ActiveParameter;
                std::stringstream stream;
                stream << name << attribIdx;
                strncpy(attribParam->name, stream.str().c_str(), stream.str().size());
                attribParam->location = glGetAttribLocation(mShaderprog, attribParam->name);
                mAttribParams.add(attribParam);
            }
        } else {
            ActiveParameter *attribParam = new ActiveParameter;
            strncpy(attribParam->name, name, strlen(name));
            attribParam->location = glGetAttribLocation(mShaderprog, attribParam->name);
            mAttribParams.add(attribParam);
        }
    }
}

void OGLShader::getActiveUniformList() {
    const i32 numUniforms(getActiveParam(mShaderprog, GL_ACTIVE_UNIFORMS));
    if (numUniforms < 1) {
        return;
    }

    for (i32 i = 0; i < numUniforms; i++) {
        GLint actual_length(0), size(0);
        GLenum type;
        c8 name[MaxLen];
        ::memset(name, '\0', sizeof(c8) * MaxLen);
        glGetActiveUniform(mShaderprog, i, MaxLen, &actual_length, &size, &type, name);
        ActiveParameter *attribParam = new ActiveParameter;
        strncpy(attribParam->name, name, strlen(name));
        attribParam->location = glGetUniformLocation(mShaderprog, name);
        mAttribParams.add(attribParam);
    }
}

void OGLShader::logCompileOrLinkError(ui32 shaderprog) {
    GLint infoLogLength{ 0 };
    glGetProgramiv(shaderprog, GL_INFO_LOG_LENGTH, &infoLogLength);
    GLchar *infoLog = new GLchar[infoLogLength];
    ::memset(infoLog, 0, infoLogLength);
    glGetProgramInfoLog(shaderprog, infoLogLength, NULL, infoLog);
    String error(infoLog);
    Logger::getInstance()->print("Link log:\n" + error + "\n");
    delete[] infoLog;
}

bool OGLShader::isCompiled() const {
    return mIsCompiledAndLinked;
}

GLint OGLShader::getAttributeLocation(const String &attribute) {
    const GLint loc = mAttributeMap[attribute];
    return loc;
}

GLint OGLShader::getUniformLocation(const String &uniform) {
    if (uniform.empty()) {
        return InvalidLocationId;
    }

    std::map<String, GLint>::iterator it = mUniformLocationMap.find(uniform);
    if (mUniformLocationMap.end() == it) {
        return InvalidLocationId;
    }
    const GLint loc = mUniformLocationMap[uniform];
    return loc;
}

} // namespace OSRE::RenderBackend