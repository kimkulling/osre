#include <osre/RenderBackend/Shader.h>

namespace OSRE {
namespace RenderBackend {

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

} // Namespace RenderBackend
} // Namespace OSRE
