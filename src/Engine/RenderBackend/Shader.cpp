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
#include <osre/RenderBackend/Shader.h>
#include <osre/IO/IOService.h>
#include <osre/IO/Stream.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;  
using namespace ::OSRE::IO;

Shader::Shader() {
    ::memset(m_compileState, 0, sizeof(CompileState)*MaxCompileState);
}

Shader::~Shader() {
    // empty
}

void Shader::setSource(ShaderType type, const String &src) {
    if (src != m_src[(ui32)type]) {
        m_src[(ui32)type] = src;
        m_compileState[(ui32)type] = Updated;
    }
}

ShaderType Shader::getTypeFromeExtension( const String &extension ) {
    if (extension.empty()) {
        return ShaderType::InvalidShaderType;
    }

    if (extension == "vs") {
        return ShaderType::SH_VertexShaderType;
    } else if (extension == "fs") {
        return ShaderType::SH_FragmentShaderType;
    } else if (extension == "gs") {
        return ShaderType::SH_GeometryShaderType;
    } else if (extension == "ts") {
        return ShaderType::SH_TesselationShaderType;
    }

    return ShaderType::InvalidShaderType;
}

ShaderLoader::ShaderLoader() {
    // empty
}

ShaderLoader::~ShaderLoader() {
    // empty
}

size_t ShaderLoader::load( const IO::Uri &uri, Shader *shader ) {
    if (nullptr == shader) {
        return 0;
    }

    Stream *stream = IOService::getInstance()->openStream(uri, Stream::AccessMode::ReadAccess);
    if (nullptr == stream) {
        stream->close();
        return 0;
    }

    const String &ext = uri.getExtension();
    size_t size = stream->getSize();
    CPPCore::TArray<c8> buffer;
    buffer.resize(size);
    size_t readSize = stream->read( &buffer[0], size);
    if (readSize == 0) {
        return 0;
    }

    String shaderSrc = &buffer[0];
    ShaderType type = Shader::getTypeFromeExtension(ext);
    if (type == ShaderType::InvalidShaderType) {
        stream->close();
        return 0;
    }

    shader->m_src[(size_t)type] = shaderSrc;
    stream->close();
 
    return size;
}

bool ShaderLoader::unload( Shader *shader ) {
    if (nullptr == shader) {
        return false;
    }

    shader->setSource(ShaderType::SH_VertexShaderType, "");
    shader->setSource(ShaderType::SH_FragmentShaderType, "");
    shader->setSource(ShaderType::SH_GeometryShaderType, "");
    shader->setSource(ShaderType::SH_TesselationShaderType, "");

    return true;
}

ShaderResource::ShaderResource(const String &shaderName, const IO::Uri &uri) :
        TResource<Shader, ShaderLoader>(shaderName, uri) {
    // empty
}

ShaderResource::~ShaderResource() {
    // empty
}

Common::ResourceState ShaderResource::onLoad( const IO::Uri &uri, ShaderLoader &loader ) {
    if (getState() == ResourceState::Loaded) {
        return getState();
    }
    Shader *shader = create();
    loader.load(uri, shader);
    return getState();
}

Common::ResourceState ShaderResource::onUnload( ShaderLoader &loader ) {
    return getState();
}

} // Namespace RenderBackend
} // Namespace OSRE
