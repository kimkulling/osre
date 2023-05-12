#include <osre/RenderBackend/Material.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace RenderBackend {

Material::Material(const String &name, const IO::Uri &uri) :
        m_name(name),
        m_type(MaterialType::ShaderMaterial),
        m_numTextures(0),
        m_textures(nullptr),
        m_shader(nullptr),
        m_numParameters(0),
        m_parameters(nullptr),
        mShineness(0.0f),
        mShinenessStrength(0.0f),
        mUri(uri) {
    // empty
}

void Material::setMaterialType(MaterialType matType) {
    m_type = matType;
}

Material::~Material() {
    delete m_shader;
    delete[] m_textures;
}

MaterialType Material::getMaterialType() const {
    return m_type;
}

void Material::createShader(ShaderSourceArray &shaders) {
    m_shader = new Shader;
    for (ui32 i = 0; i < MaxShaderTypes; ++i) {
        if (!shaders[i].empty()) {
            m_shader->setSource(static_cast<ShaderType>(i), shaders[i]);
        }
    }
}

} // namespace RenderBackend
} // namespace OSRE
