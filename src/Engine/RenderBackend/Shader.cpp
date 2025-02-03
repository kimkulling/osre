/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "RenderBackend/Shader.h"
#include "IO/IOService.h"
#include "IO/Stream.h"

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;
using namespace ::OSRE::IO;

Shader::Shader(const String &name) :
        mName(name), mUniformBuffer(), mVertexAttributes(), mSrc{}, mCompileState{} {
    ::memset(mCompileState, 0, sizeof(CompileState) * Count);
}

void Shader::addVertexAttribute(const String &name) {
    if (name.empty()) {
        return;
    }

    mVertexAttributes.add(name);
}

void Shader::addVertexAttributes(const String *names, size_t numAttributes) {
    if (numAttributes == 0 || nullptr == names) {
        return;
    }

    for (size_t i = 0; i < numAttributes; ++i) {
        mVertexAttributes.add(names[i]);
    }
}

const c8 *Shader::getVertexAttributeAt(size_t index) const {
    if (index >= mVertexAttributes.size()) {
        return nullptr;
    }

    return mVertexAttributes[index].c_str();
}

void Shader::addUniformBuffer(const String &name) {
    if (name.empty()) {
        return;
    }

    mUniformBuffer.add(name);
}

const c8 *Shader::getUniformBufferAt(size_t index) const {
    if (index >= mUniformBuffer.size()) {
        return nullptr;
    }

    return mUniformBuffer[index].c_str();
}

void Shader::setSource(ShaderType type, const String &src) {
    const size_t index = static_cast<size_t>(type);
    if (src == mSrc[index]) {
        return;
    }

    mSrc[index] = src;
    mCompileState[index] = Updated;
}

const c8 *Shader::getSource(ShaderType type) const {
    return mSrc[static_cast<size_t>(type)].c_str();
}

static constexpr size_t InvalidLocation = 9999;

size_t Shader::getLocation( const c8 *vertexAttribute ) const {
    if (mVertexAttributes.isEmpty()) {
        return InvalidLocation;
    }

    for (size_t location = 0; location < mVertexAttributes.size(); ++location) {
        if (mVertexAttributes[location] == vertexAttribute) {
            return location;
        }
    }

    return InvalidLocation;
}

ShaderType Shader::getTypeFromeExtension(const String &extension) {
    if (extension.empty()) {
        return ShaderType::Invalid;
    }

    if (extension == "vs") {
        return ShaderType::SH_VertexShaderType;
    }
    if (extension == "fs") {
        return ShaderType::SH_FragmentShaderType;
    }
    if (extension == "gs") {
        return ShaderType::SH_GeometryShaderType;
    }
    if (extension == "ts") {
        return ShaderType::SH_TesselationShaderType;
    }

    return ShaderType::Invalid;
}

size_t ShaderLoader::load(const Uri &uri, Shader *shader) {
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
    cppcore::TArray<c8> buffer;
    buffer.resize(size);
    size_t readSize = stream->read( &buffer[0], size);
    if (readSize == 0) {
        return 0;
    }

    String shaderSrc = &buffer[0];
    ShaderType type = Shader::getTypeFromeExtension(ext);
    if (type == ShaderType::Invalid) {
        stream->close();
        return 0;
    }
    shader->setSource(type, shaderSrc);
    stream->close();

    return size;
}

bool ShaderLoader::unload(Shader *shader) {
    if (nullptr == shader) {
        return false;
    }

    shader->setSource(ShaderType::SH_VertexShaderType, "");
    shader->setSource(ShaderType::SH_FragmentShaderType, "");
    shader->setSource(ShaderType::SH_GeometryShaderType, "");
    shader->setSource(ShaderType::SH_TesselationShaderType, "");

    return true;
}

ShaderResource::ShaderResource(const String &shaderName, const Uri &uri) :
        TResource<Shader, ShaderLoader>(shaderName, uri) {
    // empty
}

ResourceState ShaderResource::onLoad( const Uri &uri, ShaderLoader &loader ) {
    if (getState() == ResourceState::Loaded) {
        return getState();
    }

    Shader *shader = create(uri.getResource());
    if (loader.load(uri, shader) == 0l) {
        setState(ResourceState::Unloaded);
        return getState();
    }

    return getState();
}

ResourceState ShaderResource::onUnload( ShaderLoader &loader) {
    if (getState() == ResourceState::Unloaded) {
        return getState();
    }

    loader.unload(get());

    return getState();
}

} // Namespace RenderBackend
} // Namespace OSRE
