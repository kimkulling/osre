#include <osre/RenderBackend/Shader.h>
#include <osre/IO/IOService.h>
#include <osre/io/Stream.h>

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
